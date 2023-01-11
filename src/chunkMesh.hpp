#ifndef CHUNK_MESH_HPP
#define CHUNK_MESH_HPP

namespace ChunkMesh {

struct Vertex {
    float x {0.0f}, y {0.0f}, z {0.0f}, u {0.0f}, v {0.0f}, t {0.0f};
};

unsigned int Mesh(std::vector<Vertex>& vertice, int X, int Y, int Z, bool F, bool B, bool R, bool L, bool U, bool D, int blockType) {
    const auto x {static_cast<float>(X)}, y {static_cast<float>(Y)}, z {static_cast<float>(Z)};

    auto amount {0u};

    if (F) {
        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 0.0f, blockType});
        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f, blockType});
        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f, blockType});

        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 1.0f, 0.0f, blockType});
        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f, blockType});
        vertice.push_back({x - 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, blockType});

        amount += 6u;
    }

    if (B) {
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 0.0f, 0.0f, blockType});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, blockType});
        vertice.push_back({x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 0.0f, blockType});

        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 0.0f, 1.0f, blockType});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, blockType});
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 0.0f, 0.0f, blockType});

        amount += 6u;
    }

    if (R) {
        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 0.0f, blockType});
        vertice.push_back({x + 0.5f, y - 0.5f, z + 0.5f, 0.0f, 0.0f, blockType});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 0.0f, 1.0f, blockType});

        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 1.0f, 0.0f, blockType});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 0.0f, 1.0f, blockType});
        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 1.0f, 1.0f, blockType});

        amount += 6u;
    }

    if (L) {
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 0.0f, blockType});
        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f, blockType});
        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, blockType});

        vertice.push_back({x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f, blockType});
        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, blockType});
        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f, blockType});

        amount += 6u;
    }

    if (U) {
        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f, blockType});
        vertice.push_back({x + 0.5f, y + 0.5f, z + 0.5f, 1.0f, 1.0f, blockType});
        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 0.0f, blockType});

        vertice.push_back({x + 0.5f, y + 0.5f, z - 0.5f, 0.0f, 1.0f, blockType});
        vertice.push_back({x - 0.5f, y + 0.5f, z + 0.5f, 1.0f, 0.0f, blockType});
        vertice.push_back({x - 0.5f, y + 0.5f, z - 0.5f, 0.0f, 0.0f, blockType});

        amount += 6u;
    }

    if (D) {
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, blockType});
        vertice.push_back({x + 0.5f, y - 0.5f, z + 0.5f, 1.0f, 0.0f, blockType});
        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f, blockType});

        vertice.push_back({x - 0.5f, y - 0.5f, z - 0.5f, 0.0f, 1.0f, blockType});
        vertice.push_back({x - 0.5f, y - 0.5f, z + 0.5f, 1.0f, 1.0f, blockType});
        vertice.push_back({x + 0.5f, y - 0.5f, z - 0.5f, 0.0f, 0.0f, blockType});

        amount += 6u;
    }

    return amount;
}

}

#endif
