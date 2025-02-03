#pragma once

#include <vector>
#include <SDL.h>
#include <image.hpp>
#include <memory>
#include "camera.hpp"
#include "objectSphere.hpp"
#include "pointLight.hpp"

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

        // The list of objects
        std::vector<std::shared_ptr<RT::ObjectBase>> m_ObjectList;

        // The list of lights in the scene.
        std::vector<std::shared_ptr<RT::LightBase>> m_lightList;
    };
}