#pragma once
#include <iostream>
#include "Vector2D.h"
#include <string>

class Particle {
    public: 
        // Elementary charge constant (Coulombs)
        static constexpr double ELEMENTARY_CHARGE = 1.6e-19;

        Particle();
        
        // Parameterized constructor
        Particle(const std::string& newName, double newMass, double newCharge, 
                 double newRadius, double newXPos, double newYPos, 
                 double newVX = 0.0, double newVY = 0.0);

        // Static factory methods for creating standard particles
        static Particle CreateElectron(double xPos, double yPos, double vx = 0.0, double vy = 0.0);
        static Particle CreateProton(double xPos, double yPos, double vx = 0.0, double vy = 0.0);

        // Setter/Mutator Methods/Functions

        // set particle name
        void SetName(const std::string& newName);

        // set mass
        void SetMass(double newMass);

        // set charge
        void SetCharge(double newCharge);

        // set radius
        void SetRadius(double newRadius);

        // set position
        void SetPosition(double newXPos, double newYPos);

        // set velocity
        void SetVelocity(double newVX, double newVY);

        // Getters/Accessor Methods/Functions
        std::string GetName() const;
        double GetMass() const;
        double GetCharge() const;
        double GetRadius() const; 
        double GetXPos() const;
        double GetYPos() const;
        double GetVX() const;
        double GetVY() const;

        // get the distance between two particles
        double DistanceTo(const Particle& otherParticle) const;
        void ApplyForce(double fx, double fy, double dt);
        void Move(double dt);
        void Print() const;

    private:
        std::string name;
        double mass;
        double charge;
        double radius;

        double xPos;
        double yPos;

        double xVelocity;
        double yVelocity;
};