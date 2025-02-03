#include "camera.hpp"
#include <math.h>

RT::Camera::Camera()
{
    m_CameraPosition = qbVector<double>{std::vector<double>{0.0, -10.0, 0.0}};
    m_CameraLookAt = qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}};
    m_CameraUp = qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}};

    m_CameraLength = 1.0;
    m_CameraHorizentalSize = 1.0;
    m_CameraAspectRatio = 1.0;
}

bool RT::Camera::GenerateRay(float proScreenX, float proScreenY, RT::Ray &cameraRay)
{
    // 计算图像平面的世界坐标
    /**
     * m_ProjectScreenU * proScreenX: 在图像平面上按照水平向量的伸缩量，从中心点移动到水平坐标为 proScreenX 的位置。
     */
    auto screenWorldPart1 = m_ProjectScreenCenter + (m_ProjectScreenU * proScreenX);
    auto screenWorldCoordinate = screenWorldPart1 + (m_ProjectScreenV * proScreenY);

    cameraRay.m_Point1 = m_CameraPosition;
    cameraRay.m_Point2 = screenWorldCoordinate;
    cameraRay.m_Lab = screenWorldCoordinate - m_CameraPosition;

    return true;
}

void RT::Camera::UpdateCameraGeometry()
{
    // Frist，计算目标点到摄像机位置的 vector
    m_AlimentVector = m_CameraLookAt - m_CameraPosition;
    m_AlimentVector.Normalize();

    // Second, 计算 UV 向量
    m_ProjectScreenU = qbVector<double>::cross(m_AlimentVector, m_CameraUp);
    m_ProjectScreenU.Normalize();
    m_ProjectScreenV = qbVector<double>::cross(m_ProjectScreenU, m_AlimentVector);
    m_ProjectScreenV.Normalize();

    // Third，计算图像平面中心点位置
    m_ProjectScreenCenter = m_CameraPosition + (m_CameraLength * m_AlimentVector);

    m_ProjectScreenU = m_ProjectScreenU * m_CameraHorizentalSize;
    m_ProjectScreenV = m_ProjectScreenV * (m_CameraHorizentalSize / m_CameraAspectRatio);
}
