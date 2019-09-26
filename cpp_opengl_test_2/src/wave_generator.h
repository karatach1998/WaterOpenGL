#pragma once


// C+==
#include <random>
#include <random>
#include <queue>
#include <thread>
#include <mutex>

// Local
#include "wave.h"


class WaveGenerator
{
public:
    WaveGenerator();

    auto waveCount() const
        { return m_waves.size(); }
    const Wave& get(int i) const;

    void generatePermanentWave();

    friend void generate_permanent_wave(WaveGenerator* ptr);

private:
    static constexpr auto m_waveLifetime = 15.0f;
    static constexpr auto m_permanentWaveTotalCount = 3;
    static std::minstd_rand m_rand;

    std::vector<Wave> m_waves;
    unsigned m_currentPermanentWaveIndex = 0;

    std::mutex m_mtx;
    std::thread m_permanentWaveGenratorThread;
};


void generate_permanent_wave(WaveGenerator* ptr);
