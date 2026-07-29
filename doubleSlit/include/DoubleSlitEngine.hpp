#pragma once

#include <string>

namespace doubleslit
{
    class DoubleSlitEngine
    {
        public:
            [[nodiscard]] std::string status() const;

            double intensityAt(double screenPosition) const;
        
        private:
            double wavelength;
            double slitSeparation;
            double slitWidth;
            double screenDistance;
    };
}