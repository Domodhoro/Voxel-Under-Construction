#ifndef CHUNK_MESH_HPP
#define CHUNK_MESH_HPP

namespace chunk_mesh {

void mesh(std::vector<util::vertex> &vertice, unsigned int &count, const int x, const int y, const int z, const util::face &face, const int block_type) {
    const auto X {static_cast<float>(x)};
    const auto Y {static_cast<float>(y)};
    const auto Z {static_cast<float>(z)};

    util::face_texture tex;

    switch (block_type) {
    case static_cast<int>(util::BLOCK_TYPE::GRASS):
        tex = {2.0f, 2.0f, 2.0f, 2.0f, 1.0f, 3.0f};
        break;
    case static_cast<int>(util::BLOCK_TYPE::DIRT):
        tex = {3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f};
        break;
    case static_cast<int>(util::BLOCK_TYPE::STONE):
        tex = {4.0f, 4.0f, 4.0f, 4.0f, 4.0f, 4.0f};
        break;
    case static_cast<int>(util::BLOCK_TYPE::SAND):
        tex = {5.0f, 5.0f, 5.0f, 5.0f, 5.0f, 5.0f};
        break;
    }

    if (face.F) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.F});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.F});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.F});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.F});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.F});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, tex.F});

        count += 6u;
    }

    if (face.B) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, tex.B});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.B});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.B});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, tex.B});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.B});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, tex.B});

        count += 6u;
    }

    if (face.R) {
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.R});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 0.0f, 0.0f, tex.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, tex.R});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 1.0f, 0.0f, tex.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 0.0f, 1.0f, tex.R});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 1.0f, 1.0f, tex.R});

        count += 6u;
    }

    if (face.L) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.L});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.L});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.L});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.L});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.L});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.L});

        count += 6u;
    }

    if (face.U) {
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.U});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.U});
        vertice.push_back({X + 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.U});
        vertice.push_back({X - 0.5f, Y + 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.U});

        count += 6u;
    }

    if (face.D) {
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 0.0f, tex.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.D});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 1.0f, tex.D});
        vertice.push_back({X - 0.5f, Y - 0.5f, Z + 0.5f, 1.0f, 1.0f, tex.D});
        vertice.push_back({X + 0.5f, Y - 0.5f, Z - 0.5f, 0.0f, 0.0f, tex.D});

        count += 6u;
    }
}

}

#endif
