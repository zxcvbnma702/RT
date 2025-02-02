#pragma once
#include "qbVector.h"

namespace RT
{
    class Ray
    {
    public:
        Ray(const qbVector<double> &point1, const qbVector<double> &point2);

        qbVector<double> GetPoint1() const;
        qbVector<double> GetPoint2() const;

    public:
        qbVector<double> m_Point1{3};
        qbVector<double> m_Point2{3};
        qbVector<double> m_Lab{3};
    };     
}