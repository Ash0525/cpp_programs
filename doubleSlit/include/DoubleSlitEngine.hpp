#pragma once

#include <string>
#include <vector>

namespace doubleslit
{

    struct SimulationParameters
    {
        double wavelength = 650e-9;
        double slitWidth = 40e-6;
        double slitSeparation = 250e-6;
        double screenDistance = 1.0;
    };

    struct SimulationData
    {
        std::vector<double> positions;
        std::vector<double> intensities;
    };

    class DoubleSlitEngine
    {
        public:
            [[nodiscard]] std::string status() const;
            [[nodiscard]] double intensityAt(double screenPosition) const;
            [[nodiscard]] SimulationData generatePattern(
                double minPosition,
                double maxPosition,
                int sampleCount
            ) const;
        
        private:
            SimulationParameters parameters_;
    };


}