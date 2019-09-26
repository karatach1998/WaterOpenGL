
// C++
#include <cmath>

// Local
#include "camera.h"


Camera::Camera(glm::vec3 pos, glm::vec3 up, float pitch, float yaw)
    : m_pos(pos), m_up(up), m_front(-pos), m_pitch(pitch), m_yaw(yaw) { }


glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_pos, m_pos + m_front, m_up);
}


float Camera::zoom() const
{
    return m_zoom;
}


glm::vec3 Camera::position() const
{
    return m_pos;
}


void Camera::processInput(const Input& input)
{
    constexpr float camera_speed = 0.005f;

    if (input.isKeyPressed(GLFW_KEY_W) || input.isKeyHolded(GLFW_KEY_W))
        m_pos += camera_speed * m_front;
    if (input.isKeyPressed(GLFW_KEY_S) || input.isKeyHolded(GLFW_KEY_S))
        m_pos -= camera_speed * m_front;
    if (input.isKeyPressed(GLFW_KEY_A) || input.isKeyHolded(GLFW_KEY_A))
        m_pos -= camera_speed * glm::normalize(glm::cross(m_front, m_up));
    if (input.isKeyPressed(GLFW_KEY_D) || input.isKeyHolded(GLFW_KEY_D))
        m_pos += camera_speed * glm::normalize(glm::cross(m_front, m_up));
    
    double xpos, ypos;
    double yscroll;
    
    input.getMousePosition(xpos, ypos);
    yscroll = input.getYScroll();

    if (m_first)
    {
        m_xLastPos = xpos;
        m_yLastPos = ypos;
        m_yLastScroll = yscroll;
        m_first = false;
    }
    
    float xoffset = static_cast<float>(xpos - m_xLastPos);
    float yoffset = static_cast<float>(ypos - m_yLastPos);

    m_xLastPos = xpos;
    m_yLastPos = ypos;

    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    m_yaw += xoffset;
    m_pitch -= yoffset;

    if (m_pitch >= 89.0f)
        m_pitch = 89.0f;
    else if (m_pitch <= -89.0f)
        m_pitch = -89.0f;

    m_front.x = std::cos(glm::radians(m_pitch)) * std::sin(glm::radians(m_yaw));
    m_front.y = std::sin(glm::radians(m_pitch));
    m_front.z = -std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_yaw));

    float yscrolloffset = yscroll - m_yLastScroll;
    m_yLastScroll = yscroll;
    
    m_zoom -= yscrolloffset;
    if (m_zoom > 45.0f)
        m_zoom = 45.0f;
    else if (m_zoom < 1.0f)
        m_zoom = 1.0f;
}
