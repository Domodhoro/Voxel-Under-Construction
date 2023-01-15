#ifndef CAMERA_HPP
#define CAMERA_HPP

enum class MOVEMENTS : int {
    FORWARD = 0,
    BACKWARD,
    RIGHT,
    LEFT
};

class Camera {
public:
    void setPosition(const glm::tvec3<float> Position) {
        m_Position = Position;
    }

    void setSpeed(const float speed) {
        m_speed = speed;
    }

    void setFov(const float fov) {
        m_fov = fov;
    }

    void setAspect(const float aspect) {
        m_aspect = aspect;
    }

    void setNearPlane(const float near) {
        m_near = near;
    }

    void setFarPlane(const float far) {
        m_far = far;
    }

    glm::tvec3<float>& getPosition() {
        return m_Position;
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    void keyboardProcess(MOVEMENTS movements) {
        if (movements == MOVEMENTS::FORWARD) {
            m_Position += m_speed * m_Front;
        }

        if (movements == MOVEMENTS::BACKWARD) {
            m_Position -= m_speed * glm::normalize(glm::cross(m_Front, m_Up));
        }

        if (movements == MOVEMENTS::RIGHT) {
            m_Position -= m_speed * m_Front;
        }

        if (movements == MOVEMENTS::LEFT) {
            m_Position += m_speed * glm::normalize(glm::cross(m_Front, m_Up));
        }
    }

    void mouseProcess(double *offSetX, double *offSetY) {
        m_alpha += *offSetX;
        m_beta += *offSetY;

        const auto angleMax {89.0f};

        if (m_beta >= angleMax) {
            m_beta = angleMax;
        }

        if (m_beta <= -angleMax) {
            m_beta = -angleMax;
        }

        glm::tvec3<float> Direction {
            std::cos(glm::radians(m_alpha)) * std::cos(glm::radians(m_beta)),
            std::sin(glm::radians(m_beta)),
            std::sin(glm::radians(m_alpha)) * std::cos(glm::radians(m_beta))
        };

        m_Front = glm::normalize(Direction);
    }

private:
    float m_fov {60.0f}, m_aspect {1.0f}, m_near {0.001f}, m_far {1000.0f}, m_speed {1.0f}, m_alpha {90.0f}, m_beta {0.0f};

    glm::tvec3<float> m_Front {0.0f, 0.0f, 1.0f}, m_Position {0.0f}, m_Up {0.0f, 1.0f, 0.0f};
};

#endif
