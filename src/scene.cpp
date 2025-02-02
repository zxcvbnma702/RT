#include "scene.hpp"

RT::Scene::Scene()
{
}

RT::Scene::~Scene()
{
}

bool RT::Scene::Render(Image &image)
{
    // Get the dimensions of the output image
    auto xSize = image.GetXSize();
    auto ySize = image.GetYSize();

    // Create some color variation
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            auto red = (static_cast<double>(x) / 1280.0) * 255.0;
            auto green = (static_cast<double>(y) / 720.0) * 255.0;
            image.SetPixel(x, y, red, green, 0.0);
        }
    }

    return false;
}
