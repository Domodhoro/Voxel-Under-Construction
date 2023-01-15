#ifndef CHUNK_MESH_HPP
#define CHUNK_MESH_HPP

struct Vertex {
    float X {0.0f}, Y {0.0f}, Z {0.0f}, U {0.0f}, V {0.0f}, T {0.0f};
};

unsigned int mesh(std::vector<Vertex> &Vertice, int x, int y, int z, bool &front, bool &back, bool &right, bool &left, bool &up, bool &down, int blockID) {
    const auto X {static_cast<float>(x)}, Y {static_cast<float>(y)}, Z {static_cast<float>(z)};

    auto frontTexture {0.0f}, backTexture {0.0f}, rightTexture {0.0f}, leftTexture {0.0f}, upTexture {0.0f}, downTexture {0.0f};

    switch (blockID) {
    case 1:
        frontTexture = 2.0f;
        backTexture = 2.0f;
        rightTexture = 2.0f;
        leftTexture = 2.0f;
        upTexture = 1.0f;
        downTexture = 3.0f;
        break;
    case 2:
        frontTexture = 3.0f;
        backTexture = 3.0f;
        rightTexture = 3.0f;
        leftTexture = 3.0f;
        upTexture = 3.0f;
        downTexture = 3.0f;
        break;
    case 3:
        frontTexture = 4.0f;
        backTexture = 4.0f;
        rightTexture = 4.0f;
        leftTexture = 4.0f;
        upTexture = 4.0f;
        downTexture = 4.0f;
        break;
    case 4:
        frontTexture = 5.0f;
        backTexture = 5.0f;
        rightTexture = 5.0f;
        leftTexture = 5.0f;
        upTexture = 5.0f;
        downTexture = 5.0f;
        break;
    default:
        frontTexture = 0.0f;
        backTexture = 0.0f;
        rightTexture = 0.0f;
        leftTexture = 0.0f;
        upTexture = 0.0f;
        downTexture = 0.0f;
    }

    auto count {0u};

    if (front) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, frontTexture});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, frontTexture});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, frontTexture});

        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, frontTexture});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, frontTexture});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, frontTexture});

        count += 6u;
    }

    if (back) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, backTexture});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, backTexture});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, backTexture});

        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, backTexture});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, backTexture});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, backTexture});

        count += 6u;
    }

    if (right) {
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, rightTexture});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, rightTexture});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, rightTexture});

        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, rightTexture});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, rightTexture});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, rightTexture});

        count += 6u;
    }

    if (left) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, leftTexture});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, leftTexture});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, leftTexture});

        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, leftTexture});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, leftTexture});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, leftTexture});

        count += 6u;
    }

    if (up) {
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, upTexture});
        Vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, upTexture});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, upTexture});

        Vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, upTexture});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, upTexture});
        Vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 0.0f, upTexture});

        count += 6u;
    }

    if (down) {
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, downTexture});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, downTexture});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, downTexture});

        Vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 1.0f, downTexture});
        Vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, downTexture});
        Vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, downTexture});

        count += 6u;
    }

    return count;
}

#endif
