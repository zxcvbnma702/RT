#include "plane.hpp"
#include <math.h>

RT::ObjPlane::ObjPlane()
{
}

RT::ObjPlane::~ObjPlane()
{
}

bool RT::ObjPlane::TestIntersections(const RT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor)
{
    // 世界坐标系转换为局部坐标系，使用逆向变换矩阵将光线转换到平面局部坐标系
    RT::Ray bckRay = m_TransformMatrix.Apply(castRay, RT::BCKTFORM);

    // 获取局部坐标系中的光线方向向量并归一化
    qbVector<double> k = bckRay.m_Lab;
    k.Normalize();
    // 检查光线是否平行于平面
    // 判断光线方向向量的Z分量是否接近0（平面是XY平面）
    if (!CloseEnough(k.GetElement(2), 0.0))
    {
        // 计算交点参数t, 计算光线起点到平面的距离
        // 使用光线参数方程计算光线与平面的交点参数 t。
        // t = -（ 起点的 z坐标 / 光线方向向量 z 方向分量）
        auto t = bckRay.m_Point1.GetElement(2) / -k.GetElement(2);

        if (t > 0.0)
        {
            // 计算交点在局部坐标系中的坐标。
            // 知道了光线起点，距离参数，方向向量 => 表示出局部坐标系 uv 平面
            auto u = bckRay.m_Point1.GetElement(0) + (k.GetElement(0) * t);
            auto v = bckRay.m_Point1.GetElement(1) + (k.GetElement(1) * t);

            // 判断焦点是否在平面范围内
            if ((abs(u) < 1.0) && (abs(v) < 1.0))
            {
                // 计算出焦点坐标，局部坐标系
                auto poi = bckRay.m_Point1 + t * k;

                // 将交点转换回世界坐标系
                intPoint = m_TransformMatrix.Apply(poi, RT::FWDTFORM);

                // 默认平面法线是局部坐标系的-Z方向
                qbVector<double> localOrigin{std::vector<double>{0.0, 0.0, 0.0}};
                qbVector<double> normalVector{std::vector<double>{0.0, 0.0, -1.0}};
                
                // 将法线转换到世界坐标系并归一化
                qbVector<double> globalOrigin = m_TransformMatrix.Apply(localOrigin, RT::FWDTFORM);
                localNormal = m_TransformMatrix.Apply(normalVector, RT::FWDTFORM) - globalOrigin;
                localNormal.Normalize();

                // 第九步：设置颜色为平面基础颜色
                localColor = m_BaseColor;

                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    return false;
}
