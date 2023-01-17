#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace camera {

enum struct MOVEMENTS : int {
    FORWARD = 0,
    BACKWARD,
    RIGHT,
    LEFT
};

struct camera {
    void set_position  (glm::tvec3<float> Position) { m_Position = Position; }
    void set_speed     (float speed)                { m_speed    = speed; }
    void set_FOV       (float fov)                  { m_fov      = fov; }
    void set_aspect    (float aspect)               { m_aspect   = aspect; }
    void set_near_plane(float near)                 { m_near     = near; }
    void set_far_plane (float far)                  { m_far      = far; }

    glm::tvec3<float>& get_position          () { return m_Position; }
    glm::mat4          get_projection_matrix () { return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far); }
    glm::mat4          get_view_matrix       () { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }

    void keyboard_process(MOVEMENTS movements) {
        if (movements == MOVEMENTS::FORWARD)  m_Position += m_speed * m_Front;
        if (movements == MOVEMENTS::RIGHT)    m_Position -= m_speed * m_Front;
        if (movements == MOVEMENTS::BACKWARD) m_Position -= m_speed * glm::normalize(glm::cross(m_Front, m_Up));
        if (movements == MOVEMENTS::LEFT)     m_Position += m_speed * glm::normalize(glm::cross(m_Front, m_Up));
    }

    void mouse_process(double &offSetX, double &offSetY) {
        m_alpha += offSetX;
        m_beta  += offSetY;

        const auto angleMax {89.0f};

        if (m_beta >=  angleMax) m_beta =  angleMax;
        if (m_beta <= -angleMax) m_beta = -angleMax;

        m_Direction.x = std::cos(glm::radians(m_alpha)) * std::cos(glm::radians(m_beta));
        m_Direction.y = std::sin(glm::radians(m_beta));
        m_Direction.z = std::sin(glm::radians(m_alpha)) * std::cos(glm::radians(m_beta));

        m_Front = glm::normalize(m_Direction);
    }

private:
    float m_fov    {60.0f};
    float m_aspect {1.0f};
    float m_near   {0.001f};
    float m_far    {1000.0f};
    float m_speed  {1.0f};
    float m_alpha  {90.0f};
    float m_beta   {0.0f};

    glm::tvec3<float> m_Front     {0.0f, 0.0f, 1.0f};
    glm::tvec3<float> m_Position  {0.0f};
    glm::tvec3<float> m_Up        {0.0f, 1.0f, 0.0f};
    glm::tvec3<float> m_Direction {0.0f, 0.0f, 1.0f};
};

}

#endif

