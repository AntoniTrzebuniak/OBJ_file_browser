#pragma once
#include "convertOBJ.h"
#include "loaders/readMTL.h"


struct Shape{ std::vector<Vertex> Vertices; std::vector<unsigned int> Indices; std::vector<std::string> materialsNames;};
struct GPUmesh
{
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLsizei indexCount = 0;
    GLuint textureID = 0;
    Mtl& material;
    GPUmesh(Mtl& mat)
        : material(mat)
    {}
};

void createMesh(const Shape& s, GPUmesh& m);
/* Mo¿na wywo³ywaæ wy³¹cznie po stworzeniu kontekstu graficznego!!! */

void computeSceneBoundsAndDistance(
    const std::vector<float> pos,
    glm::vec3& outCenter,
    float& outDistance,
    float fovYRadians,
    float aspect,
    float margin = 1.2f);