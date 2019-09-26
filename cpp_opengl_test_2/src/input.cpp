
// Local
#include "input.h"



Input::Input(GLFWwindow* window)
{
    for (int i = 0; i < MAX_KEYS; ++i)
        m_keys[i] = GLFW_RELEASE;

    for (int i = 0; i < MAX_BUTTONS; ++i)
        m_buttons[i] = GLFW_RELEASE;

    glfwGetCursorPos(window, &m_xPos, &m_yPos);

    m_xScroll = 0.0;
    m_yScroll = 0.0;

    glfwSetWindowUserPointer(window, this);
}


bool Input::isKeyPressed(int key) const
{
    return m_keys[key] == GLFW_PRESS;
}


bool Input::isKeyHolded(int key) const
{
    return m_keys[key] == GLFW_REPEAT;
}


bool Input::isButtonPressed(int button) const
{
    return m_buttons[button] == GLFW_PRESS;
}


bool Input::isButtonHolded(int button) const
{
    return m_buttons[button] == GLFW_REPEAT;
}


void Input::getMousePosition(double& rx, double& ry) const
{
    rx = m_xPos;
    ry = m_yPos;
}


void Input::getScroll(double& rx, double& ry) const
{
    rx = m_xScroll;
    ry = m_yScroll;
}


double Input::getYScroll() const
{
    return m_yScroll;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

    assert(input != nullptr);

    if ((input->m_keys[key] = action) == GLFW_PRESS)
        std::cout << "Press key: " << key << std::endl;
    
}


void button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

    assert(input != nullptr);

    input->m_buttons[button] = action;
}


void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

    assert(input != nullptr);

    input->m_xPos = xpos;
    input->m_yPos = ypos;

    std::cout << "Mouse position: (" << xpos << ", " << ypos << ")" << std::endl;
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

    assert(input != nullptr);

    input->m_xScroll += xoffset;
    input->m_yScroll += yoffset;
}
