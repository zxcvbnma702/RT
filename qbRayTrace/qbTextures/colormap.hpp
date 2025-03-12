#pragma once

#include "../qbLinAlg/qbVector.h"

namespace qbRT
{
    namespace Texture
    {
        class ColorMap
        {
        private:
            // The stop positions
            std::vector<double> m_stopPositions;
            std::vector<qbVector<double>> m_stopValues;

        public:
            ColorMap(/* args */);
            ~ColorMap();

            void SetStop(double position, const qbVector<double> &value);

            qbVector<double> GetColor(double position);
        };

    }
}