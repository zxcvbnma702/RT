#pragma once

#include "qbVector.h"
#include "ray.hpp"
#include "gtfm.hpp"

namespace RT
{
    class ObjectBase
    {
    public:
        ObjectBase(/* args */);
        virtual ~ObjectBase();

        // Function to test
        virtual bool TestIntersections(const RT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor);

        // Function to set transform matrix
        void SetTransformMatrix(const RT::GTform &transformMatrix);

        bool CloseEnough(const double f1, const double f2);

    public:
        qbVector<double> m_BaseColor{3};

        RT::GTform m_TransformMatrix;
    };

}