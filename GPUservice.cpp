#include "GPUservice.h"
#include "tekstura.h"

void createMesh(const Shape& s, GPUmesh& m){
    m.indexCount = (GLsizei)s.Indices.size();
    
    const std::string& texPath = m.material.texturePath;
    addText2Mesh(m);

    glGenVertexArrays( 1, &m.VAO );
    glGenBuffers( 1, &m.VBO );
    glGenBuffers( 1, &m.EBO );

    glBindVertexArray( m.VAO );

    glBindBuffer( GL_ARRAY_BUFFER, m.VBO );
    glBufferData( GL_ARRAY_BUFFER, s.Vertices.size() * sizeof( Vertex ), s.Vertices.data(), GL_STATIC_DRAW );

    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m.EBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, s.Indices.size() * sizeof( unsigned int ), s.Indices.data(), GL_STATIC_DRAW );

    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)0 );
    glEnableVertexAttribArray( 0 );

    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, uv ) );
    glEnableVertexAttribArray( 1 );

    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), (void*)offsetof( Vertex, normal ) );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 );
}


void computeSceneBoundsAndDistance(
    const std::vector<float> pos,
    glm::vec3& outCenter,
    float& outDistance,
    float fovYRadians,
    float aspect,
    float margin) // margin >1 to give padding
{
    if (pos.empty()) {
        outCenter = glm::vec3(0.0f);
        outDistance = 5.0f;
        return;
    }

    
    glm::vec3 minV, maxV = glm::vec3( pos[0], pos[1], pos[2]);

    for(int i = 0; i<pos.size(); i+=3) {
        minV = glm::min(minV, glm::vec3( pos[i], pos[i+1], pos[i+2]));
        maxV = glm::max(maxV, glm::vec3( pos[i], pos[i+1], pos[i+2]));
    }

    outCenter = (minV + maxV) * 0.5f;
    glm::vec3 half = (maxV - minV) * 0.5f;

    // use half extents to compute required distance for vertical and horizontal fit
    float halfY = fabs(half.y);
    float halfX = fabs(half.x);

    // Avoid divide by zero in tan
    float tanHalfFov = tanf(fovYRadians * 0.5f);
    if (tanHalfFov <= 1e-6f) tanHalfFov = 1e-6f;

    // distance required to fit vertical and horizontal extents
    float dY = (halfY) / tanHalfFov;
    float dX = (halfX) / (tanHalfFov * aspect);

    // choose the larger, add depth extent as safety
    float d = glm::max(dX, dY);

    // also account for depth (z) extent so near plane doesn't cut object; add half.z
    d += fabs(half.z);

    outDistance = d * margin;
}