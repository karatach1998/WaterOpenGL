#pragma once


// Local
#include "input.h"


#define YAW    0.0f
#define PITCH  0.0f
#define SPEED  2.5f
#define SENSITIVITY 0.1f
#define ZOOM        45.0f


class Camera
{
public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 up, float pitch = 0.0f, float yaw = 0.0f);

    glm::mat4 getViewMatrix() const;
    float zoom() const;
    glm::vec3 position() const;

    virtual void processInput(const Input& input);

protected:
    glm::vec3 m_pos;
    glm::vec3 m_up;
    glm::vec3 m_front;

    float m_pitch = PITCH;
    float m_yaw   = YAW;
    float m_zoom  = ZOOM;

    double m_xLastPos;
    double m_yLastPos;
    double m_yLastScroll;
    bool m_first = true;
};
