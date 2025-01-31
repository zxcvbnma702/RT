#pragma once

#include <SDL.h>
#include "image.hpp"

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
    Image m_Image;

    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
};