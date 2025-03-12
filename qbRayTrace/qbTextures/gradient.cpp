#include "gradient.hpp"

qbRT::Texture::Gradient::Gradient()
{
}

qbRT::Texture::Gradient::~Gradient()
{
}

void qbRT::Texture::Gradient::SetStop(double position, const qbVector<double> &value)
{
    m_colorMap.SetStop(position, value);
}

// Get the color at the given position
qbVector<double> qbRT::Texture::Gradient::GetColor(const qbVector<double> &uvCoords)
{
    qbVector<double> inputLoc = uvCoords;
    qbVector<double> newLoc = ApplyTransform(inputLoc);

    // 
    double newU = std::min((newLoc.GetElement(1) + 1.0) / 2.0, 1.0);
    return m_colorMap.GetColor(newU);
}
