#include "lightBase.hpp"

RT::LightBase::LightBase()
{
}

RT::LightBase::~LightBase()
{
}

bool RT::LightBase::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal, const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList, const std::shared_ptr<RT::ObjectBase> &currentObject, qbVector<double> &color, double &intensity)
{
    return false;
}
