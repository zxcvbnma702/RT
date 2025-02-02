#pragma once

#include <SDL.h>
#include "image.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "qbVector.h"

class CApp
{
public:
    CApp();

    int OnExecute();
    bool OnInit();
    void OnEvent(SDL_Event *event);
    void OnLoop();
    void OnRender();
    void OnExit();

private:
    void PrintVector(const qbVector<double> &inputVector);

private:
    Image m_Image;

    RT::Scene m_Scene;

    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
};