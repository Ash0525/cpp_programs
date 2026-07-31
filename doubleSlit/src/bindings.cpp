// Bridge between C++ and JavaScript

#include "DoubleSlitEngine.hpp"
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(doubleslit_bindings)
{
    emscripten::register_vector<double>("DoubleVector");

    emscripten::value_object<doubleslit::SimulationData>("SimulationData")
        .field("positions", &doubleslit::SimulationData::positions)
        .field("intensities", &doubleslit::SimulationData::intensities);

    emscripten::class_<doubleslit::DoubleSlitEngine>("DoubleSlitEngine")
        .constructor<>()
        .function("status", &doubleslit::DoubleSlitEngine::status)
        .function(
            "generatePattern",
            &doubleslit::DoubleSlitEngine::generatePattern
        )
        .function(
            "setDiffractionEnabled",
            &doubleslit::DoubleSlitEngine::setDiffractionEnabled
        )
        .function(
            "diffractionEnabled",
            &doubleslit::DoubleSlitEngine::diffractionEnabled
        )
        .function(
            "setWavelength",
            &doubleslit::DoubleSlitEngine::setWavelength
        )
        .function(
            "wavelength",
            &doubleslit::DoubleSlitEngine::wavelength
        )
        .function(
            "setSlitSeparation",
            &doubleslit::DoubleSlitEngine::setSlitSeparation
        )
        .function(
            "slitSeparation",
            &doubleslit::DoubleSlitEngine::slitSeparation
        )
        .function(
            "setSlitWidth",
            &doubleslit::DoubleSlitEngine::setSlitWidth
        )
        .function(
            "slitWidth",
            &doubleslit::DoubleSlitEngine::slitWidth
        )
        .function(
            "setScreenDistance",
            &doubleslit::DoubleSlitEngine::setScreenDistance
        )
        .function(
            "screenDistance",
            &doubleslit::DoubleSlitEngine::screenDistance
        );
}
