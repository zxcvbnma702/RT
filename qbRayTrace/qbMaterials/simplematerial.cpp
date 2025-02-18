#include "simplematerial.hpp"

qbRT::SimpleMaterial::SimpleMaterial()
{
}

qbRT::SimpleMaterial::~SimpleMaterial()
{
}

qbVector<double> qbRT::SimpleMaterial::ComputeColor(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList, const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList, const std::shared_ptr<qbRT::ObjectBase> &currentObject, const qbVector<double> &intPoint, const qbVector<double> &localNormal, const qbRT::Ray &cameraRay)
{
    qbVector<double> matColor{3};
    qbVector<double> refColor{3};
    qbVector<double> difColor{3};
    qbVector<double> spcColor{3};

    // Compute diffuse component
    difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);

    // Compute the reflection component.
	if (m_reflectivity > 0.0)
		refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);
		
	// Combine reflection and diffuse components.
	matColor = (refColor * m_reflectivity) + (difColor * (1 - m_reflectivity));

    // Compute specutar component
    if (m_shininess > 0.0)
        spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);

    matColor = matColor + spcColor;
    return matColor;
}

qbVector<double> qbRT::SimpleMaterial::ComputeSpecular(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList, const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList, const qbVector<double> &intPoint, const qbVector<double> &localNormal, const qbRT::Ray &cameraRay)
{
    qbVector<double> spcColor{3};
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;

    // Loop through all of the lights in the scene.
    for (auto currentLight : lightList)
    {
        /* Check for intersections with all objects in the scene. */
        double intensity = 0.0;

        // Construct a vector pointing from the intersection point to the light.
        qbVector<double> lightDir = (currentLight->m_location - intPoint).Normalized();

        // Compute a start point. 在交点上稍微偏移一点，以避免自相交问题。
        qbVector<double> startPoint = intPoint + (lightDir * 0.001);

        // Construct a ray from the point of intersection to the light.
        qbRT::Ray lightRay(startPoint, startPoint + lightDir);

        /* Loop through all objects in the scene to check if any
            obstruct light from this source. */
        qbVector<double> poi{3};
        qbVector<double> poiNormal{3};
        qbVector<double> poiColor{3};
        bool validInt = false;
        for (auto sceneObject : objectList)
        {
            validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
            if (validInt)
                break;
        }

        /* If no intersections were found, then proceed with
            computing the specular component. */
        if (!validInt)
        {
            // Compute the reflection vector.
            qbVector<double> d = lightRay.m_lab;
            // To do to calculate
            qbVector<double> r = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);
            r.Normalize();

            // Compute the dot product.
            qbVector<double> v = cameraRay.m_lab;
            v.Normalize();
            double dotProduct = qbVector<double>::dot(r, v);

            //     // 计算视角方向向量
            // qbVector<double> viewDir = cameraRay.m_lab.Normalized();

            // // 计算半程向量
            // qbVector<double> halfVector = (lightDir + viewDir).Normalized();

            // // 计算法向量与半程向量的点积
            // double dotProduct = qbVector<double>::dot(localNormal, halfVector);

            // Only proceed if the dot product is positive.
            if (dotProduct > 0.0)
            {
                intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
            }
        }

        red += currentLight->m_color.GetElement(0) * intensity;
        green += currentLight->m_color.GetElement(1) * intensity;
        blue += currentLight->m_color.GetElement(2) * intensity;
    }

    spcColor.SetElement(0, red);
    spcColor.SetElement(1, green);
    spcColor.SetElement(2, blue);
    return spcColor;
}
