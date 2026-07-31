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

    std::cout << engine.status() << '\n';

    return 0;
}
