#include "convertOBJ.h"

void convertOBJToMeshIndexed(
    const std::vector<float>& positions,
    const std::vector<float>& texcoords,
    const std::vector<float>& normals,
    const std::vector<int>& faces,
    std::vector<Vertex>& outVertices,
    std::vector<unsigned int>& outIndices ) 
{
    int sizePOS3 = positions.size();
    int sizeTEX2 = texcoords.size();
    int sizeNORM3 = normals.size();
    std::unordered_map<OBJIndex, int, OBJIndexHash> indexMap;
    int t1,t2,t3;

    for (size_t i = 0; i < faces.size(); i += 3) {
        t1 = fixObjIndex(faces[i], sizePOS3 );
        t2 = fixObjIndex(faces[i+ 1], sizeTEX2 );
        t3 = fixObjIndex(faces[i + 2], sizeNORM3 );

        OBJIndex key {
            t1      - 1,
            t2      - 1,
            t3      - 1
        };

        auto it = indexMap.find(key);
        if (it == indexMap.end()) {
            Vertex vert;

            vert.pos = glm::vec3(
                positions[key.v * 3 + 0],
                positions[key.v * 3 + 1],
                positions[key.v * 3 + 2]
            );

            // UV can be missing in OBJ (vt = -1)
            vert.uv = glm::vec2( 0.0f, 0.0f );
            if (key.vt >= 0 && (key.vt * 2 + 1) < (int)texcoords.size()) {
                vert.uv = glm::vec2(
                    texcoords[key.vt * 2 + 0],
                    texcoords[key.vt * 2 + 1]
                );
            }

            // Normal can be missing in OBJ (vn = -1)
            vert.normal = glm::vec3( 0.0f, 1.0f, 0.0f );
            if (key.vn >= 0 && (key.vn * 3 + 2) < (int)normals.size()) {
                vert.normal = glm::vec3(
                    normals[key.vn * 3 + 0],
                    normals[key.vn * 3 + 1],
                    normals[key.vn * 3 + 2]
                );
            }

            unsigned int newIndex = static_cast<unsigned int>(outVertices.size());
            outVertices.push_back(vert);
            outIndices.push_back(newIndex);
            indexMap[key] = newIndex;
        } else {
            outIndices.push_back(it->second);
        }
    }
}

static int fixObjIndex( int idx, int count )
{
    if (idx > 0) return idx;                 // zostaje 1-based
    if (idx < 0) return count + idx + 1;     // np. count=10, idx=-1 => 10
    return 0;                                // brak
}