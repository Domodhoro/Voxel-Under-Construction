#ifndef MESH_HPP
#define MESH_HPP

namespace Mesh {

constexpr auto SIZE {16};

enum class TEXTURE_TYPE : short int {
    AIR,
    GRASS,
    DIRT,
    STONE
};

struct faces {
    bool F {true}, B {true}, R {true}, L {true}, U {true}, D {true};
};

struct vertex {
    unsigned int count {0u};

    std::vector<glm::tvec3<float>> position, texture;
    std::vector<short int> type;
};

class Mesh {
public:
    Mesh(glm::tvec3<long int> position, const std::unique_ptr<FastNoiseLite>& noise) : m_position {position} {
        m_position *= glm::tvec3<float>(static_cast<float>(SIZE));

        const auto amplitude {8.0f};

        for (auto x = 0; x != SIZE; ++x) {
            for (auto y = 0; y != SIZE; ++y) {
                for (auto z = 0; z != SIZE; ++z) {
                    auto gain {
                        noise->GetNoise(static_cast<float>(x + m_position.z), static_cast<float>(z + m_position.x))
                    };

                    if (y < 1 + std::abs(std::floor(amplitude * gain))) {
                        m_vertex.type.emplace_back(static_cast<short int>(TEXTURE_TYPE::GRASS));
                    } else {
                        m_vertex.type.emplace_back(static_cast<short int>(TEXTURE_TYPE::AIR));
                    }
                }
            }
        }

        for (auto x = 0; x != SIZE; ++x) {
            for (auto y = 0; y != SIZE; ++y) {
                for (auto z = 0; z != SIZE; ++z) {
                    if (m_vertex.type.at(x + y * SIZE + z * SIZE * SIZE) == static_cast<short int>(TEXTURE_TYPE::AIR)) {
                        continue;
                    }

                    faces faces;

                    if (x > 0 && m_vertex.type.at((x - 1) + y * SIZE + z * SIZE * SIZE) != static_cast<short int>(TEXTURE_TYPE::AIR)) {
                        faces.L = false;
                    }

                    if (y > 0 && m_vertex.type.at(x + (y - 1) * SIZE + z * SIZE * SIZE) != static_cast<short int>(TEXTURE_TYPE::AIR)) {
                        faces.D = false;
                    }

                    if (z > 0 && m_vertex.type.at(x + y * SIZE + (z - 1) * SIZE * SIZE) != static_cast<short int>(TEXTURE_TYPE::AIR)) {
                        faces.F = false;
                    }

                    if (x < (SIZE - 1) && m_vertex.type.at((x + 1) + y * SIZE + z * SIZE * SIZE) != static_cast<short int>(TEXTURE_TYPE::AIR)) {
                        faces.R = false;
                    }

                    if (y < (SIZE - 1) && m_vertex.type.at(x + (y + 1) * SIZE + z * SIZE * SIZE) != static_cast<short int>(TEXTURE_TYPE::AIR)) {
                        faces.U = false;
                    }

                    if (z < (SIZE - 1) && m_vertex.type.at(x + y * SIZE + (z + 1) * SIZE * SIZE) != static_cast<short int>(TEXTURE_TYPE::AIR)) {
                        faces.B = false;
                    }

                    glm::tvec3<float> position {
                            static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)
                    };

                    build(position + static_cast<glm::tvec3<float>>(m_position), m_vertex.type.at(x + y * SIZE + z * SIZE * SIZE), faces);
                }
            }
        }

        setup();
    }

    virtual ~Mesh() {
        glDeleteVertexArrays(1, &m_vertexArray);
    }

    unsigned int getVertexArray() {
        return m_vertexArray;
    }

    unsigned int getCount() {
        return m_vertex.count;
    }

protected:
    unsigned int m_vertexArray {0u};

    glm::tvec3<long int> m_position {0};

    vertex m_vertex;

    void setup() {
        glGenVertexArrays(1, &m_vertexArray);
        glBindVertexArray(m_vertexArray);

        auto vertexBuffer {0u}, textureBuffer {0u};

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_vertex.position.size() * sizeof(glm::tvec3<float>), &m_vertex.position.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::tvec3<float>), static_cast<void*>(0));
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &textureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
        glBufferData(GL_ARRAY_BUFFER, m_vertex.texture.size() * sizeof(glm::tvec3<float>), &m_vertex.texture.at(0), GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(glm::tvec3<float>), static_cast<void*>(0));
        glEnableVertexAttribArray(1);

        m_vertex.position.clear();
        m_vertex.texture.clear();

        if (m_vertexArray == 0u) {
            throw std::string {"Falha ao criar 'vertexArray' [mesh]."};
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void build(const glm::tvec3<float>& position, const short int type, const faces& faces) {
        if (faces.F) {
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 0.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 1.0f, 0.0f} + position);

            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});

            m_vertex.count += 6u;
        }

        if (faces.B) {
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 0.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 1.0f} + position);

            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});

            m_vertex.count += 6u;
        }

        if (faces.R) {
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 0.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 0.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 0.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 0.0f} + position);

            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});

            m_vertex.count += 6u;
        }

        if (faces.L) {
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 1.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 0.0f} + position);

            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});

            m_vertex.count += 6u;
        }

        if (faces.U) {
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 1.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 1.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 1.0f, 0.0f} + position);

            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});

            m_vertex.count += 6u;
        }

        if (faces.D) {
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 0.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 0.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 0.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{0.0f, 0.0f, 1.0f} + position);
            m_vertex.position.emplace_back(glm::tvec3<float>{1.0f, 0.0f, 0.0f} + position);

            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{1.0f, 1.0f, static_cast<float>(type)});
            m_vertex.texture.emplace_back(glm::tvec3<float>{0.0f, 0.0f, static_cast<float>(type)});

            m_vertex.count += 6u;
        }
    }
};

}

#endif
