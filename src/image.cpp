#include "image.hpp"

Image::Image()
{
	m_xSize = 0;
	m_ySize = 0;
	m_pTexture = nullptr;
}

Image::~Image()
{
	if (m_pTexture != nullptr)
	{
		SDL_DestroyTexture(m_pTexture);
	}
}

void Image::Initialize(const int xSize, const int ySize, SDL_Renderer* pRenderer)
{
	// Resize image array
	m_rChannel.resize(xSize, std::vector<double>(ySize, 0.0));
	m_gChannel.resize(xSize, std::vector<double>(ySize, 0.0));
	m_bChannel.resize(xSize, std::vector<double>(ySize, 0.0));

	// Store the dimension
	m_xSize = xSize;
	m_ySize = ySize;

	m_pRenderer = pRenderer;

	InitTexture();
}

void Image::SetPixel(const int x, const int y, const double red, const double green, const double blue)
{
	m_rChannel.at(x).at(y) = red;
	m_gChannel.at(x).at(y) = green;
	m_bChannel.at(x).at(y) = blue;
}

void Image::Display()
{
	// Allocate memory for a pixel buffer
	auto* tempPixels = new Uint32[m_xSize * m_ySize];
	// Clear the piexl buffer
	memset(tempPixels, 0, m_xSize * m_ySize * sizeof(Uint32));

	for (int x = 0; x < m_xSize; ++x)
	{
		for (int y = 0; y < m_ySize; ++y)
		{
			tempPixels[(y * m_xSize) + x] = ConvertColor(m_rChannel.at(x).at(y), m_gChannel.at(x).at(y), m_bChannel.at(x).at(y));
		}
	}

	// Update the texture with the pixel buffer
	SDL_UpdateTexture(m_pTexture, nullptr, tempPixels, m_xSize * sizeof(uint32_t));

	// Delete the pixel buffer
	delete[] tempPixels;

	// Copy the texture to the renderer
	SDL_Rect srcRect{
		.x = 0,
		.y = 0,
		.w = m_xSize,
		.h = m_ySize };

	auto bounds = srcRect;

	SDL_RenderCopy(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

Uint32 Image::ConvertColor(const double red, const double green, const double blue)
{
	// Convert the color to unsigned integers
	auto r = static_cast<unsigned char>(red);
	auto g = static_cast<unsigned char>(green);
	auto b = static_cast<unsigned char>(blue);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	// Combine colors in the order ARGB
	Uint32 pixelColor = (r << 24) + (g << 16) + (b << 8) + 255;
#else
	// Combine colors in the order BGRA
	Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

	return pixelColor;
}

void Image::InitTexture()
{
	Uint32 rMask, gMask, bMask, aMask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rMask = 0xff000000;
	gMask = 0x00ff0000;
	bMask = 0x0000ff00;
	aMask = 0x000000ff;
#else
	rMask = 0x000000ff;
	gMask = 0x0000ff00;
	bMask = 0x00ff0000;
	aMask = 0xff000000;
#endif

	if (m_pTexture != nullptr)
	{
		SDL_DestroyTexture(m_pTexture);
	}

	auto* tempSurface = SDL_CreateRGBSurface(0, m_xSize, m_ySize, 32, rMask, gMask, bMask, aMask);
	m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
	SDL_FreeSurface(tempSurface);
}