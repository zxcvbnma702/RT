#pragma once

#include "qbVector.h"
#include "ray.hpp"

namespace RT
{
    class ObjectBase
    {
    public:
        ObjectBase(/* args */);
        virtual ~ObjectBase();

        // Function to test
        virtual bool TestIntersections(const RT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor);

        bool CloseEnough(const double f1, const double f2);

    public:
        qbVector<double> mBaseColor{3};
    };

}