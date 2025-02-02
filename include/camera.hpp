#pragma once

#include "qbVector.h"
#include "ray.hpp"

namespace RT
{
    class Camrera
    {
    public:
        Camrera(/* args */);
        ~Camrera() = default;

        // Function to set camera paramters
        void SetPosition(const qbVector<double> &newPosition)
        {
            m_CameraPosition = newPosition;
        };
        void SetLookAt(const qbVector<double> &newLookAt)
        {
            m_CameraLookAt = newLookAt;
        };
        void SetUp(const qbVector<double> &upVector)
        {
            m_CameraUp = upVector;
        };
        void SetLength(double newLength)
        {
            m_CameraLength = newLength;
        };
        void SetHorizentalSize(double newSize)
        {
            m_CameraHorizentalSize = newSize;
        };
        void SetAspect(double newAspect)
        {
            m_CameraAspectRatio = newAspect;
        };

        // Function to return camera parmters
        qbVector<double> GetPosition() const
        {
            return m_CameraPosition;
        };
        qbVector<double> GetLookAt() const
        {
            return m_CameraLookAt;
        };
        qbVector<double> GetUp() const
        {
            return m_CameraUp;
        };
        qbVector<double> GetU() const
        {
            return m_ProjectScreenU;
        };
        qbVector<double> GetV() const
        {
            return m_ProjectScreenV;
        };
        qbVector<double> GetScreenCenter() const
        {
            return m_ProjectScreenCenter;
        };
        double GetLength() const
        {
            return m_CameraLength;
        };
        double GetHorzentalSize() const
        {
            return m_CameraHorizentalSize;
        }
        double GetAspect() const
        {
            return m_CameraAspectRatio;
        };

        // Function to generate a ray
        RT::Ray GenerateRay(float proScreenX, float proScreenY);
        // Function to update camera geometry
        void UpdateCameraGeometry();

    private:
        qbVector<double> m_CameraPosition{3};
        qbVector<double> m_CameraLookAt{3};
        qbVector<double> m_CameraUp{3};
        double m_CameraLength;
        double m_CameraHorizentalSize;
        double m_CameraAspectRatio;

        qbVector<double> m_AlimentVector{3};
        qbVector<double> m_ProjectScreenU{3};
        qbVector<double> m_ProjectScreenV{3};
        qbVector<double> m_ProjectScreenCenter{3};
    };

}