#include <vector>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "Simulation.h"

// Default simulation size
Simulation::Simulation() {
    width = 900.0;
    height = 700.0;
};

// Custom size for simulation
Simulation::Simulation(double width, double height) {

    // update the width and height in private
    this->width = width;
    this->height = height;
}

void Simulation::AddParticle(const Particle& newParticle) {
    
    // Push a new particle object into the vector list particles
    particles.push_back(newParticle);
};

// Update function will update the physics of the simulation
void Simulation::Update(double dt) {
    for (Particle& particle : particles) {
        // Move the particles
        particle.Move(dt);

        // When the particles hit the wall, HandleBoundaries
        HandleBoundaries(particle);
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

// Boundaries ensure that the particles do not move off the screen
void Simulation::HandleBoundaries(Particle& particle) {
    double x = particle.GetXPos();
    double y = particle.GetYPos();

    double vx = particle.GetVX();
    double vy = particle.GetVY();

    double r = particle.GetRadius();

    // Left wall. If the x position of the particle minus radius is less than 0 (left limit of width)
    if (x - r < 0.0) {
        // reverse the direction of vx
        particle.SetPosition(r, y);
        particle.SetVelocity(-vx, vy);
    }

    // Right wall
    if (x + r > width) {
        particle.SetPosition(width - r, y);
        particle.SetVelocity(-vx, vy);
    }

    // Top wall
    if (y - r < 0.0) {
        // reverse the direction of vx
        particle.SetPosition(x, r);
        particle.SetVelocity(vx, -vy);
    }

    // Right wall
    if (y + r > height) {
        particle.SetPosition(x, height - r);
        particle.SetVelocity(-vx, -vy);
    }
}