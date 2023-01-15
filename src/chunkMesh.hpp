#ifndef CHUNK_MESH_HPP
#define CHUNK_MESH_HPP

struct Vertex {
    float X {0.0f}, Y {0.0f}, Z {0.0f}, U {0.0f}, V {0.0f}, T {0.0f};
};

unsigned int Mesh(std::vector<Vertex> &Vertice, int x, int y, int z, bool &F, bool &B, bool &R, bool &L, bool &U, bool &D, int texture) {
    const auto X {static_cast<float>(x)}, Y {static_cast<float>(y)}, Z {static_cast<float>(z)};

    struct {
        float F {0.0f}, B {0.0f}, R {0.0f}, L {0.0f}, U {0.0f}, D {0.0f};
    } Texture;

    if (texture == 1.0f) {
        Texture.F = 2.0f;
        Texture.B = 2.0f;
        Texture.R = 2.0f;
        Texture.L = 2.0f;
        Texture.U = 1.0f;
        Texture.D = 3.0f;
    } else if (texture == 2.0f) {
        Texture.F = 3.0f;
        Texture.B = 3.0f;
        Texture.R = 3.0f;
        Texture.L = 3.0f;
        Texture.U = 3.0f;
        Texture.D = 3.0f;
    } else if (texture == 3.0f) {
        Texture.F = 4.0f;
        Texture.B = 4.0f;
        Texture.R = 4.0f;
        Texture.L = 4.0f;
        Texture.U = 4.0f;
        Texture.D = 4.0f;
    }

    auto count {0u};

    if (F) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, Texture.F});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, Texture.F});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, Texture.F});

        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, Texture.F});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, Texture.F});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, Texture.F});

        count += 6u;
    }

    if (B) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, Texture.B});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, Texture.B});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, Texture.B});

        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, Texture.B});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, Texture.B});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, Texture.B});

        count += 6u;
    }

    if (R) {
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, Texture.R});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, Texture.R});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, Texture.R});

        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, Texture.R});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, Texture.R});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, Texture.R});

        count += 6u;
    }

    if (L) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, Texture.L});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, Texture.L});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, Texture.L});

        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, Texture.L});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, Texture.L});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, Texture.L});

        count += 6u;
    }

    if (U) {
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, Texture.U});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, Texture.U});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, Texture.U});

        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, Texture.U});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, Texture.U});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 0.0f, Texture.U});

        count += 6u;
    }

    if (D) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, Texture.D});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, Texture.D});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, Texture.D});

        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 1.0f, Texture.D});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, Texture.D});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, Texture.D});

        count += 6u;
    }

    return count;
}

#endif
