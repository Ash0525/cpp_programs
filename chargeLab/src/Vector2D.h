#pragma once

#include <iostream>

class Vector2D {
    public:
        Vector2D();
        Vector2D(double newX, double newY);

        void SetX(double newX);
        void SetY(double newY);

        double GetX() const;
        double GetY() const;

        double Magnitude() const;
        double MagnitudeSquared() const;

        Vector2D Normalized() const;

        // Operator overloaders, so the program knows how to add vectors
        Vector2D operator+(const Vector2D& other) const;
        Vector2D operator-(const Vector2D& other) const;
        Vector2D operator*(double scalar) const;
        Vector2D operator/(double scalar) const;

        void operator+=(const Vector2D& other);
        void operator-=(const Vector2D& other);
        void operator*=(double scalar);

        void Print() const;

    private: 
        double x;
        double y;
};