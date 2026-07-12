#include <iostream>
#include <SFML/Graphics.hpp>
#include "src/Particle.h"
#include "src/Simulation.h"
#include <vector>
#include <random>
#include <string>
#include <iomanip>

// Temporary random particle generator
Particle CreateRandomParticle(int particleNumber, double worldWidth, double worldHeight)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<double> xDist(50.0, worldWidth - 50.0);
    std::uniform_real_distribution<double> yDist(50.0, worldHeight - 50.0);
    std::uniform_real_distribution<double> vDist(-80.0, 80.0);
    std::uniform_int_distribution<int> chargeDist(0, 1);

    Particle p;

    p.SetName("Particle " + std::to_string(particleNumber));
    p.SetMass(10.0);
    p.SetRadius(15.0);

    if (chargeDist(gen) == 0)
    {
        p.SetCharge(-1.0);
    }
    else
    {
        p.SetCharge(1.0);
    }

    p.SetPosition(xDist(gen), yDist(gen));
    p.SetVelocity(vDist(gen), vDist(gen));

    return p;
}

int main()
{
    std::cout << "ChargeLab Starting..." << std::endl;
    // Recreating main but with the Simulation instead of making individual particles

    // Create window
    sf::RenderWindow window(sf::VideoMode({900, 700}), "ChargeLab");
    window.setFramerateLimit(60);

    // Make a simulation object
    Simulation simulation(900.0, 700.0);
    bool isDraggingSelected = false;

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

    // Debug print the max particles allowed in simulation
    std::cout << "Number of particles in simulation: ";
    std::cout << simulation.GetParticleCount() << "/" << simulation.GetMaxParticles() << std::endl;

    // Main loop
    while (window.isOpen())
    {
        double dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {

                // Space bar: Pause and unpause the simulation
                if (keyPressed->code == sf::Keyboard::Key::Space)
                {
                    simulation.TogglePaused();
                }

                // C: clear the particles
                if (keyPressed->code == sf::Keyboard::Key::C)
                {
                    simulation.ClearParticles();
                    std::cout << "Cleared all particles" << std::endl;
                }

                // A: add particle
                if (keyPressed->code == sf::Keyboard::Key::A)
                {
                    // particle number plus 1 bc 0-indexed
                    int nextParticleNumber = simulation.GetParticleCount() + 1;

                    Particle newParticle = CreateRandomParticle(nextParticleNumber, 900.0, 700.0);

                    if (simulation.AddParticle(newParticle))
                    {
                        std::cout << "Added particle. Count: "
                                  << simulation.GetParticleCount()
                                  << "/"
                                  << simulation.GetMaxParticles()
                                  << std::endl;
                    }
                    else
                    {
                        std::cout << "Cannot add particle. Simulation full: "
                                  << simulation.GetParticleCount()
                                  << "/"
                                  << simulation.GetMaxParticles()
                                  << std::endl;
                    }
                }

                // Num1: if the number 1 is pressed, add a positive particle
                if (keyPressed->code == sf::Keyboard::Key::Num1)
                {
                    // The mouse has to be in the window
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (simulation.AddParticleAt(mousePos.x, mousePos.y, 1.0))
                    {
                        std::cout << "Added positive particle: "
                                  << simulation.GetParticleCount()
                                  << "/"
                                  << simulation.GetMaxParticles()
                                  << std::endl;
                    }
                    else
                    {
                        std::cout << "Simulation full." << std::endl;
                    }
                }
                
                // Num2: If the number 2 is pressed, add a negative particle
                if (keyPressed->code == sf::Keyboard::Key::Num2)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (simulation.AddParticleAt(mousePos.x, mousePos.y, -1.0))
                    {
                        std::cout << "Added negative particle: "
                                    << simulation.GetParticleCount()
                                    << "/"
                                    << simulation.GetMaxParticles()
                                    << std::endl;
                    }
                    else
                    {
                        std::cout << "Simulation full." << std::endl;
                    }
                }
            }
            // Clicking functions
            // Left-click: if the left mouse button is pressed, select a particle
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePressed->button == sf::Mouse::Button::Left) {
                    // Make sure the mouse is within the SFML window
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                    // If the mouse is in the simulation window AND user clicked within the particle
                    if (simulation.SelectedParticleAt(mousePosition.x, mousePosition.y)) {

                        isDraggingSelected = true;

                        int selectedIndex = simulation.GetSelectedParticleIndex();
                        Vector2D totalForce = simulation.GetTotalForceOnParticle(
                            selectedIndex);
                        Vector2D totalForceSI = simulation.GetTotalForceOnParticleSI(
                            selectedIndex);

                        std::cout << "Selected particle index: "
                                << selectedIndex
                                << std::endl;

                        std::cout << "Total force (simulation units): ";
                        totalForce.Print();

                        std::cout << std::scientific << std::setprecision(6);
                        std::cout << "Total force (SI, N): ("
                                  << totalForceSI.GetX() << ", "
                                  << totalForceSI.GetY() << ")"
                                  << std::endl;
                        std::cout << std::defaultfloat;
                    }

                    else {
                        isDraggingSelected = false;
                        simulation.ClearSelectedParticle();
                        std::cout << "No particle selected." << std::endl;
                    }
                }
            }

            // Mouse Dragging
            if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseReleased->button == sf::Mouse::Button::Left) {
                    isDraggingSelected = false;
                }
            }
        }

        if (isDraggingSelected && simulation.HasSelectedParticle()) {
            sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

            simulation.MoveSelected(mouseWorld.x, mouseWorld.y);
            simulation.SetSelectedVelocity(0.0, 0.0);
        }

        // Update the simulation
        simulation.Update(dt);

        window.clear(sf::Color::Black);
        simulation.Draw(window);
        window.display();
    }

    std::cout << "Program ended." << std::endl;
    return 0;
}