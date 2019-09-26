
#define _USE_MATH_DEFINES

// C++
#include <math.h>
#include <string>

// Local
#include "animator.h"


using namespace std::literals::string_literals;


bool Animator::fComp(const TimelinePoint& a, const TimelinePoint& b)
{
    return a.first > b.first;
}


float Animator::linearApprox(float x1, float y1, float x2, float y2, float x)
{
    return (x - x1) * (y2 - y1) / (x2 - x1) + y1;
}


float Animator::trigonometricApprox(float x1, float y1, float x2, float y2, float x)
{
    return std::cos(x / (x2 - x1) * M_PI_2) * (y1 - y2) + y1;
}



Animator::timeline_locked::timeline_locked(float time, float value)
    : std::exception("Error: ") { }


Animator::Animator()
    : m_timeline(fComp) { }


void Animator::addTimelinePoint(float time, float value)
{
    if (!m_locked)
        m_timeline.emplace(time, value);
    else
        throw timeline_locked(time, value);
}


void Animator::setDefaultApproxFunc(ApproxFunc f)
{
    m_defApproxFunc = f;
}


float Animator::get(float time)
{
    return get(time, m_defApproxFunc);
}


float Animator::get(float time, Animator::ApproxFunc approx)
{
    
    if (!m_locked)
    {
        assert(m_timeline.size() >= 2);
        assert(m_timeline.top().first < time);

        m_p2 = m_timeline.top();
        m_locked = true;
    }

    while (!m_timeline.empty() && m_p2.first < time)
    {
        m_timeline.pop();
        m_p1 = m_p2;
        m_p2 = m_timeline.top();
    }

    if (m_timeline.empty())
    {
        m_p1 = m_p2;
    }

    float result = approx(m_p1.first, m_p1.second, m_p2.first, m_p2.second, time);

    return result;
}
