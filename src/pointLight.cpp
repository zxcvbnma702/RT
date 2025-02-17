#include "pointLight.hpp"

RT::PointLight::PointLight()
{
    m_Color = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
    m_Intensity = 1.0;
}

RT::PointLight::~PointLight()
{
}

// 根据光源的位置、光线的入射角度和局部法线，来确定被照射点的光照强度和颜色。
bool RT::PointLight::ComputeIllumination(
    const qbVector<double> &intPoint,
    const qbVector<double> &localNormal,
    const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
    const std::shared_ptr<RT::ObjectBase> &currentObject,
    qbVector<double> &color,
    double &intensity)
{
    // 构造一个从交点指向光源的位置向量
    qbVector<double> lightDir = (m_Location - intPoint).Normalized();

    // 计算光线起点
    qbVector<double> startPoint = intPoint;

    // 从焦点构建一条光线到光源
    RT::Ray lightRay(startPoint, startPoint + lightDir);

    qbVector<double> poi{3};
    qbVector<double> poiNormal{3};
    qbVector<double> poiColor{3};
    bool validInt = false;

    // 判断能否到达
    for (auto sceneObject : objectList)
    {
        if (sceneObject != currentObject)
        {
            validInt = sceneObject->TestIntersections(lightRay, poi, poiNormal, poiColor);
        }
        if (validInt)
        {
            break;
        }
    }

    if (!validInt)
    {
        // Compute the angle between the local normal and the light ray.
        // Note that we assume that localNormal is a unit vector.
        double angle = acos(qbVector<double>::dot(localNormal, lightDir));

        // If the normal is pointing away from the light, then we have no illumination.
        if (angle > 1.5708)
        {
            // No illumination.
            color = m_Color;
            intensity = 0.0;
            return false;
        }
        else
        {
            // We do have illumination.
            color = m_Color;
            intensity = m_Intensity * (1.0 - (angle / 1.5708));
            return true;
        }
    }
    else
    {
        // Shadow, so no illumination.
        color = m_Color;
        intensity = 0.0;
        return false;
    }
}
