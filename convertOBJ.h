#pragma once

#include <unordered_map>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include "loaders/LoadOBJ.h"

struct OBJIndex {
    int v;
    int vt;
    int vn;

    bool operator==(const OBJIndex& other) const {
        return v == other.v &&
            vt == other.vt &&
            vn == other.vn;
    }
};

struct OBJIndexHash {
    size_t operator()(const OBJIndex& i) const {
        size_t h1 = std::hash<int>()(i.v);
        size_t h2 = std::hash<int>()(i.vt);
        size_t h3 = std::hash<int>()(i.vn);

        return (h1 ^ (h2 << 1)) ^ (h3 << 2);
    }
};

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 normal;

    // potrzebne do mapy (porównywanie)
    bool operator==(const Vertex& other) const {
        return pos == other.pos &&
            uv == other.uv &&
            normal == other.normal;
    }
};

void convertOBJToMeshIndexed(
    const std::vector<float>& positions,
    const std::vector<float>& texcoords,
    const std::vector<float>& normals,
    const std::vector<int>& faces,
    std::vector<Vertex>& outVertices,
    std::vector<unsigned int>& outIndices);

static int fixObjIndex(int idx,int count);


