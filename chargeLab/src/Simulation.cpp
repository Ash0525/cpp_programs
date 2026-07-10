#include <vector>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "Simulation.h"

Simulation::Simulation() {

};

void Simulation::AddParticle(const Particle& newParticle) {
    
    // Push a new particle object into the vector list particles
    particles.push_back(newParticle);
};

// Update function will update the physics of the simulation
void Simulation::Update(double dt) {
    for (Particle& particle : particles) {
        // For now, just move the particle based on velocity
        particle.Move(dt);
    }
};

// Draw simulation particles
void Simulation::Draw(sf::RenderWindow& window) const {
    for (const Particle& particle : particles) {
        double radius = particle.GetRadius();

        // Declare circle object for every particle
        sf::CircleShape circle(static_cast<float>(radius));
        circle.setOrigin({
            static_cast<float>(radius),
            static_cast<float>(radius)
        });

        // Set the position of the each particle
        circle.setPosition({

            // get the X and the Y from each particle
            static_cast<float>(particle.GetXPos()),
            static_cast<float>(particle.GetYPos())
        });

        // If the particle's charge is positive, then make it blue
        if (particle.GetCharge() > 0) {
            circle.setFillColor(sf::Color::Cyan);
        
        // If the particle's charge is negative, then make it red
        } else if (particle.GetCharge() < 0) {
            circle.setFillColor(sf::Color::Red);

        // If the particle has no charge, then make it white
        } else {
            circle.setFillColor(sf::Color::White);
        }

        window.draw(circle);
    }
};

// Print all particles to terminal
void Simulation::PrintParticles() const {
    for (const Particle& particle : particles) {
        particle.Print();
        std::cout << std::endl;
    }
}