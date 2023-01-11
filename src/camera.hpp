#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace Camera {

enum class MOVEMENTS : int {
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT
};

class Camera {
public:
    void setPosition(const glm::tvec3<float> position) {
        m_position = position;
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
        return m_position;
    }

    glm::mat4 getProjectionMatrix() {
        return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void keyboardProcess(MOVEMENTS movements) {
        if (movements == MOVEMENTS::FORWARD) {
            m_position += m_speed * m_front;
        }

        if (movements == MOVEMENTS::BACKWARD) {
            m_position -= m_speed * glm::normalize(glm::cross(m_front, m_up));
        }

        if (movements == MOVEMENTS::RIGHT) {
            m_position -= m_speed * m_front;
        }

        if (movements == MOVEMENTS::LEFT) {
            m_position += m_speed * glm::normalize(glm::cross(m_front, m_up));
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

        glm::tvec3<float> direction {
            std::cos(glm::radians(m_alpha)) * std::cos(glm::radians(m_beta)),
            std::sin(glm::radians(m_beta)),
            std::sin(glm::radians(m_alpha)) * std::cos(glm::radians(m_beta))
        };

        m_front = glm::normalize(direction);
    }

private:
    float m_fov {60.0f}, m_aspect {1.0f}, m_near {0.001f}, m_far {1000.0f}, m_speed {1.0f}, m_alpha {90.0f}, m_beta {0.0f};

    glm::tvec3<float> m_front {0.0f, 0.0f, 1.0f}, m_position {0.0f}, m_up {0.0f, 1.0f, 0.0f};
};

}

#endif
