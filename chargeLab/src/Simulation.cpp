#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Particle.h"
#include "Simulation.h"
#include "Vector2D.h"
#include <string>
#include <algorithm>
#include <iostream>

// ===== File-local helpers =====
namespace {

double CoulombsLaw(double k, double q1, double q2, double r) {
    return (-k * q1 * q2) / (r * r);
}

Vector2D ComputeCoulombForceSI(
    const Vector2D& delta,
    double k,
    double q1,
    double q2,
    double minDistanceClamp,
    double metersPerPixel,
    double coulombsPerChargeUnit) {
    Vector2D deltaMeters = delta * metersPerPixel;
    double distanceSquared = deltaMeters.MagnitudeSquared();
    double minDistanceMeters = minDistanceClamp * metersPerPixel;
    double minDistanceSquared = minDistanceMeters * minDistanceMeters;

    if (distanceSquared < minDistanceSquared) {
        distanceSquared = minDistanceSquared;
    }

    double distance = std::sqrt(distanceSquared);
    if (distance == 0.0) {
        return Vector2D(0.0, 0.0);
    }

    Vector2D direction = deltaMeters / distance;
    double q1SI = q1 * coulombsPerChargeUnit;
    double q2SI = q2 * coulombsPerChargeUnit;
    double forceMagnitude = CoulombsLaw(k, q1SI, q2SI, distance);
    return direction * forceMagnitude;
}

}

// ===== Constructors and core state =====

// Default simulation size
Simulation::Simulation()
{
    width = 900.0;
    height = 700.0;
    coulombConstant = 8.9875517923e9;
    metersPerPixel = 0.01;
    coulombsPerChargeUnit = 1.0e-6;
    kilogramsPerMassUnit = 1.0e-3;
    minDistanceClamp = 20.0;

    // Add pause default
    paused = false;

    // Max particles
    maxParticles = 10;

    // Selected Particle status
    selectedParticleIndex = -1;

};

// Custom size for simulation
Simulation::Simulation(double width, double height)
{

    // update the width and height in private
    this->width = width;
    this->height = height;
    coulombConstant = 8.9875517923e9;
    metersPerPixel = 0.01;
    coulombsPerChargeUnit = 1.0e-6;
    kilogramsPerMassUnit = 1.0e-3;
    minDistanceClamp = 20.0;

    // Add puase defualt
    paused = false;

    maxParticles = 10;

    // Selected Particle status
    selectedParticleIndex = -1;

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

// ===== Particle add/remove and capacity control =====

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

// ===== Main simulation flow and rendering =====

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
    for (size_t i = 0; i < particles.size(); i++)
    {
        // Initilaize a particle
        const Particle& particle = particles[i];

        // Get an individual particle from the vector of particles
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

        // If the user selects a particle, then draw a yellow line around it
        if (static_cast<int>(i) == selectedParticleIndex) {
            circle.setOutlineThickness(4.0f);
            circle.setOutlineColor(sf::Color::Yellow);
        }

        window.draw(circle);
    }

    DrawForces(window);
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

// ===== Spatial helpers and physics internals =====

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
    double newtonsToSimulationForce = 1.0 / (kilogramsPerMassUnit * metersPerPixel);

    // Pick particle i first
    for (size_t i = 0; i < particles.size(); i++)
    {
        // Pick particle j, which is every other particle that is NOT i
        for (size_t j = i + 1; j < particles.size(); j++)
        {
            Vector2D delta = GetDeltaDistances(static_cast<int>(i), static_cast<int>(j));

            // Get the charge of each particle
            double q1 = particles[i].GetCharge();
            double q2 = particles[j].GetCharge();

            Vector2D forceSI = ComputeCoulombForceSI(
                delta,
                coulombConstant,
                q1,
                q2,
                minDistanceClamp,
                metersPerPixel,
                coulombsPerChargeUnit);

            Vector2D forceSimulation = forceSI * newtonsToSimulationForce;

            // Apply the force
            particles[i].ApplyForce(forceSimulation.GetX(), forceSimulation.GetY(), dt);
            particles[j].ApplyForce(-forceSimulation.GetX(), -forceSimulation.GetY(), dt);
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

void Simulation::IncreaseMaxParticles(int amount)
{

    // If user input amount of particles is more than the maxParticles, increase
    if (amount > 0)
    {
        maxParticles -= amount;

        if (maxParticles < 0)
        {
            maxParticles = 0;
        }
    }
}

void Simulation::DecreaseMaxParticles(int amount)
{
    if (amount > 0)
    {
        maxParticles -= amount;

        // Make sure the max particles is not negative
        if (maxParticles < 0)
        {
            maxParticles = 0;
        }
    }
}

// Clear the particles
void Simulation::ClearParticles()
{

    // remove the particles
    particles.clear();
}

bool Simulation::AddParticleAt(double x, double y, double charge)
{

    // Return particle, user defined position and charge
    return AddParticleAt(x, y, charge, 10.0, 15.0);
}

bool Simulation::AddParticleAt(double x, double y, double charge, double mass, double radius)
{

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

// ===== Selection =====

// Define select particle at
bool Simulation::SelectedParticleAt(double x, double y) {
    selectedParticleIndex = -1;

    // Initialize a vector object
    Vector2D clickPosition(x, y);

    for (int i = static_cast<int>(particles.size()) - 1; i >= 0; i--) {

        // Note where the particle is
        Vector2D particlePosition(
            particles[i].GetXPos(),
            particles[i].GetYPos()
        );

        Vector2D delta = clickPosition - particlePosition;

        // Get the distance squared. If the delta between the click position and the particle
        // position is within the radius, then the user clicked the particle
        double distanceSquared = delta.MagnitudeSquared();
        double radius = particles[i].GetRadius();

        if (distanceSquared <= radius * radius) {

            // You've selected the i'th particle
            selectedParticleIndex = i;
            return true;
        }
    }

    // return false if the user did not click within a particle
    return false;
}

void Simulation::ClearSelectedParticle() {
    selectedParticleIndex = -1;
}

bool Simulation::HasSelectedParticle() const {

    return selectedParticleIndex != -1;
}

int Simulation::GetSelectedParticleIndex() const {
    return selectedParticleIndex;
}

// ===== Force analysis =====

// Force Analysis
Vector2D Simulation::GetForceBetweenParticles(int particle1, int particle2) const {
    // if particle 1 or particle 2 is less then 0, then return vector between
    // them as zero
    if (particle1 < 0 || particle2 < 0) {
        return Vector2D(0.0, 0.0);
    }

    // if either particle has an index greater than the particles vector, then return 0
    if (particle1 >= static_cast<int>(particles.size()) || 
        particle2 >= static_cast<int>(particles.size())) {
        return Vector2D(0.0, 0.0);
    }

    // If both particles have the same index, vector return 0
    if (particle1 == particle2) {
        return Vector2D(0.0, 0.0);
    }

    // calculate the delta between the two particles
    Vector2D delta = GetDeltaDistances(particle1, particle2);
    // Get the charge 
    double q1 = particles[particle1].GetCharge();
    double q2 = particles[particle2].GetCharge();

    Vector2D forceSI = ComputeCoulombForceSI(
        delta,
        coulombConstant,
        q1,
        q2,
        minDistanceClamp,
        metersPerPixel,
        coulombsPerChargeUnit);

    double newtonsToSimulationForce = 1.0 / (kilogramsPerMassUnit * metersPerPixel);
    return forceSI * newtonsToSimulationForce;
}

Vector2D Simulation::GetTotalForceOnParticle(int particleIndex) const {

    // Instances where particle index is less than 0 or is greater than particles size
    if (particleIndex < 0 || particleIndex >= static_cast<int>(particles.size())) {
        return Vector2D(0.0, 0.0);
    }

    Vector2D totalForce(0.0, 0.0);

    for (int i = 0; i < static_cast<int>(particles.size()); i++) {
        if (i != particleIndex) {
            totalForce += GetForceBetweenParticles(particleIndex, i);
        }
    }

    return totalForce;
}

Vector2D Simulation::GetTotalForceOnParticleSI(int particleIndex) const {
    if (particleIndex < 0 || particleIndex >= static_cast<int>(particles.size())) {
        return Vector2D(0.0, 0.0);
    }

    Vector2D totalForceSI(0.0, 0.0);

    for (int i = 0; i < static_cast<int>(particles.size()); i++) {
        if (i != particleIndex) {
            Vector2D delta = GetDeltaDistances(particleIndex, i);
            double q1 = particles[particleIndex].GetCharge();
            double q2 = particles[i].GetCharge();

            totalForceSI += ComputeCoulombForceSI(
                delta,
                coulombConstant,
                q1,
                q2,
                minDistanceClamp,
                metersPerPixel,
                coulombsPerChargeUnit);
        }
    }

    return totalForceSI;
}

// ===== Force vector drawing =====

// Draw the force arrow
void Simulation::DrawForceArrow(sf::RenderWindow& window, const Vector2D& startPosition, const Vector2D& force, sf::Color color, double referenceMagnitude) const {

    // Get the magnitude of the force
    double forceMagnitude = force.Magnitude();

    // if force is less than 0 (not possible)
    if (forceMagnitude <= 0.0) {
        return;
    }

    Vector2D direction = force.Normalized();

    if (referenceMagnitude <= 0.0) {
        return;
    }

    // Scale arrow lengths relative to the strongest displayed force.
    const double minArrowLength = 8.0;
    const double maxArrowLength = 120.0;
    double normalizedMagnitude = std::clamp(forceMagnitude / referenceMagnitude, 0.0, 1.0);
    double perceptualScale = std::sqrt(normalizedMagnitude);
    double arrowLength = minArrowLength + (maxArrowLength - minArrowLength) * perceptualScale;

    double headLength = std::clamp(arrowLength * 0.18, 6.0, 14.0);
    double headHalfWidth = headLength * 0.6;

    Vector2D arrowVector = direction * arrowLength;
    Vector2D endPosition = startPosition + arrowVector;

    // Get radians, then turn to degrees
    double angleRadians = std::atan2(arrowVector.GetY(), arrowVector.GetX());
    double angleDegrees = angleRadians * 180 / 3.14159265358979323846;

    // Build the arrow
    sf::RectangleShape shaft;
    shaft.setSize({static_cast<float>(arrowLength), 3.0f});
    shaft.setPosition({
        static_cast<float>(startPosition.GetX()),
        static_cast<float>(startPosition.GetY())
    });
    shaft.setRotation(sf::degrees(static_cast<float>(angleDegrees)));
    shaft.setFillColor(color);

    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, { 0.0f, 0.0f });
    arrowHead.setPoint(1, { static_cast<float>(-headLength), static_cast<float>(-headHalfWidth) });
    arrowHead.setPoint(2, { static_cast<float>(-headLength), static_cast<float>(headHalfWidth) });
    arrowHead.setPosition({
        static_cast<float>(endPosition.GetX()),
        static_cast<float>(endPosition.GetY())
    });
    arrowHead.setRotation(sf::degrees(static_cast<float>(angleDegrees)));
    arrowHead.setFillColor(color);

    window.draw(shaft);
    window.draw(arrowHead);
}

// Implement drawing force vectors
void Simulation::DrawForces(sf::RenderWindow& window) const {
    if (!HasSelectedParticle()) {
        return;
    }


    if (selectedParticleIndex < 0 || selectedParticleIndex >= static_cast<int>(particles.size())) {
        return;
    }

    const Particle& selectedParticle = particles[selectedParticleIndex];

    Vector2D selectedPosition(
        selectedParticle.GetXPos(),
        selectedParticle.GetYPos()
    );

    std::vector<Vector2D> forcesToDraw;
    std::vector<sf::Color> colorsToDraw;
    forcesToDraw.reserve(particles.size());
    colorsToDraw.reserve(particles.size());

    double maxMagnitude = 0.0;

    for (int i = 0; i < static_cast<int>(particles.size()); ++i) {
        if (i == selectedParticleIndex) {
            continue;
        }

        Vector2D force = GetForceBetweenParticles(selectedParticleIndex, i);
        maxMagnitude = std::max(maxMagnitude, force.Magnitude());
        forcesToDraw.push_back(force);
        colorsToDraw.push_back(sf::Color::Yellow);
    }

    Vector2D totalForce = GetTotalForceOnParticle(selectedParticleIndex);
    maxMagnitude = std::max(maxMagnitude, totalForce.Magnitude());

    if (maxMagnitude <= 0.0) {
        return;
    }

    for (size_t i = 0; i < forcesToDraw.size(); ++i) {
        DrawForceArrow(window, selectedPosition, forcesToDraw[i], colorsToDraw[i], maxMagnitude);
    }

    DrawForceArrow(window, selectedPosition, totalForce, sf::Color::White, maxMagnitude);

}

// ===== Selected particle mutators =====

void Simulation::MoveSelected(double x, double y) {
    if (!HasSelectedParticle()) {
        return;
    }

    particles[selectedParticleIndex].SetPosition(x, y);
}

void Simulation::SetSelectedVelocity(double vx, double vy) {
    if (!HasSelectedParticle()) {
        return;
    }

    particles[selectedParticleIndex].SetVelocity(vx, vy);
}

void Simulation::SetSelectedCharge(double charge) {
    if (!HasSelectedParticle()) {
        return;
    }

    particles[selectedParticleIndex].SetCharge(charge);
}

void Simulation::SetSelectedMass(double mass) {
    if (!HasSelectedParticle()) {
        return;
    }

    particles[selectedParticleIndex].SetMass(mass);
}

void Simulation::SetSelectedRadius(double radius) {
    if (!HasSelectedParticle()) {
        return;
    }

    particles[selectedParticleIndex].SetRadius(radius);
}

// ===== Selected particle accessors =====

double Simulation::GetSelectedCharge() const {
    if (!HasSelectedParticle()) {
        return 0.0;
    }

    return particles[selectedParticleIndex].GetCharge();
}

double Simulation::GetSelectedMass() const {
    if (!HasSelectedParticle()) {
        return 0.0;
    }

    return particles[selectedParticleIndex].GetMass();
}

double Simulation::GetSelectedRadius() const {
    if (!HasSelectedParticle()) {
        return 0.0;
    }

    return particles[selectedParticleIndex].GetRadius();
}

double Simulation::GetSelectedX() const {
    if (!HasSelectedParticle()) {
        return 0.0;
    }

    return particles[selectedParticleIndex].GetXPos();
}

double Simulation::GetSelectedY() const {
    if (!HasSelectedParticle()) {
        return 0.0;
    }

    return particles[selectedParticleIndex].GetYPos();
}

double Simulation::GetSelectedVx() const {
    if (!HasSelectedParticle()) {
        return 0.0;
    }

    return particles[selectedParticleIndex].GetVX();
}

double Simulation::GetSelectedVy() const {
    if (!HasSelectedParticle()) {
        return 0.0;
    }

    return particles[selectedParticleIndex].GetVY();
}

double Simulation::GetParticleX(int index) const {
    if (index < 0 || index >= static_cast<int>(particles.size())) {
        return 0.0;
    }

    return particles[index].GetXPos();
}

double Simulation::GetParticleY(int index) const {
    if (index < 0 || index >= static_cast<int>(particles.size())) {
        return 0.0;
    }

    return particles[index].GetYPos();
}

double Simulation::GetParticleRadius(int index) const {
    if (index < 0 || index >= static_cast<int>(particles.size())) {
        return 0.0;
    }

    return particles[index].GetRadius();
}

double Simulation::GetParticleCharge(int index) const {
    if (index < 0 || index >= static_cast<int>(particles.size())) {
        return 0.0;
    }

    return particles[index].GetCharge();
}