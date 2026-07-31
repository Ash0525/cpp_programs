#include "DoubleSlitEngine.hpp"
#include <cmath>
#include <numbers>
namespace doubleslit

{
    // Define pi
    constexpr double PI = std::numbers::pi;

    // Status to return physics engine loaded
    std::string DoubleSlitEngine::status() const {
        return "Physics engine loaded";
    }

    // Find the intensity at a specific position on the screen
    double DoubleSlitEngine::intensityAt(double screenPosition) const {
        // Let a be the slit width
        double a = parameters_.slitWidth;

        // Let x be the screenPosition
        double x = screenPosition;

        // Let lambda be the wavelength
        double lambda = parameters_.wavelength;

        // Let L be the distance away from the screen
        double L = parameters_.screenDistance;

        // Let d be the slit distance
        double d = parameters_.slitSeparation;

        // Calculate delta equal the interferencePhase
        const double delta = (PI * d * x) / (lambda * L);

        // Let beta equal the diffraction envelope
        const double beta = (PI * a * x) / (lambda * L);

        // Calculate the intensity
        double intensity = std::pow(std::cos(delta), 2);

        // If the user wants a more realistic representation (with sine envelope)
        if (!diffractionEnabled_)
        {
            return intensity;
        }

        // Let the diffraction amplitude be fixed at 1.0, just in case
        double diffractionAmplitude = 1.0;

        // Get the envelope
        if (std::abs(beta) > 1e-12)
        {
            diffractionAmplitude = std::sin(beta) / beta;
        }

        const double diffractionEnvelope = diffractionAmplitude * diffractionAmplitude;

        // Return intensity
        return intensity * diffractionEnvelope;
    }

    // Generate pattern
    SimulationData DoubleSlitEngine::generatePattern(
        double minPosition,
        double maxPosition,
        int sampleCount
    ) const
    {
        // Reject invalid output, return empty
        if (sampleCount < 2 || minPosition >= maxPosition)
        {
            return {};
        }

        SimulationData data;

        // Create a step
        double step = (maxPosition - minPosition) / (sampleCount - 1);

        // Calculate each sampled position and its intensity
        for (int i = 0; i < sampleCount; i++)
        {
            double position = minPosition + i * step;
            data.positions.push_back(position);
            data.intensities.push_back(intensityAt(position));
        }

        return data;
    }

    // Let the user set diffractionEnabled
    void DoubleSlitEngine::setDiffractionEnabled(bool enabled)
    {
        diffractionEnabled_ = enabled;
    }

    // Get true or false from diffraction enabled
    bool DoubleSlitEngine::diffractionEnabled() const
    {
        return diffractionEnabled_;
    }

    // SIMULATION TOGGLES

    // User set wavelength
    void DoubleSlitEngine::setWavelength(double wavelength)
    {
        if (wavelength > 0.0) 
        {
            parameters_.wavelength = wavelength;
        }
    }

    // Get the wavelength
    double DoubleSlitEngine::wavelength() const
    {
        return parameters_.wavelength;
    }

    // User set slit distance
    void DoubleSlitEngine::setSlitSeparation(double slitSeparation) 
    {
        if (slitSeparation > 0.0)
        {
            parameters_.slitSeparation = slitSeparation;
        }
    }

    // Get slit distance
    double DoubleSlitEngine::slitSeparation() const
    {
        return parameters_.slitSeparation;
    }

    // User set slit width
    void DoubleSlitEngine::setSlitWidth(double slitWidth)
    {
        if (slitWidth > 0.0)
        {
            parameters_.slitWidth = slitWidth;
        }
    }

    double DoubleSlitEngine::slitWidth() const
    {
        return parameters_.slitWidth;
    }

    // User set screen distance
    void DoubleSlitEngine::setScreenDistance(double screenDistance)
    {
        if (screenDistance > 0.0)
        {
            parameters_.screenDistance = screenDistance;
        }
    }

    double DoubleSlitEngine::screenDistance() const
    {
        return parameters_.screenDistance;
    }
}
