#pragma once

#include "objectBase.hpp"
#include "gtfm.hpp"

namespace RT
{
    class ObjectSphere : public ObjectBase
    {
    public:
        ObjectSphere(/* args */);
        virtual ~ObjectSphere() override;

        virtual bool TestIntersections(const RT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;

    private:
    };
}