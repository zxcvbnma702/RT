#pragma once

#include "texturebase.hpp"
#include "colormap.hpp"

namespace qbRT
{
    namespace Texture
    {
        class Gradient : public TextureBase
        {
        private:
            // The color map
            qbRT::Texture::ColorMap m_colorMap;

        public:
            Gradient(/* args */);
            virtual ~Gradient();

            // Set the color stop
            void SetStop(double position, const qbVector<double> &value);

            // Get the color at the given position
            virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;
        };
    }
}