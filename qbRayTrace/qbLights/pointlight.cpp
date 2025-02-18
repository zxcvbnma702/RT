/* ***********************************************************
	pointlight.cpp

	The point light class implementation - A class for handling point
	lights.

	This file forms part of the qbRayTrace project as described
	in the series of videos on the QuantitativeBytes YouTube
	channel.

	This code corresponds specifically to Episode 6 of the series,
	which may be found here:
	https://youtu.be/9K9ZYq6KgFY

	The whole series may be found on the QuantitativeBytes
	YouTube channel at:
	www.youtube.com/c/QuantitativeBytes

	GPLv3 LICENSE
	Copyright (c) 2021 Michael Bennett

***********************************************************/

#include "pointlight.hpp"

// Default constructor.
qbRT::PointLight::PointLight()
{
	m_color = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	m_intensity = 1.0;
}

// Destructor.
qbRT::PointLight::~PointLight()
{
}

/**
 * Function to compute illumination.
 *  1. 从交点intPoint到光源m_location构造一条方向向量，并归一化。
	2. 构造从交点出发的一条光线。
	3. 遍历场景中的所有物体（除了交点所在物体），检查光线是否与其中任何物体相交。
	4. 如果光线与场景中的某物体相交，则设置validInt为true，并跳出循环。
	5. 如果没有物体遮挡光源，则计算交点法线与光线方向之间的角度。
	6. 如果法线背向光源，设置颜色和强度为0，返回无光照。
	7. 如果法线朝向光源，计算光照强度和颜色，返回有光照。
	8. 如果有物体遮挡光源，则设置颜色和强度为0，返回无光照。
 */
bool qbRT::PointLight::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
										   const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
										   const std::shared_ptr<qbRT::ObjectBase> &currentObject,
										   qbVector<double> &color, double &intensity)
{
	// Construct a vector pointing from the intersection point to the light.
	qbVector<double> lightDir = (m_location - intPoint).Normalized();

	// Compute a starting point.
	qbVector<double> startPoint = intPoint;

	// Construct a ray from the point of intersection to the light.
	qbRT::Ray lightRay(startPoint, startPoint + lightDir);

	/* Check for intersections with all of the objects
		in the scene, except for the current one. */
	qbVector<double> poi{3};
	qbVector<double> poiNormal{3};
	qbVector<double> poiColor{3};
	bool validInt = false;
	for (auto sceneObject : objectList)
	{
		if (sceneObject != currentObject)
		{
			validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
		}

		/* If we have an intersection, then there is no point checking further
			so we can break out of the loop. In other words, this object is
			blocking light from this light source. */
		if (validInt)
			break;
	}

	/* Only continue to compute illumination if the light ray didn't
		intersect with any objects in the scene. Ie. no objects are
		casting a shadow from this light source. */
	if (!validInt)
	{
		// Compute the angle between the local normal and the light ray.
		// Note that we assume that localNormal is a unit vector.
		double angle = acos(qbVector<double>::dot(localNormal, lightDir));

		// If the normal is pointing away from the light, then we have no illumination.
		if (angle > 1.5708)
		{
			// No illumination.
			color = m_color;
			intensity = 0.0;
			return false;
		}
		else
		{
			// We do have illumination.
			color = m_color;
			intensity = m_intensity * (1.0 - (angle / 1.5708));
			return true;
		}
	}
	else
	{
		// Shadow, so no illumination.
		color = m_color;
		intensity = 0.0;
		return false;
	}
}
