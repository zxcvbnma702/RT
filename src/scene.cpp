#include "scene.hpp"

RT::Scene::Scene()
{
    m_Camera.SetPosition(qbVector<double>{std::vector<double>{0.0, -10.0, 0.0}});
    m_Camera.SetLookAt(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
    m_Camera.SetUp(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
    m_Camera.SetHorizentalSize(0.25);
    m_Camera.SetAspect(16.0 / 9.0);
    m_Camera.UpdateCameraGeometry();

    // Construct a test sphere
    m_ObjectList.push_back(std::make_shared<RT::ObjectSphere>());
    m_ObjectList.push_back(std::make_shared<RT::ObjectSphere>());
    m_ObjectList.push_back(std::make_shared<RT::ObjectSphere>());

    // Modify the Sphere
    RT::GTform testMatrix1, testMatrix2, testMatrix3;
    testMatrix1.SetTransform(qbVector<double>{std::vector<double>{-1.5, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.5, 0.5, 0.75}});

    testMatrix2.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.75, 0.5, 0.5}});

    testMatrix3.SetTransform(qbVector<double>{std::vector<double>{1.5, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}});

    m_ObjectList.at(0)->SetTransformMatrix(testMatrix1);
    m_ObjectList.at(1)->SetTransformMatrix(testMatrix2);
    m_ObjectList.at(2)->SetTransformMatrix(testMatrix3);

    m_ObjectList.at(0)->m_BaseColor = qbVector<double>{std::vector<double>{64.0, 128.0, 200.0}};
    m_ObjectList.at(1)->m_BaseColor = qbVector<double>{std::vector<double>{255.0, 128.0, 0.0}};
    m_ObjectList.at(2)->m_BaseColor = qbVector<double>{std::vector<double>{255.0, 200.0, 0.0}};

    // test Light
    // Construct a test light.
    m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
    m_lightList.at(0)->m_Location = qbVector<double>{std::vector<double>{5.0, -10.0, -5.0}};
    m_lightList.at(0)->m_Color = qbVector<double>{std::vector<double>{255.0, 255.0, 255.0}};
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

            // 测试🍌
            for (auto current : m_ObjectList)
            {
                bool valid = current->TestIntersections(cameraRay, intPoint, localNormal, localColor);

                if (valid)
                {
                    // Compute intensity of illumination.
                    double intensity;
                    qbVector<double> color{3};
                    bool validIllum = false;
                    for (auto currentLight : m_lightList)
                    {
                        validIllum = currentLight->ComputeIllumination(intPoint, localNormal, m_ObjectList, current, color, intensity);
                    }

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

                    if (validIllum)
                    {
                        // image.SetPixel(x, y, 255.0 * intensity, 0.0, 0.0);
                        image.SetPixel(x, y, localColor.GetElement(0) * intensity,
                                       localColor.GetElement(1) * intensity,
                                       localColor.GetElement(2) * intensity);
                    }
                    else
                    {
                        // image.SetPixel(x, y, 0.0, 0.0, 0.0);
                    }
                }
                else
                {
                    // image.SetPixel(x, y, 0.0, 0.0, 0.0);
                }
            }

            // bool valid = m_TestSphere.TestIntersections(cameraRay, intPoint, localNormal, localColor);
        }
    }

    std::cout << "MinNum distence: " << minDist << std::endl;
    std::cout << "ManNum distence: " << maxDist << std::endl;

    return true;
}
