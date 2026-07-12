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

        // Force analysis
        Vector2D GetForceBetweenParticles(int particle1, int particle2) const;
        Vector2D GetTotalForceOnParticle(int particleIndex) const;
        Vector2D GetTotalForceOnParticleSI(int particleIndex) const;

        // Draw particle forces
        void DrawForces(sf::RenderWindow& window) const;
        void DrawForceArrow(sf::RenderWindow& window, const Vector2D& startPosition, const Vector2D& force, sf::Color color, double referenceMagnitude) const;
        
        // Moving particles with mouse
        void MoveSelected(double x, double y);
        void SetSelectedVelocity(double vx, double vy);
        void SetSelectedCharge(double charge);
        void SetSelectedMass(double mass);
        void SetSelectedRadius(double radius);

        double GetSelectedCharge() const;
        double GetSelectedMass() const;
        double GetSelectedRadius() const;
        double GetSelectedX() const;
        double GetSelectedY() const;
        double GetSelectedVx() const;
        double GetSelectedVy() const;

        // Add Getters for renderer-simualtion split
        double GetParticleX(int index) const;
        double GetParticleY(int index) const;
        double GetParticleRadius(int index) const;
        double GetParticleCharge(int index) const;
        
    private:
        // private functions
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
        double coulombConstant;

        // Unit scales convert simulation quantities to SI for physically meaningful values.
        double metersPerPixel;
        double coulombsPerChargeUnit;
        double kilogramsPerMassUnit;

        // Prevent huge forces at tiny distances.
        double minDistanceClamp;

        // width and height to set boundaries for box
        double width;
        double height;
        bool paused;
        int maxParticles;
};