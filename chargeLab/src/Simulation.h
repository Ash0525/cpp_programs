#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Particle.h"
#include "Vector2D.h"

class Simulation {
    public: 
        // Define constructor
        Simulation();
        Simulation(double width, double height);

        // ADD PARTICLES
        bool AddParticle(const Particle& newParticle);
        void ClearParticles();

        // ESSENTIAL SIMULATION FUNCTIONS
        void Update(double dt);
        void Draw(sf::RenderWindow& window) const;
        void PrintParticles() const;

        // Add pause toggle for window
        void TogglePaused();
        void SetPaused(bool newPaused);
        bool IsPaused() const;

        // HANDLE PARTICLE COUNT
        int GetParticleCount() const;
        int GetMaxParticles() const;

        void SetMaxParticles(int newMaxParticles);
        void IncreaseMaxParticles(int amount);
        void DecreaseMaxParticles(int amount);
        bool IsFull() const;

        bool AddParticleAt(double x, double y, double charge);
        bool AddParticleAt(double x, double y, double charge, double mass, double radius);

        // Select particles
        bool SelectedParticleAt(double x, double y);
        void ClearSelectedParticle();
        bool HasSelectedParticle() const;
        int GetSelectedParticleIndex() const;
        
        
    private:
        // private functions
        // Add the forces using sum of the forces
        void ComputeForces(std::vector<double>& fx, std::vector<double>& fy) const;

        // Apply boundary so particle does not go off the map
        void HandleBoundaries(Particle& particle);

        // Handle particle collisions
        void HandleParticleCollisions();

        // Coulomb's Force
        void ApplyCoulombForces(double dt);

        // Get the delta distances
        Vector2D GetDeltaDistances(int particle1, int particle2) const;

        // Selected particle index lets the program know if a particle was selected
        int selectedParticleIndex;

        // vector list of particles
        std::vector<Particle> particles;
        double charge = 1.6e-19;
        double coulombConstant = 8.9875517923e9;
        double dummyCoulomb = 500000.0;

        // Prevent huge forces at tiny distances.
        double minDistanceClamp;

        // width and height to set boundaries for box
        double width;
        double height;
        bool paused;
        int maxParticles;
};