#pragma once

#include "objectBase.hpp"
#include "gtfm.hpp"

namespace RT
{
    class ObjPlane : public ObjectBase
    {
    public:
        ObjPlane(/* args */);

        virtual ~ObjPlane() override;

        virtual bool TestIntersections(const RT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;
    };

} // namespace RT
