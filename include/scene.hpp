#pragma once

#include <vector>
#include <SDL.h>
#include <image.hpp>
#include "camera.hpp"
#include "objectSphere.hpp"

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
        RT::Camera m_Camera;
        RT::ObjectSphere m_TestSphere;
    };
}