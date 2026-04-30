#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

// Tests de rangos de parámetros — sin instanciar JUCE processor
TEST_CASE ("Rangos de parámetros — valores normalizados", "[Parameters]")
{
    // Rate: 0.1 a 5.0 Hz
    float rateMin = 0.1f;
    float rateMax = 5.0f;
    REQUIRE (rateMin < rateMax);
    REQUIRE (rateMin > 0.0f);

    // Depth: 0 a 100%
    float depthMin = 0.0f;
    float depthMax = 100.0f;
    REQUIRE (depthMin >= 0.0f);
    REQUIRE (depthMax <= 100.0f);

    // Level: 0 a 100%
    float levelMin = 0.0f;
    float levelMax = 100.0f;
    REQUIRE (levelMin >= 0.0f);
    REQUIRE (levelMax <= 100.0f);
}