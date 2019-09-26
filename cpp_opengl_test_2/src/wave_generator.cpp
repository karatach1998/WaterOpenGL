
// C++
#include <chrono>

// Local
#include "wave_generator.h"


std::minstd_rand WaveGenerator::m_rand;


WaveGenerator::WaveGenerator()
    : m_permanentWaveGenratorThread(generate_permanent_wave, this)
{
}


const Wave& WaveGenerator::get(int i) const
{
    return m_waves.at(i);
}


void WaveGenerator::generatePermanentWave()
{
    std::lock_guard<std::mutex> g(m_mtx);
    auto dir_animator = std::make_shared<Animator>();

    float current_time = static_cast<float>(glfwGetTime());

    dir_animator->setDefaultApproxFunc(Animator::trigonometricApprox);
    dir_animator->addTimelinePoint(current_time, 0.0f);
    for (auto t : {1.0f, 14.0f})
    {
        t += current_time;
        float angle = static_cast<float>(m_rand() % 90 - 45);
        float rad = glm::radians(angle);
        dir_animator->addTimelinePoint(t, rad);
    }

    Wave w(0.5f / 3, 0.05f, 0.01f, 0.04f, glm::vec2(0.3f, 0.7f));
    w.setDirectionAnimator(dir_animator);
    
    if (m_waves.size() < m_permanentWaveTotalCount)
        m_waves.emplace_back(w);
    else
    {
        m_currentPermanentWaveIndex %= m_permanentWaveTotalCount;
        m_waves[m_currentPermanentWaveIndex] = w;
    }
}


void generate_permanent_wave(WaveGenerator* wg)
{
    while (true)
    {
        wg->generatePermanentWave();
        std::this_thread::sleep_for(std::chrono::duration<float>(WaveGenerator::m_waveLifetime / 3.0f));
    }
}
