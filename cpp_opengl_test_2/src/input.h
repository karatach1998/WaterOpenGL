#pragma once


// C++
#include <memory>


#define MAX_KEYS 1000
#define MAX_BUTTONS 16


class Input
{
public:
    Input() = default;
    Input(GLFWwindow* window);

    template <typename... Args> static inline
    auto create(Args... args)
        { return std::make_shared<Input>(args...); }

    bool isKeyPressed(int key) const;
    bool isKeyHolded(int key) const;
    bool isButtonPressed(int button) const;
    bool isButtonHolded(int key) const;
    void getMousePosition(double& xpos, double& ypos) const;
    void getScroll(double& rx, double& ry) const;
    double getYScroll() const;

    friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void button_callback(GLFWwindow* window, int button, int action, int mods);
    friend void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
    friend void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

private:
    int m_keys[MAX_KEYS];
    int m_buttons[MAX_BUTTONS];
    double m_xPos, m_yPos;
    double m_xScroll, m_yScroll;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
