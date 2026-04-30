#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "dsp/BBDFilter.h"

using namespace Catch::Matchers;

TEST_CASE ("BBDFilter — silencio entra, silencio sale", "[BBDFilter]")
{
    Lupex::BBDFilter f;
    f.prepare (44100.0);
    f.setTone (1.0f);

    for (int i = 0; i < 100; ++i)
    {
        float out = f.process (0.0f);
        REQUIRE_THAT (out, WithinAbs (0.0f, 1e-6f));
    }
}

TEST_CASE ("BBDFilter — no genera NaN con señal de impulso", "[BBDFilter]")
{
    Lupex::BBDFilter f;
    f.prepare (44100.0);
    f.setTone (0.5f);

    float out = f.process (1.0f);
    REQUIRE_FALSE (std::isnan (out));
    REQUIRE_FALSE (std::isinf (out));

    for (int i = 0; i < 100; ++i)
    {
        out = f.process (0.0f);
        REQUIRE_FALSE (std::isnan (out));
    }
}

TEST_CASE ("BBDFilter — reset limpia estado interno", "[BBDFilter]")
{
    Lupex::BBDFilter f;
    f.prepare (44100.0);
    f.setTone (1.0f);

    for (int i = 0; i < 50; ++i)
        f.process (1.0f);

    f.reset();
    float out = f.process (0.0f);
    REQUIRE_THAT (out, WithinAbs (0.0f, 1e-6f));
}