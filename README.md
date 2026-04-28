# Lupex Chorus — Analog BBD Chorus

![Lupex Chorus Plugin](assets/images/screenshot.png)

Lupex Chorus is an analog BBD-style stereo chorus VST3 plugin by **Fauz Audio**, built with JUCE and CMake.  
Inspired by the character of classic analog chorus pedals like the MXR M234 Analog Chorus.

---

## Features

- Dual BBD-style analog chorus chains — true stereo with independent L/R processing
- Quadrature LFO modulation (90° phase offset) for wide stereo image
- Low and High shelf tone controls based on MXR M234 specifications
- MXR-style stereo routing — dry + wet on L, wet-only on R for natural depth
- Rate smoothing for organic pitch glide when changing speed in real time
- Bypass footswitch with LED indicator

## Parameters

| Parameter | Range | Description |
|---|---|---|
| Low | 0–100% | Low shelf cutoff (70Hz–800Hz, 12dB/oct) — controls wet low end |
| High | 0–100% | High shelf cutoff (660Hz–flat, 6dB/oct) — controls wet brightness |
| Level | 0–100% | Wet signal volume — dry always at 100% |
| Rate | 0.1–5.0 Hz | LFO speed |
| Depth | 0–100% | LFO modulation depth |
| Bypass | On/Off | Hard bypass |

## Build

```bash
git clone --recursive https://github.com/maurocosentino/lupex-chorus-bbd
cd lupex-chorus-bbd
cmake -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release
```

## Requirements

- JUCE 7+
- CMake 3.24+
- C++20
- Linux / Windows / macOS

## License

MIT