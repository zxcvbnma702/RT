#include "ray.hpp"

RT::Ray::Ray(const qbVector<double> &point1, const qbVector<double> &point2)
{
   m_Point1 = point1;
   m_Point2 = point2;
   m_Lab = m_Point2 - m_Point1;
}

qbVector<double> RT::Ray::GetPoint1() const
{
    return m_Point1;
}

qbVector<double> RT::Ray::GetPoint2() const
{
    return m_Point2;
}
