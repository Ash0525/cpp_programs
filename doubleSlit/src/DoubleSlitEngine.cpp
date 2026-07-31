#include "DoubleSlitEngine.hpp"
#include <cmath>
#include <numbers>
namespace doubleslit
{
    // Status to return physics engine loaded
    std::string DoubleSlitEngine::status() const {
        return "Physics engine loaded";
    }

    // Find the intensity at a specific position on the screen
    double DoubleSlitEngine::intensityAt(double screenPosition) const {

        // Calculate delta
        double delta = (std::numbers::pi * parameters_.slitSeparation * screenPosition) 
                        / (parameters_.wavelength * parameters_.screenDistance);

        // Calculate the intensity
        double intensity = std::pow(std::cos(delta), 2);

        // Return intensity
        return intensity;
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
    
}
