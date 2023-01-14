#ifndef CHUNK_MESH_HPP
#define CHUNK_MESH_HPP

struct Vertex {
    float X {0.0f}, Y {0.0f}, Z {0.0f}, U {0.0f}, V {0.0f}, T {0.0f};
};

unsigned int Mesh(std::vector<Vertex> &Vertice, int x, int y, int z, bool F, bool B, bool R, bool L, bool U, bool D, float blockType) {
    const auto X {static_cast<float>(x)}, Y {static_cast<float>(y)}, Z {static_cast<float>(z)};

    auto amount {0u};

    if (F) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, blockType});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, blockType});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, blockType});

        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, blockType});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, blockType});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, blockType});

        amount += 6u;
    }

    if (B) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, blockType});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, blockType});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, blockType});

        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, blockType});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, blockType});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, blockType});

        amount += 6u;
    }

    if (R) {
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, blockType});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, blockType});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, blockType});

        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, blockType});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, blockType});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, blockType});

        amount += 6u;
    }

    if (L) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, blockType});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, blockType});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, blockType});

        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, blockType});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, blockType});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, blockType});

        amount += 6u;
    }

    if (U) {
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, blockType});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, blockType});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, blockType});

        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, blockType});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, blockType});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 0.0f, blockType});

        amount += 6u;
    }

    if (D) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, blockType});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, blockType});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, blockType});

        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 1.0f, blockType});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, blockType});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, blockType});

        amount += 6u;
    }

    return amount;
}

#endif
