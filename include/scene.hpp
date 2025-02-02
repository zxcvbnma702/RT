#pragma once

#include <vector>
#include <SDL.h>
#include <image.hpp>

namespace RT
{
    class Scene
    {
    public:
        Scene();
        ~Scene();

        // Function to perform the rendering
        bool Render(Image &image);

    private:
    private:
    };
}