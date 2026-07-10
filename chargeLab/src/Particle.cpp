// Define Particle functions
#include "Particle.h"
#include <iostream>
#include <string>
#include <cmath>

// Defining the Constructor
Particle::Particle() {
    name = "";
    mass = 1.0;
    charge = 0.0;
    radius = 0.0;
    xPos = 0.0;
    yPos = 0.0;
    xVelocity = 0.0;
    yVelocity = 0.0;
}

// Parameterized Constructor
Particle::Particle(const std::string& newName, double newMass, double newCharge, 
                   double newRadius, double newXPos, double newYPos, 
                   double newVX, double newVY) {
    name = newName;
    mass = newMass;
    charge = newCharge;
    radius = newRadius;
    xPos = newXPos;
    yPos = newYPos;
    xVelocity = newVX;
    yVelocity = newVY;
}

// Static factory method to create an electron
Particle Particle::CreateElectron(double xPos, double yPos, double vx, double vy) {
    return Particle("Electron", 9.109e-31, -ELEMENTARY_CHARGE, 8.0, xPos, yPos, vx, vy);
}

// Static factory method to create a proton
Particle Particle::CreateProton(double xPos, double yPos, double vx, double vy) {
    return Particle("Proton", 1.673e-27, ELEMENTARY_CHARGE, 5.0, xPos, yPos, vx, vy);
}

// Setting the name
void Particle::SetName(const std::string& newName) {
    name = newName;
}

std::string Particle::GetName() const {
    return name;
}

// Setting the mass
void Particle::SetMass(double newMass) {
    if (newMass <= 0.0) {
        throw std::invalid_argument("Particle mass can't be negative, bro");
    }
    mass = newMass;
}

double Particle::GetMass() const {
    return mass;
}

// Setting the charge
void Particle::SetCharge(double newCharge) {
    charge = newCharge;
}

double Particle::GetCharge() const {
    return charge;
}

// Setting the radius.
void Particle::SetRadius(double newRadius) {
    if (newRadius < 0.0) {
        throw std::invalid_argument("Particle cannot have a negative radius, my dude");
    }
    radius = newRadius;
}

double Particle::GetRadius() const {
    return radius;
}

// Set position
void Particle::SetPosition(double newXPos, double newYPos) {
    xPos = newXPos;
    yPos = newYPos;
}

double Particle::GetXPos() const {
    return xPos;
}

double Particle::GetYPos() const {
    return yPos;
}

// Set velocity
void Particle::SetVelocity(double newVX, double newVY) {
    xVelocity = newVX;
    yVelocity = newVY;
}

double Particle::GetVX() const {
    return xVelocity;
}

double Particle::GetVY() const {
    return yVelocity;
}

// Define distance from one particle to another
double Particle::DistanceTo(const Particle& otherParticle) const {

    double dx = otherParticle.xPos - xPos;
    double dy = otherParticle.yPos - yPos;

    return sqrt((dx * dx) + (dy * dy));
}

// Apply force
void Particle::ApplyForce(double fx, double fy, double dt) {

    if (mass <= 0.0) {
        throw std::runtime_error("Why you dividing by zero???");
    }

    if (dt < 0.0) {
        throw std::runtime_error("Are you going back in time?");
    }
    // acceleration = force / mass
    double ax = fx / mass;
    double ay = fy / mass;

    xVelocity += ax * dt;
    yVelocity += ay * dt;
}

// Moving the particle
void Particle::Move(double dt) {

    // position = velocity * time
    xPos += xVelocity * dt;
    yPos += yVelocity * dt;
}

// Print basic stuff from the particle
void Particle::Print() const {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Mass: " << mass << std::endl;
    std::cout << "Charge: " << charge << std::endl;
    std::cout << "Radius: " << radius << std::endl;
    std::cout << "Position: (" << xPos << ", " << yPos << ")" << std::endl;
    std::cout << "Velocity: (" << xVelocity << ", " << yVelocity << ")" << std::endl;
}