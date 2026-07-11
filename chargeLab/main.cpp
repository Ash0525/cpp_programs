#include <iostream>
#include <SFML/Graphics.hpp>
#include "src/Particle.h"
#include "src/Simulation.h"
#include <vector>

int main() {
    std::cout << "ChargeLab Starting..." << std::endl;
    // Recreating main but with the Simulation instead of making individual particles

    // Create window
    sf::RenderWindow window(sf::VideoMode({900, 700}), "ChargeLab");
    window.setFramerateLimit(60);

    // Make a simulation object
    Simulation simulation(900.0, 700.0);

    // Start clock
    sf::Clock clock;

    // Declare particles
    Particle p1;
    p1.SetName("Positive");
    p1.SetMass(10.0);
    p1.SetCharge(1.0);
    p1.SetRadius(20.0);
    p1.SetPosition(250.0, 150.0);
    p1.SetVelocity(0.0, 0.0);

    Particle p2;
    p2.SetName("Negative");
    p2.SetMass(10.0);
    p2.SetCharge(-1.0);
    p2.SetRadius(20.0);
    p2.SetPosition(350.0, 250.0);
    p2.SetVelocity(0.0, 0.0);

    Particle p3;
    p3.SetName("SomeRandomDude");
    p3.SetMass(50.0);
    p3.SetCharge(+4.0);
    p3.SetRadius(20.0);
    p3.SetPosition(370.0, 250.0);
    p3.SetVelocity(-100.0, 0.0);


    // Add the particles to the simulation
    simulation.AddParticle(p1);
    simulation.AddParticle(p2);
    simulation.AddParticle(p3);

    // Print the particles that were made
    simulation.PrintParticles();
    
    // Main loop
    while (window.isOpen()) {
        double dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        simulation.Update(dt);

        window.clear(sf::Color::Black);
        simulation.Draw(window);
        window.display();
    }
    
    std::cout << "Program ended." << std::endl;
    return 0;
}