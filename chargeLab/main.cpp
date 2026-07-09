#include <iostream>
#include <SFML/Graphics.hpp>
#include "src/Particle.h"
#include <vector>

int main() {
    std::cout << "ChargeLab Starting..." << std::endl;
    
    // Create window
    sf::RenderWindow window(sf::VideoMode({900, 700}), "ChargeLab");
    window.setFramerateLimit(60);
    
    // Create particles
    std::vector<Particle> particles;
    
    // Particle 1
    Particle p1;
    p1.SetName("Electron 1");
    p1.SetMass(1.0);
    p1.SetCharge(-1.0);
    p1.SetRadius(8.0);
    p1.SetPosition(300.0, 350.0);
    p1.SetVelocity(0.0, 0.0);
    particles.push_back(p1);
    
    // Particle 2
    Particle p2;
    p2.SetName("Proton 1");
    p2.SetMass(1.0);
    p2.SetCharge(1.0);
    p2.SetRadius(5.0);
    p2.SetPosition(600.0, 350.0);
    p2.SetVelocity(0.0, 0.0);
    particles.push_back(p2);
    
    // Particle 3
    Particle p3;
    p3.SetName("Electron 2");
    p3.SetMass(1.0);
    p3.SetCharge(-1.0);
    p3.SetRadius(8.0);
    p3.SetPosition(450.0, 200.0);
    p3.SetVelocity(0.0, 0.0);
    particles.push_back(p3);
    
    // Show the stats of the particles that I initialized
    std::cout << "Created " << particles.size() << " particles" << std::endl;
    for (const auto& p : particles) {
        p.Print();
    }

    // Start clock
    sf::Clock clock;
    
    // Main loop
    while (window.isOpen()) {
        double dt = clock.restart().asSeconds();

        // Handle window events
        while (auto event = window.pollEvent()) {
            // if the window is closing
            if (event->is<sf::Event::Closed>()) {
                std::cout << "Closing window..." << std::endl;
                window.close();
            }
        }
        
        // Clear window
        window.clear(sf::Color::Black);
        
        // Apply forces to the particles
        for (auto& p : particles) {

            p.ApplyForce(1000.0, 0.0, dt);
            p.Move(dt);
        }

        // Draw particles
        for (const auto& p : particles) {

            // Get the circle radius
            sf::CircleShape circle(static_cast<float>(p.GetRadius()));
            circle.setOrigin({static_cast<float>(p.GetRadius()), static_cast<float>(p.GetRadius())});
            circle.setPosition({static_cast<float>(p.GetXPos()), static_cast<float>(p.GetYPos())});
            
            // Color based on charge
            if (p.GetCharge() < 0) {
                circle.setFillColor(sf::Color::Red);  // Negative charge = blue
            } else {
                circle.setFillColor(sf::Color::Cyan);   // Positive charge = red
            }
            
            window.draw(circle);
        }
        
        window.display();
    }
    
    std::cout << "Program ended." << std::endl;
    return 0;
}