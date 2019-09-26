
#define _USE_MATH_DEFINES

// C++
#include <math.h>

// Local
#include "Wave.h"


Wave::Wave(float Q, float L, float A, float S, const glm::vec2& D)
    : m_Q(Q / (A * (2.0f / L)))
    , m_A(A)
    , m_w(2.0f / L)
    , m_fi(S * (2.0f / L))
    , m_D(D)
{
}


float Wave::Q() const
{
    return m_Q;
}


float Wave::A() const
{
    if (m_amplitudeAnimator)
        return m_A * m_amplitudeAnimator->get(glfwGetTime());
    else
        return m_A;
}


float Wave::w() const
{
    return m_w;
}


float Wave::fi() const
{
    return m_fi;
}


glm::vec2 Wave::D() const
{
    if (m_directionAnimator)
    {
        float rad = m_directionAnimator->get(glfwGetTime());
        return glm::vec2(std::sin(rad) + m_D.x, (1.0f - std::cos(rad)) * m_D.y);
    }
    else 
        return m_D;
}


void Wave::setAmplitudeAnimator(const std::shared_ptr<Animator>& ptr)
{
    m_amplitudeAnimator = ptr;
}


void Wave::setDirectionAnimator(const std::shared_ptr<Animator>& ptr)
{
    m_directionAnimator = ptr;
}
