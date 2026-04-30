#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "dsp/ChorusEngine.h"

using namespace Catch::Matchers;

static constexpr double kSampleRate  = 44100.0;
static constexpr int    kBlockSize   = 512;

TEST_CASE ("ChorusEngine — no genera NaN con señal de guitarra simulada", "[ChorusEngine]")
{
    Chorus::ChorusEngine engine;
    engine.prepare (kSampleRate, kBlockSize);

    std::vector<float> bufL (kBlockSize, 0.0f);
    std::vector<float> bufR (kBlockSize, 0.0f);

    // Simular señal de guitarra — senoide a 440Hz
    for (int i = 0; i < kBlockSize; ++i)
    {
        bufL[i] = 0.5f * std::sin (2.0f * 3.14159f * 440.0f * i / (float)kSampleRate);
        bufR[i] = bufL[i];
    }

    engine.process (bufL.data(), bufR.data(), kBlockSize,
                    1.0f,   // rate
                    0.5f,   // depth
                    0.5f,   // level
                    0.5f,   // low
                    0.5f);  // high

    for (int i = 0; i < kBlockSize; ++i)
    {
        REQUIRE_FALSE (std::isnan (bufL[i]));
        REQUIRE_FALSE (std::isnan (bufR[i]));
        REQUIRE_FALSE (std::isinf (bufL[i]));
        REQUIRE_FALSE (std::isinf (bufR[i]));
    }
}

TEST_CASE ("ChorusEngine — bypass produce señal no silenciosa", "[ChorusEngine]")
{
    Chorus::ChorusEngine engine;
    engine.prepare (kSampleRate, kBlockSize);

    std::vector<float> bufL (kBlockSize, 0.5f);
    std::vector<float> bufR (kBlockSize, 0.5f);

    engine.process (bufL.data(), bufR.data(), kBlockSize,
                    1.0f, 0.0f, 0.0f, 0.5f, 0.5f);

    // Con level=0 y depth=0 el canal L debe ser solo dry
    REQUIRE_THAT (bufL[0], WithinAbs (0.5f, 1e-3f));
}

TEST_CASE ("ChorusEngine — reset limpia sin crashes", "[ChorusEngine]")
{
    Chorus::ChorusEngine engine;
    engine.prepare (kSampleRate, kBlockSize);

    std::vector<float> bufL (kBlockSize, 0.3f);
    std::vector<float> bufR (kBlockSize, 0.3f);

    engine.process (bufL.data(), bufR.data(), kBlockSize,
                    2.0f, 0.8f, 0.7f, 0.5f, 0.5f);

    engine.reset();

    // Después del reset — silencio no genera NaN
    std::fill (bufL.begin(), bufL.end(), 0.0f);
    std::fill (bufR.begin(), bufR.end(), 0.0f);

    engine.process (bufL.data(), bufR.data(), kBlockSize,
                    1.0f, 0.5f, 0.5f, 0.5f, 0.5f);

    for (int i = 0; i < kBlockSize; ++i)
    {
        REQUIRE_FALSE (std::isnan (bufL[i]));
        REQUIRE_FALSE (std::isnan (bufR[i]));
    }
}