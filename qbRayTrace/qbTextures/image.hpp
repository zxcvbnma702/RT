#pragma once

#include "texturebase.hpp"
#include <SDL.h>
#include <SDL_image.h>

namespace qbRT
{
    namespace Texture
    {
        class Image : public TextureBase
        {
        private:
            /* data */
        public:
            Image(/* args */);
            virtual ~Image() override;

            virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;

            bool LoadImage(std::string fileName);

        private:
            std::string m_fileName;
            SDL_Surface *m_imageSurface;
            SDL_PixelFormat *m_pixelFormat;
            bool m_imageLoaded = false;
            int m_xSize, m_ySize, m_pitch;
            uint8_t m_bytesPerPixel;
            uint32_t m_rMask, m_gMask, m_bMask, m_aMask;
        };

    } // namespace Texture

}