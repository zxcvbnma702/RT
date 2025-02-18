#include "materialbase.hpp"
#include "materialBase.hpp"

qbRT::MaterialBase::MaterialBase()
{
    m_maxReflectionRays = 3;
    m_reflectionRayCount = 0;
}

qbVector<double> qbRT::MaterialBase::ComputeDiffuseColor(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList, const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList, const std::shared_ptr<qbRT::ObjectBase> &currentObject, const qbVector<double> &intPoint, const qbVector<double> &localNormal, const qbVector<double> &baseColor)
{
    qbVector<double> diffuseColor{3};
    double intensity;
    qbVector<double> color{3};
    double red = 0.0;
    double blue = 0.0;
    double green = 0.0;
    bool validIllum = false;
    bool illumFound = false;

    for (auto currentLight : lightList)
    {
        validIllum = currentLight->ComputeIllumination(intPoint, localNormal, objectList, currentObject, color, intensity);
        if (validIllum)
        {
            illumFound = true;
            // n * l * Id
            red += color.GetElement(0) * intensity;
            green += color.GetElement(1) * intensity;
            blue += color.GetElement(2) * intensity;
        }
    }

    if (illumFound)
    {
        // baseColor = kd
        diffuseColor.SetElement(0, red * baseColor.GetElement(0));
        diffuseColor.SetElement(1, green * baseColor.GetElement(1));
        diffuseColor.SetElement(2, blue * baseColor.GetElement(2));
    }

    return diffuseColor;
}

qbRT::MaterialBase::~MaterialBase()
{
}

qbVector<double> qbRT::MaterialBase::ComputeColor(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList, const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList, const std::shared_ptr<qbRT::ObjectBase> &currentObject, const qbVector<double> &intPoint, const qbVector<double> &localNormal, const qbRT::Ray &cameraRay)
{
    qbVector<double> matColor{3};

    return matColor;
}

qbVector<double> qbRT::MaterialBase::ComputeReflectionColor(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList, const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList, const std::shared_ptr<qbRT::ObjectBase> &currentObject, const qbVector<double> &intPoint, const qbVector<double> &localNormal, const qbRT::Ray &incidentRay)
{
    qbVector<double> reflectionColor{3};

    // Compute the reflection vector.
    qbVector<double> d = incidentRay.m_lab;
    qbVector<double> reflectionVector = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);

    // Construct the reflection ray.
    qbRT::Ray reflectionRay(intPoint, intPoint + reflectionVector);

    /* Cast this ray into the scene and find the closest object that it intersects with. */
    std::shared_ptr<qbRT::ObjectBase> closestObject;
    qbVector<double> closestIntPoint{3};
    qbVector<double> closestLocalNormal{3};
    qbVector<double> closestLocalColor{3};
    bool intersectionFound = CastRay(reflectionRay, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);

    /* Compute illumination for closest object assuming that there was a
        valid intersection. */
    qbVector<double> matColor{3};
    if ((intersectionFound) && (m_reflectionRayCount < m_maxReflectionRays))
    {
        // Increment the reflectionRayCount.
        m_reflectionRayCount++;

        // Check if a material has been assigned.
        if (closestObject->m_hasMaterial)
        {
            // Use the material to compute the color.
            matColor = closestObject->m_pMaterial->ComputeColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, reflectionRay);
        }
        else
        {
            matColor = qbRT::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->m_baseColor);
        }
    }
    else
    {
        // Leave matColor as it is.
    }

    reflectionColor = matColor;
    return reflectionColor;
}

bool qbRT::MaterialBase::CastRay(const qbRT::Ray &castRay, const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList, const std::shared_ptr<qbRT::ObjectBase> &thisObject, std::shared_ptr<qbRT::ObjectBase> &closestObject, qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal, qbVector<double> &closestLocalColor)
{
    // Test for intersections with all of the objects in the scene.
    qbVector<double> intPoint{3};
    qbVector<double> localNormal{3};
    qbVector<double> localColor{3};

    double minDist = 1e6;
    bool intersectionFound = false;
    for (auto currentObject : objectList)
    {
        if (currentObject != thisObject)
        {
            bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

            // If we have a valid intersection.
            if (validInt)
            {
                // Set the flag to show that we found an intersection.
                intersectionFound = true;

                // Compute the distance between the source and the intersection point.
                double dist = (intPoint - castRay.m_point1).norm();

                // Store a reference to this object if it is the closest.
                if (dist < minDist)
                {
                    minDist = dist;
                    closestObject = currentObject;
                    closestIntPoint = intPoint;
                    closestLocalNormal = localNormal;
                    closestLocalColor = localColor;
                }
            }
        }
    }

    return intersectionFound;
}
