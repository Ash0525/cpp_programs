#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Particle.h"
#include "Simulation.h"
#include "Vector2D.h"
#include <string>

// Default simulation size
Simulation::Simulation()
{
    width = 900.0;
    height = 700.0;
    minDistanceClamp = 20.0;

    // Add pause default
    paused = false;

    // Max particles
    maxParticles = 10;
};

// Custom size for simulation
Simulation::Simulation(double width, double height)
{

    // update the width and height in private
    this->width = width;
    this->height = height;
    minDistanceClamp = 20.0;

    // Add puase defualt
    paused = false;

    maxParticles = 10;
}

void Simulation::TogglePaused()
{
    paused = !paused;
}

void Simulation::SetPaused(bool newPaused)
{
    paused = newPaused;
}

bool Simulation::IsPaused() const
{
    return paused;
}

bool Simulation::AddParticle(const Particle &newParticle)
{

    // Check if particles is greater than max size
    if (particles.size() >= maxParticles)
    {
        return false;
    }

    particles.push_back(newParticle);
    return true;
};

// Update function will update the physics of the simulation
void Simulation::Update(double dt)
{

    // If paused, then return
    if (paused)
    {
        return;
    }

    // Initialize the force
    ApplyCoulombForces(dt);

    for (Particle &particle : particles)
    {
        // Move the particles
        particle.Move(dt);

        // When the particles hit the wall, HandleBoundaries
        HandleBoundaries(particle);
    }

    HandleParticleCollisions();
};

// Draw simulation particles
void Simulation::Draw(sf::RenderWindow &window) const
{
    for (const Particle &particle : particles)
    {
        double radius = particle.GetRadius();

        // Declare circle object for every particle
        sf::CircleShape circle(static_cast<float>(radius));
        circle.setOrigin({static_cast<float>(radius),
                          static_cast<float>(radius)});

        // Set the position of the each particle
        circle.setPosition({

            // get the X and the Y from each particle
            static_cast<float>(particle.GetXPos()),
            static_cast<float>(particle.GetYPos())});

        // If the particle's charge is positive, then make it blue
        if (particle.GetCharge() > 0)
        {
            circle.setFillColor(sf::Color::Cyan);

            // If the particle's charge is negative, then make it red
        }
        else if (particle.GetCharge() < 0)
        {
            circle.setFillColor(sf::Color::Red);

            // If the particle has no charge, then make it white
        }
        else
        {
            circle.setFillColor(sf::Color::White);
        }

        window.draw(circle);
    }
};

// Print all particles to terminal
void Simulation::PrintParticles() const
{
    for (const Particle &particle : particles)
    {
        particle.Print();
        std::cout << std::endl;
    }
}

// Helper function: get dx and dy easily
Vector2D Simulation::GetDeltaDistances(int particle1, int particle2) const
{
    double dx = particles[particle2].GetXPos() - particles[particle1].GetXPos();
    double dy = particles[particle2].GetYPos() - particles[particle1].GetYPos();

    return Vector2D(dx, dy);
}

// Boundaries ensure that the particles do not move off the screen
void Simulation::HandleBoundaries(Particle &particle)
{
    double x = particle.GetXPos();
    double y = particle.GetYPos();

    double vx = particle.GetVX();
    double vy = particle.GetVY();

    double r = particle.GetRadius();

    // Left wall. If the x position of the particle minus radius is less than 0 (left limit of width)
    if (x - r < 0.0)
    {
        // reverse the direction of vx
        particle.SetPosition(r, y);
        particle.SetVelocity(-vx, vy);
    }

    // Right wall
    if (x + r > width)
    {
        particle.SetPosition(width - r, y);
        particle.SetVelocity(-vx, vy);
    }

    // Top wall
    if (y - r < 0.0)
    {
        // reverse the direction of vx
        particle.SetPosition(x, r);
        particle.SetVelocity(vx, -vy);
    }

    // Bottom wall
    if (y + r > height)
    {
        particle.SetPosition(x, height - r);
        particle.SetVelocity(vx, -vy);
    }
}

// Coulomb's force shows how charged particles should react to each other
void Simulation::ApplyCoulombForces(double dt)
{

    // Use the dummy coulomb's constant first
    double k = dummyCoulomb;

    // Pick particle i first
    for (size_t i = 0; i < particles.size(); i++)
    {
        // Pick particle j, which is every other particle that is NOT i
        for (size_t j = i + 1; j < particles.size(); j++)
        {
            // get the distance between particles
            double dx = particles[j].GetXPos() - particles[i].GetXPos();
            double dy = particles[j].GetYPos() - particles[i].GetYPos();

            // Clamp tiny separations to keep Coulomb force finite and stable.
            double distanceSquared = dx * dx + dy * dy;
            double minDistanceSquared = minDistanceClamp * minDistanceClamp;
            if (distanceSquared < minDistanceSquared)
            {
                distanceSquared = minDistanceSquared;
            }

            // use pythagorean theorem to get the distance
            double distance = std::sqrt(distanceSquared);

            // get the x and y components, make them normalized. they will be used for direction
            double unitX = dx / distance;
            double unitY = dy / distance;

            // Get the charge of each particle
            double q1 = particles[i].GetCharge();
            double q2 = particles[j].GetCharge();

            // Apply coulomb's force
            double coulombLaw = -k * q1 * q2 / distanceSquared;

            // Get the direction of the force
            double fx = coulombLaw * unitX;
            double fy = coulombLaw * unitY;

            // Apply the force
            particles[i].ApplyForce(fx, fy, dt);
            particles[j].ApplyForce(-fx, -fy, dt);
        }
    }
}

// This function will ensure particles stick together and not phase through each other
void Simulation::HandleParticleCollisions()
{
    for (size_t i = 0; i < particles.size(); i++)
    {
        for (size_t j = i + 1; j < particles.size(); j++)
        {

            // initialize a vector object
            Vector2D delta = GetDeltaDistances(i, j);

            double distance = delta.Magnitude();

            // minimum distance the particles need to be apart in order to bounce
            double minDistance = particles[i].GetRadius() + particles[j].GetRadius();

            // if the distance is greater than 0.0 and less than the minimum distances
            // Then the particles interact with each other
            if (distance > 0.0 && distance < minDistance)
            {

                // Normalize the vectors
                Vector2D direction = delta.Normalized();

                double overlap = minDistance - distance;

                // Push particle i backward
                particles[i].SetPosition(
                    particles[i].GetXPos() - direction.GetX() * overlap / 2.0,
                    particles[i].GetYPos() - direction.GetY() * overlap / 2.0);

                // Push particle j forward
                particles[j].SetPosition(
                    particles[j].GetXPos() + direction.GetX() * overlap / 2.0,
                    particles[j].GetYPos() + direction.GetY() * overlap / 2.0);

                double q1 = particles[i].GetCharge();
                double q2 = particles[j].GetCharge();

                // If the charges are opposite, then they need to stick together
                if (q1 * q2 < 0.0)
                {

                    // Get the information for an inelastic collision
                    double m1 = particles[i].GetMass();
                    double m2 = particles[j].GetMass();

                    double vx1 = particles[i].GetVX();
                    double vy1 = particles[i].GetVY();

                    double vx2 = particles[j].GetVX();
                    double vy2 = particles[j].GetVY();

                    // Find the final velocity after inelastic collision (momentum)
                    double finalVx = ((m1 * vx1) + (m2 * vx2)) / (m1 + m2);
                    double finalVy = ((m1 * vy1) + (m2 * vy2)) / (m1 + m2);

                    // Set the velocity for the inelastic collision
                    particles[i].SetVelocity(finalVx, finalVy);
                    particles[j].SetVelocity(finalVx, finalVy);
                }
            }
        }
    }
}

int Simulation::GetParticleCount() const
{
    return particles.size();
}

int Simulation::GetMaxParticles() const
{
    return maxParticles;
}

bool Simulation::IsFull() const
{
    return particles.size() >= maxParticles;
}

// Max particle controls

void Simulation::SetMaxParticles(int newMaxParticles)
{

    // if the new max particles is negative, then new particles is just 0
    if (newMaxParticles < 0)
    {
        maxParticles = 0;
    }

    // If the user wishes to change the number of max particles
    else
    {
        maxParticles = newMaxParticles;
    }
}

void Simulation::IncreaseMaxParticles(int amount) {
    
    // If user input amount of particles is more than the maxParticles, increase
    if (amount > 0) {
        maxParticles -= amount;

        if (maxParticles < 0) {
            maxParticles = 0;
        }
    }
}

void Simulation::DecreaseMaxParticles(int amount) {
    if (amount > 0) {
        maxParticles -= amount;


        // Make sure the max particles is not negative
        if (maxParticles < 0) {
            maxParticles = 0;
        }
    }
}

// Clear the particles
void Simulation::ClearParticles() {

    // remove the particles
    particles.clear();
}

bool Simulation::AddParticleAt(double x, double y, double charge) {
    
    // Return particle, user defined position and charge
    return AddParticleAt(x, y, charge, 10.0, 15.0);
}

bool Simulation::AddParticleAt(double x, double y, double charge, double mass, double radius) {
    
    // Initialize a new particle object
    Particle newParticle;

    newParticle.SetName("Particle " + std::to_string(GetParticleCount() + 1));
    newParticle.SetMass(mass);
    newParticle.SetCharge(charge);
    newParticle.SetRadius(radius);
    newParticle.SetPosition(x, y);
    newParticle.SetVelocity(0.0, 0.0);

    return AddParticle(newParticle);

}