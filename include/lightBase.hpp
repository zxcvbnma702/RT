#pragma once

#include <memory>
#include "qbVector.h"
#include "ray.hpp"
#include "objectBase.hpp"

namespace RT
{
    class LightBase
    {
    private:
        /* data */
    public:
        LightBase(/* args */);
        virtual ~LightBase();

        virtual bool ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                         const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                         const std::shared_ptr<RT::ObjectBase> &currentObject,
                                         qbVector<double> &color, double &intensity);

    public:
        qbVector<double> m_Color{3};
        qbVector<double> m_Location{3};
        double m_Intensity;
    };

}