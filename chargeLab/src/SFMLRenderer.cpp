#include "SFMLRenderer.h"

void SFMLRenderer::Draw(sf::RenderWindow& window, const Simulation& simulation) const {

    // Draw renderer is dependent on the simulation as well
    for (size_t i = 0; i < simulation.GetParticleCount(); i++) {

        // get the radius
        double radius = simulation.GetParticleRadius(i);

        // initialize a circle object
        sf::CircleShape circle (static_cast<float>(radius));

        // Size of circle
        circle.setOrigin({
            static_cast<float>(radius),
            static_cast<float>(radius)
        });

        // Set circle position (if given)
        circle.setPosition({
            static_cast<float>(simulation.GetParticleX(i)),
            static_cast<float>(simulation.GetParticleY(i))
        });

        double charge = simulation.GetParticleCharge(i);

        if (charge > 0.0) {
            circle.setFillColor(sf::Color::Blue);
        } else if (charge < 0.0) {
            circle.setFillColor(sf::Color::Red);
        } else {
            circle.setFillColor(sf::Color::White);
        }

        if (simulation.HasSelectedParticle() && i == simulation.GetSelectedParticleIndex()) {
            circle.setOutlineThickness(4.0f);
            circle.setOutlineColor(sf::Color::Yellow);
        }

        window.draw(circle);
    }
}

void SFMLRenderer::DrawForces(sf::RenderWindow&, const Simulation& simulation) const {

}

void SFMLRenderer::DrawForceArrow(
            sf::RenderWindow& window,
            const Vector2D& startPosition,
            const Vector2D& force,
            double maxForceMagnitude,
            sf::Color color
        ) const {

}
