#include "colormap.hpp"

qbRT::Texture::ColorMap::ColorMap()
{
}

qbRT::Texture::ColorMap::~ColorMap()
{
}

// Set the color stop
void qbRT::Texture::ColorMap::SetStop(double position, const qbVector<double> &value)
{
    m_stopPositions.push_back(position);
    m_stopValues.push_back(value);
}

// Get the color at the given position
qbVector<double> qbRT::Texture::ColorMap::GetColor(double position)
{
    int numStops = m_stopPositions.size();
    int firstStop = 0;
    int sencondStop = 0;
    double diff = 2.0;
    for (int i = 0; i < numStops; i++)
    {
        // If the position is exactly the same as the stop position
        double t = m_stopPositions.at(i) - position;
        if (fabs(t) < diff)
        {
            diff = fabs(t);
            firstStop = i;
            if (t < 0.0)
            {
                // If the position is less than the stop position
                // The second stop is the next stop
                sencondStop = std::min(i + 1, numStops);
            }
            else if (t > 0.0)
            {
                // If the position is greater than the stop position
                // The second stop is the previous stop
                sencondStop = std::max(i - 1, 0);
            }
            else
            {
                sencondStop = i;
            }
        }
    }

    // If the position is exactly the same as the stop position
    if (firstStop == sencondStop)
    {
        return m_stopValues.at(firstStop);
    }

    // If the position is less than the first stop position
    if (sencondStop < firstStop)
    {
        int temp = firstStop;
        firstStop = sencondStop;
        sencondStop = temp;
    }

    // Linear interpolation
    double x = position;
    double x0 = m_stopPositions.at(firstStop);
    double x1 = m_stopPositions.at(sencondStop);
    return m_stopValues.at(firstStop) + (m_stopValues.at(sencondStop) - m_stopValues.at(firstStop)) * (x - x0) * (1 / (x1 - x0));
}
