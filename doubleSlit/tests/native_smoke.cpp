#include "DoubleSlitEngine.hpp"
#include <iostream>

int main() {
    doubleslit::DoubleSlitEngine engine;
    std::cout << engine.status() << '\n';

    return 0;
}