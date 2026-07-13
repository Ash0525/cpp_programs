#include "SFMLRenderer.h"
#include <cmath>
#include <algorithm>

void SFMLRenderer::Draw(sf::RenderWindow &window, const Simulation &simulation) const
{

    // Draw renderer is dependent on the simulation as well
    for (size_t i = 0; i < simulation.GetParticleCount(); i++)
    {

        // get the radius
        double radius = simulation.GetParticleRadius(i);

        // initialize a circle object
        sf::CircleShape circle(static_cast<float>(radius));

        // Size of circle
        circle.setOrigin({static_cast<float>(radius),
                          static_cast<float>(radius)});

        // Set circle position (if given)
        circle.setPosition({static_cast<float>(simulation.GetParticleX(i)),
                            static_cast<float>(simulation.GetParticleY(i))});

        double charge = simulation.GetParticleCharge(i);

        if (charge > 0.0)
        {
            circle.setFillColor(sf::Color::Blue);
        }
        else if (charge < 0.0)
        {
            circle.setFillColor(sf::Color::Red);
        }
        else
        {
            circle.setFillColor(sf::Color::White);
        }

        if (simulation.HasSelectedParticle() && i == simulation.GetSelectedParticleIndex())
        {
            circle.setOutlineThickness(4.0f);
            circle.setOutlineColor(sf::Color::Yellow);
        }

        window.draw(circle);
    }

    DrawForces(window, simulation);
}

void SFMLRenderer::DrawForces(sf::RenderWindow& window, const Simulation &simulation) const
{
    if (!simulation.HasSelectedParticle()) {
        return;
    }

    int selectedIndex = simulation.GetSelectedParticleIndex();

    if (selectedIndex < 0 || selectedIndex >= simulation.GetParticleCount()) {
        return;
    }

    Vector2D selectedPosition(
        simulation.GetParticleX(selectedIndex),
        simulation.GetParticleY(selectedIndex)
    );

    double maxForceMagnitude = 0.0;

    for (int i = 0; i < simulation.GetParticleCount(); ++i) {
        if (i == selectedIndex) {
            continue;
        }

        Vector2D force = simulation.GetForceBetweenParticles(selectedIndex, i);
        double magnitude = force.Magnitude();

        if (magnitude > maxForceMagnitude) {
            maxForceMagnitude = magnitude;
        }
    }

    Vector2D totalForce = simulation.GetTotalForceOnParticle(selectedIndex);

    if (totalForce.Magnitude() > maxForceMagnitude) {
        maxForceMagnitude = totalForce.Magnitude();
    }

    for (int i = 0; i < simulation.GetParticleCount(); ++i) {
        if (i == selectedIndex) {
            continue;
        }

        Vector2D force = simulation.GetForceBetweenParticles(selectedIndex, i);

        DrawForceArrow(
            window,
            selectedPosition,
            force,
            maxForceMagnitude,
            sf::Color::Yellow
        );
    }

    DrawForceArrow(
        window,
        selectedPosition,
        totalForce,
        maxForceMagnitude,
        sf::Color::White
    );
}

void SFMLRenderer::DrawForceArrow(
    sf::RenderWindow &window,
    const Vector2D &startPosition,
    const Vector2D &force,
    double maxForceMagnitude,
    sf::Color color) const
{
    double forceMagnitude = force.Magnitude();

    if (forceMagnitude <= 0.0 || maxForceMagnitude <= 0.0) {
        return;
    }

    Vector2D direction = force.Normalized();

    const double minArrowLength = 20.0;
    const double maxArrowLength = 120.0;

    double forceRatio = forceMagnitude / maxForceMagnitude;

    double arrowLength = minArrowLength +
        forceRatio * (maxArrowLength - minArrowLength);

    Vector2D arrowVector = direction * arrowLength;
    Vector2D endPosition = startPosition + arrowVector;

    double angleRadians = std::atan2(arrowVector.GetY(), arrowVector.GetX());
    double angleDegrees = angleRadians * 180.0 / 3.14159265358979323846;

    sf::RectangleShape shaft;
    shaft.setSize({
        static_cast<float>(arrowLength),
        3.0f
    });

    shaft.setOrigin({ 0.0f, 1.5f });

    shaft.setPosition({
        static_cast<float>(startPosition.GetX()),
        static_cast<float>(startPosition.GetY())
    });

    shaft.setRotation(sf::degrees(static_cast<float>(angleDegrees)));
    shaft.setFillColor(color);

    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, { 0.0f, 0.0f });
    arrowHead.setPoint(1, { -10.0f, -6.0f });
    arrowHead.setPoint(2, { -10.0f, 6.0f });

    arrowHead.setPosition({
        static_cast<float>(endPosition.GetX()),
        static_cast<float>(endPosition.GetY())
    });

    arrowHead.setRotation(sf::degrees(static_cast<float>(angleDegrees)));
    arrowHead.setFillColor(color);

    window.draw(shaft);
    window.draw(arrowHead);
}
