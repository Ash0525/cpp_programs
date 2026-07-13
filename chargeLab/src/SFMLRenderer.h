#pragma once

#include <SFML/Graphics.hpp>
#include "Simulation.h"

class SFMLRenderer {
    public:
        void Draw(sf::RenderWindow& window, const Simulation& simulation) const;
};