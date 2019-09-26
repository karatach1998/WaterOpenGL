#pragma once


// Local
#include "animator.h"


class Wave
{
public:
    enum class Type {
        DIRECTIONAL,
        CIRCULAR,
    };

public:
    Wave(float Q, float L, float A, float S, const glm::vec2& D);

    float Q() const;
    float A() const;
    float w() const;
    float fi() const;
    glm::vec2 D() const;

    void setAmplitudeAnimator(const std::shared_ptr<Animator>& ptr);
    void setDirectionAnimator(const std::shared_ptr<Animator>& ptr);

private:

private:
    float m_Q;
    float m_A;
    float m_w;
    float m_fi;
    glm::vec2 m_D;

    std::shared_ptr<Animator> m_amplitudeAnimator;
    std::shared_ptr<Animator> m_directionAnimator;
};
