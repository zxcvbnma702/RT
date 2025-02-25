#pragma once

#include "objectbase.hpp"
#include "../gtfm.hpp"

namespace qbRT
{
    class Box : public ObjectBase
    {
    public:
        Box(/* args */);
        virtual ~Box() override;

        virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal, qbVector<double> &localColor) override;

    private:
        std::array<double, 6> t;
        std::array<double, 6> u;
        std::array<double, 6> v;
    };
}