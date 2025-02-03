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
    double &intensity
) {
    // 构造一个从交点指向光源的位置向量
    qbVector<double> lightDir = (m_Location - intPoint).Normalized();
    
    // 计算光线起点
    qbVector<double> startPoint = intPoint;
    
    // 计算局部法线与光线之间的角度（假设localNormal是单位向量）
    double angle = acos(qbVector<double>::dot(localNormal, lightDir));
    
    // 如果法线指向远离光源的方向，则没有光照
    if (angle > 1.5708) {
        // 没有光照
        color = m_Color;
        intensity = 0.0;
        return false;
    } else {
        // 有光照
        color = m_Color;
        intensity = m_Intensity * (1.0 - (angle / 1.5708));
        return true;
    }
}
