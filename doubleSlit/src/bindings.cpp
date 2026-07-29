// Bridge between C++ and JavaScript

#include "DoubleSlitEngine.hpp"
#include <emscripten/bind.h>

EMSCRIPTEN_BINDINGS(doubleslit_bindings)
{
    emscripten::class_<doubleslit::DoubleSlitEngine>("DoubleSlitEngine")
        .constructor<>()
        .function("status", &doubleslit::DoubleSlitEngine::status);
}