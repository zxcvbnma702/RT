#include "scene.hpp"

RT::Scene::Scene()
{
    m_Camera.SetPosition(qbVector<double>{std::vector<double>{0.0, -10.0, 0.0}});
    m_Camera.SetLookAt(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
    m_Camera.SetUp(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
    m_Camera.SetHorizentalSize(0.25);
    m_Camera.SetAspect(16.0 / 9.0);
    m_Camera.UpdateCameraGeometry();
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
    RT::Ray cameraRay;
    qbVector<double> intPoint(3);
    qbVector<double> localNormal(3);
    qbVector<double> localColor(3);
    auto xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
    auto yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
    double minDist = 1e6;
    double maxDist = 0.0;
    for (int x = 0; x < xSize; ++x)
    {
        for (int y = 0; y < ySize; ++y)
        {
            auto normX = (static_cast<double>(x) * xFact) - 1.0;
            auto normY = (static_cast<double>(y) * yFact) - 1.0;

            m_Camera.GenerateRay(normX, normY, cameraRay);

            bool valid = m_TestSphere.TestIntersections(cameraRay, intPoint, localNormal, localColor);

            if (valid)
            {
                double dist = (intPoint - cameraRay.m_Point1).norm();
                std::cout << "distence: " << dist << std::endl;

                if (dist > maxDist)
                {
                    maxDist = dist;
                }

                if (dist < minDist)
                {
                    minDist = dist;
                }

                image.SetPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0);
            }
            else
            {
                image.SetPixel(x, y, 0.0, 0.0, 0.0);
            }
        }
    }

    std::cout << "MinNum distence: " << minDist << std::endl;
    std::cout << "ManNum distence: " << maxDist << std::endl;

    return true;
}
