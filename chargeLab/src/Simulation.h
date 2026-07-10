#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Particle.h"

class Simulation {
    public: 
        // Define constructor
        Simulation();
        Simulation(double width, double height);

        // Add a particle to the vector particles
        void AddParticle(const Particle& newParticle);
        void ClearParticles();
        void Update(double dt);
        void Draw(sf::RenderWindow& window) const;
        void PrintParticles() const;
        void HandleBoundaries(Particle& particle);
        
    private:
        // private functions
        // Add the forces using sum of the forces
        void ComputeForces(std::vector<double>& fx, std::vector<double>& fy) const;

        // Apply boundary so particle does not go off the map
        void ApplyBoundary(Particle& p) const;

        // vector list of particles
        std::vector<Particle> particles;
        double charge = 1.6e-19;
        double coulombConstant = 8.9875517923e9;

        // Prevent huge forces at tiny distances.
        double minDistanceClamp;

        // width and height to set boundaries for box
        double width;
        double height;
};