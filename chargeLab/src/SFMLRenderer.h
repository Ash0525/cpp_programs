#pragma once

#include <SFML/Graphics.hpp>
#include "Simulation.h"

class SFMLRenderer {
    public:
        void Draw(sf::RenderWindow& window, const Simulation& simulation) const;

    private:
        void DrawForces(sf::RenderWindow& window, const Simulation& simulation) const;

        void DrawForceArrow(
            sf::RenderWindow& window,
            const Vector2D& startPosition,
            const Vector2D& force,
            double maxForceMagnitude,
            sf::Color color
        ) const;
};