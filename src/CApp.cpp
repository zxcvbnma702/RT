#include "CApp.h"

CApp::CApp()
{
    isRunning = true;
    pWindow = nullptr;
    pRenderer = nullptr;
}

bool CApp::OnInit()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    pWindow = SDL_CreateWindow(
        "RT",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_SHOWN);

    if (pWindow != nullptr)
    {
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

        m_Image.Initialize(1280, 720, pRenderer);

        RT::Camrera testCamera;
        testCamera.SetPosition(qbVector<double>(std::vector<double>{0.0, 0.0, 0.0}));
        testCamera.SetLookAt(qbVector<double>(std::vector<double>{0.0, 2.0, 0.0}));
        testCamera.SetUp(qbVector<double>(std::vector<double>{0.0, 0.0, 1.0}));
        testCamera.SetLength(1.0);
        testCamera.SetHorizentalSize(1.0);
        testCamera.SetAspect(1.0);
        testCamera.UpdateCameraGeometry();

        auto screenCenter = testCamera.GetScreenCenter();
        auto screenU = testCamera.GetU();
        auto screenV = testCamera.GetV();
        PrintVector(screenCenter);
        PrintVector(screenU);
        PrintVector(screenV);
    }
    else
    {
        return false;
    }

    return true;
}

int CApp::OnExecute()
{
    SDL_Event event;

    if (OnInit() == false)
        return -1;

    while (isRunning)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            OnEvent(&event);
        }
        OnLoop();
        OnRender();
    }

    OnExit();
    return 0;
}

void CApp::OnEvent(SDL_Event *event)
{
    if (event->type == SDL_QUIT)
    {
        isRunning = false;
    }
}

void CApp::OnLoop()
{
}

void CApp::OnRender()
{
    SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 0);
    SDL_RenderClear(pRenderer);

    // Render the scene
    m_Scene.Render(m_Image);

    m_Image.Display();

    SDL_RenderPresent(pRenderer);
}

void CApp::OnExit()
{
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = nullptr;
    SDL_Quit();
}

void CApp::PrintVector(const qbVector<double> &inputVector)
{
    int nRows = inputVector.GetNumDims();
    for (int row = 0; row < nRows; row++)
    {
        std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(row) << std::endl;
    }
}
