#include "scene.hpp"

RT::Scene::Scene()
{
    m_Camera.SetPosition(qbVector<double>{std::vector<double>{0.0, -10.0, -2.0}});
    m_Camera.SetLookAt(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
    m_Camera.SetUp(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
    m_Camera.SetHorizentalSize(0.25);
    m_Camera.SetAspect(16.0 / 9.0);
    m_Camera.UpdateCameraGeometry();

    // Construct a test sphere
    m_ObjectList.push_back(std::make_shared<RT::ObjectSphere>());
    m_ObjectList.push_back(std::make_shared<RT::ObjectSphere>());
    m_ObjectList.push_back(std::make_shared<RT::ObjectSphere>());

    // Constract a test plane
    m_ObjectList.push_back(std::make_shared<RT::ObjPlane>());
    m_ObjectList.at(3)->m_BaseColor = qbVector<double>(std::vector<double>{0.5, 0.5, 0.5});

    RT::GTform planeTransformMatrix;
    planeTransformMatrix.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                                      qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                                      qbVector<double>{std::vector<double>{4.0, 4.0, 1.0}});
    m_ObjectList.at(3)->SetTransformMatrix(planeTransformMatrix);

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

    m_ObjectList.at(0)->m_BaseColor = qbVector<double>{std::vector<double>{0.25, 0.5, 0.8}};
    m_ObjectList.at(1)->m_BaseColor = qbVector<double>{std::vector<double>{1.0, 0.5, 0.0}};
    m_ObjectList.at(2)->m_BaseColor = qbVector<double>{std::vector<double>{1.0, 0.8, 0.0}};

    // test Light
    // Construct a test light.
    m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
    m_lightList.at(0)->m_Location = qbVector<double>{std::vector<double>{5.0, -10.0, -5.0}};
    m_lightList.at(0)->m_Color = qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}};

    m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
    m_lightList.at(1)->m_Location = qbVector<double>{std::vector<double>{-5.0, -10.0, -5.0}};
    m_lightList.at(1)->m_Color = qbVector<double>{std::vector<double>{1.0, 0.0, 0.0}};

    m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
    m_lightList.at(2)->m_Location = qbVector<double>{std::vector<double>{0.0, -10.0, -5.0}};
    m_lightList.at(2)->m_Color = qbVector<double>{std::vector<double>{0.0, 1.0, 0.0}};
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

            std::cout << "loading..." << std::endl;
            auto normX = (static_cast<double>(x) * xFact) - 1.0;
            auto normY = (static_cast<double>(y) * yFact) - 1.0;

            m_Camera.GenerateRay(normX, normY, cameraRay);

            std::shared_ptr<RT::ObjectBase> closestObject;
            qbVector<double> closestIntPoint{3};
            qbVector<double> closestLocalNormal{3};
            qbVector<double> closestLocalColor{3};
            double minDist = 1e6;
            bool intersectionFound = false;

            // 测试🍌
            for (auto current : m_ObjectList)
            {
                bool valid = current->TestIntersections(cameraRay, intPoint, localNormal, localColor);

                if (valid)
                {
                    intersectionFound = true;

                    // 计算摄像机到🍌点距离
                    double dist = (intPoint - cameraRay.m_Point1).norm();

                    if (dist < minDist)
                    {
                        minDist = dist;
                        closestObject = current;
                        closestIntPoint = intPoint;
                        closestLocalColor = localColor;
                        closestLocalNormal = localNormal;
                    }
                }
            }

            //
            if (intersectionFound)
            {
                // 计算🍌点阴影密度
                double intensity;
                qbVector<double> color{3};
                double red = 0.0;
                double green = 0.0;
                double blue = 0.0;
                bool validIllum = false;
                bool illumFound = false;
                for (auto currentLight : m_lightList)
                {
                    validIllum = currentLight->ComputeIllumination(closestIntPoint, closestLocalNormal, m_ObjectList, closestObject, color, intensity);

                    if (validIllum)
                    {
                        illumFound = true;
                        red += color.GetElement(0) * intensity;
                        green += color.GetElement(1) * intensity;
                        blue += color.GetElement(2) * intensity;
                    }
                }

                if (illumFound)
                {
                    red *= closestLocalColor.GetElement(0);
                    green *= closestLocalColor.GetElement(1);
                    blue *= closestLocalColor.GetElement(2);
                    image.SetPixel(x, y, red, green, blue);
                }
            }
        }
    }
    return true;
}
