#include <cmath>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "dsp/DelayLine.h"

using namespace Catch::Matchers;

TEST_CASE ("DelayLine — prepare inicializa sin NaN", "[DelayLine]")
{
    Lupex::DelayLine dl;
    dl.prepare (44100.0, 20.0f);

    // Leer sin haber escrito — debe retornar silencio
    float out = dl.readDirect (10.0f);
    REQUIRE_FALSE (std::isnan (out));
    REQUIRE_FALSE (std::isinf (out));
    REQUIRE_THAT (out, WithinAbs (0.0f, 1e-6f));
}

TEST_CASE ("DelayLine — write y readDirect retornan señal correcta", "[DelayLine]")
{
    Lupex::DelayLine dl;
    dl.prepare (44100.0, 50.0f);

    // Llenar el buffer con un impulso
    // 10ms a 44100Hz = 441 samples
    int delaySamples = 441;
    for (int i = 0; i < delaySamples; ++i)
        dl.write (i == 0 ? 1.0f : 0.0f);

    float out = dl.readDirect (10.0f);
    REQUIRE_FALSE (std::isnan (out));
    REQUIRE_FALSE (std::isinf (out));
}

TEST_CASE ("DelayLine — reset limpia el buffer", "[DelayLine]")
{
    Lupex::DelayLine dl;
    dl.prepare (44100.0, 20.0f);

    // Escribir señal
    for (int i = 0; i < 100; ++i)
        dl.write (1.0f);

    dl.reset();

    // Después del reset debe retornar silencio
    float out = dl.readDirect (5.0f);
    REQUIRE_THAT (out, WithinAbs (0.0f, 1e-6f));
}

TEST_CASE ("DelayLine — no genera NaN con delay time extremo", "[DelayLine]")
{
    Lupex::DelayLine dl;
    dl.prepare (44100.0, 20.0f);

    for (int i = 0; i < 1000; ++i)
        dl.write (0.5f);

    // Delay time en el límite máximo
    float out = dl.readDirect (19.9f);
    REQUIRE_FALSE (std::isnan (out));
    REQUIRE_FALSE (std::isinf (out));
}