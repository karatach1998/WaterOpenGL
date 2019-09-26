#pragma once


// C++
#include <queue>
#include <exception>


class Animator
{
    using TimelinePoint = std::pair<float, float>;
    using ApproxFunc = float(*)(float, float, float, float, float);

public:
    struct timeline_locked : std::exception
    {
        timeline_locked(float time, float value);
    };

public:
    Animator();

    void addTimelinePoint(float time, float value);
    float get(float time);
    float get(float time, ApproxFunc);
    void setDefaultApproxFunc(ApproxFunc f);

    static float linearApprox(float x1, float y1, float x2, float y2, float x);
    static float trigonometricApprox(float x1, float y1, float x2, float y2, float x);
    
private:
    static bool fComp(const TimelinePoint& a, const TimelinePoint& b);

private:
    ApproxFunc m_defApproxFunc = linearApprox;

    std::priority_queue<TimelinePoint, std::vector<TimelinePoint>, decltype(&fComp)> m_timeline;
    TimelinePoint m_p1;
    TimelinePoint m_p2;
    bool m_locked = false;
};
