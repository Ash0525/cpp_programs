#include "Vector2D.h"
#include <cmath>

// Default constructor
Vector2D::Vector2D() {
    x = 0.0;
    y = 0.0;
}

// Parametrized Constructor
Vector2D::Vector2D(double newX, double newY) {
    x = newX;
    y = newY;
}

// Setters
void Vector2D::SetX(double newX) {
    x = newX;
}

void Vector2D::SetY(double newY) {
    y = newY;
}

// Getters
double Vector2D::GetX() const {
    return x;
}

double Vector2D::GetY() const {
    return y;
}

// Vector magnitude
double Vector2D::Magnitude() const {
    return std::sqrt(x * x + y * y);
}

// magniutde squared, just in case
double Vector2D::MagnitudeSquared() const {
    return x * x + y * y;
}

// Return a unit vector pointing in same direction as vector
Vector2D Vector2D::Normalized() const {
    double magnitude = Magnitude();

    // Handle when magnitude is zero
    if (magnitude == 0.0) {
        return Vector2D(0.0, 0.0);
    }

    // normalized x and y
    double normX = x / magnitude;
    double normY = y / magnitude;

    return Vector2D(normX, normY);
}

// These create brand new vectors

// Vector addition overloader
Vector2D Vector2D::operator+(const Vector2D& other) const {

    // Add the x component of the two vectors
    // Add the y componenet of the two vectors
    return Vector2D(x + other.x, y + other.y);
}

// Vector subtraction overloader
Vector2D Vector2D::operator-(const Vector2D& other) const {

    // Subtract the x and y components of two components
    return Vector2D(x + other.x, y + other.y);
}

// Scalar multiplication 
Vector2D Vector2D::operator*(double scalar) const {

    // Like matrix multiplied by scalar
    return Vector2D(x * scalar, y * scalar);
}

// Scalar division
Vector2D Vector2D::operator/(double scalar) const {
    if (scalar == 0.0) {
        return Vector2D(0.0, 0.0);
    }

    return Vector2D(x / scalar, y / scalar);
}

// These add to the current vector
void Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
}

// Subtract another vector from this vector
void Vector2D::operator-=(const Vector2D& other) {
    x -= other.x;
    y -= other.y;
}

// Multiply this vector by scalar
void Vector2D::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
}

// Print vector
void Vector2D::Print() const {
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}

