#pragma once

#include <string>
#include <vector>
#include <SDL.h>

class Image
{
public:
    Image();
    ~Image();

    // Function to initialise
    void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);

    // Function to set the color of a pixel
    void SetPixel(const int x, const int y, const double red, const double green, const double blue);

    // Function to return the image to display
    void Display();

private:
    Uint32 ConvertColor(const double red, const double green, const double blue);
    void InitTexture();

private:
    // Array to store image data
    std::vector<std::vector<double>> m_rChannel;
    std::vector<std::vector<double>> m_gChannel;
    std::vector<std::vector<double>> m_bChannel;

    // Store the dimesions of image
    int m_xSize, m_ySize;

    // SDL stuff
    SDL_Renderer *m_pRenderer;
    SDL_Texture *m_pTexture;
};
