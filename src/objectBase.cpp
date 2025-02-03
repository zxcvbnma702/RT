#include "objectBase.hpp"
#include <math.h>

#define EPSILON 1e-21f;

RT::ObjectBase::ObjectBase()
{

}

RT::ObjectBase::~ObjectBase()
{
}

bool RT::ObjectBase::TestIntersections(const RT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor)
{
    return false;
}

bool RT::ObjectBase::CloseEnough(const double f1, const double f2)
{
    return fabs(f1 - f2) < EPSILON;
}
