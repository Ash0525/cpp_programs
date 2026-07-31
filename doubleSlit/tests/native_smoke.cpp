#include "DoubleSlitEngine.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
    doubleslit::DoubleSlitEngine engine;

    const auto data = engine.generatePattern(-0.01, 0.01, 5);

    assert(data.positions.size() == 5);
    assert(data.intensities.size() == 5);
    assert(std::abs(data.positions[0] - (-0.01)) < 1e-12);
    assert(std::abs(data.positions[2] - 0.0) < 1e-12);
    assert(std::abs(data.positions[4] - 0.01) < 1e-12);
    assert(std::abs(data.intensities[2] - 1.0) < 1e-12);

    // Diffraction is optional and disabled by default.
    assert(!engine.diffractionEnabled());

    const double offCenterPosition = 0.01;
    const double idealIntensity = engine.intensityAt(offCenterPosition);

    engine.setDiffractionEnabled(true);
    assert(engine.diffractionEnabled());

    // The diffraction envelope leaves the center unchanged but dims this
    // off-center point.
    assert(std::abs(engine.intensityAt(0.0) - 1.0) < 1e-12);
    const double diffractedIntensity =
        engine.intensityAt(offCenterPosition);
    assert(diffractedIntensity < idealIntensity);

    // Disabling diffraction restores the ideal interference result.
    engine.setDiffractionEnabled(false);
    assert(!engine.diffractionEnabled());
    assert(
        std::abs(engine.intensityAt(offCenterPosition) - idealIntensity) <
        1e-12
    );

    std::cout << engine.status() << '\n';

    return 0;
}
