/* ***********************************************************
	scene.cpp

	The scene class implementation - A class to handle information
	about the scene and rendering to an image.

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

// scene.cpp

#include "scene.hpp"

// The constructor.
qbRT::Scene::Scene()
{
	// **************************************************************************************
	// Configure the camera.
	// **************************************************************************************
	m_camera.SetPosition(qbVector<double>{std::vector<double>{2.0, -5.0, 0.25}});
	m_camera.SetLookAt(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
	m_camera.SetUp(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
	m_camera.SetHorzSize(1.0);
	m_camera.SetAspect(16.0 / 9.0);
	m_camera.UpdateCameraGeometry();

	// **************************************************************************************
	// Setup ambient lightling.
	// **************************************************************************************
	qbRT::MaterialBase::m_ambientColor = std::vector<double>{1.0, 1.0, 1.0};
	qbRT::MaterialBase::m_ambientIntensity = 0.2;

	// **************************************************************************************
	// Create some textures.
	// **************************************************************************************
	auto floorTexture = std::make_shared<qbRT::Texture::Checker>(qbRT::Texture::Checker());
	auto gradientTexture = std::make_shared<qbRT::Texture::Gradient>(qbRT::Texture::Gradient());
	auto wallTexture = std::make_shared<qbRT::Texture::Checker>(qbRT::Texture::Checker());
	auto wallTexture2 = std::make_shared<qbRT::Texture::Checker>(qbRT::Texture::Checker());
	auto imageTexture = std::make_shared<qbRT::Texture::Image>(qbRT::Texture::Image());

	auto smallCheck1 = std::make_shared<qbRT::Texture::Checker>(qbRT::Texture::Checker());
	auto smallCheck2 = std::make_shared<qbRT::Texture::Checker>(qbRT::Texture::Checker());

	// **************************************************************************************
	// Setup the textures.
	// **************************************************************************************
	smallCheck1->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
							  0.0,
							  qbVector<double>{std::vector<double>{8.0, 8.0}});
	smallCheck1->SetColor(qbVector<double>{std::vector<double>{0.2, 0.2, 0.8, 1.0}},
						  qbVector<double>{std::vector<double>{1.0, 1.0, 1.0, 1.0}});

	smallCheck2->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
							  0.0,
							  qbVector<double>{std::vector<double>{8.0, 8.0}});
	smallCheck2->SetColor(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0, 1.0}},
						  qbVector<double>{std::vector<double>{1.0, 0.5, 0.1, 1.0}});

	floorTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
							   0.0,
							   qbVector<double>{std::vector<double>{16.0, 16.0}});
	// floorTexture->SetColor(qbVector<double>{std::vector<double>{0.2, 0.2, 0.2, 1.0}}, qbVector<double>{std::vector<double>{0.4, 0.4, 0.4, 1.0}});
	floorTexture->SetColor(smallCheck1, smallCheck2);

	gradientTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
								  0.0,
								  qbVector<double>{std::vector<double>{1.0, 1.0}});
	gradientTexture->SetStop(0.0, qbVector<double>{std::vector<double>{1.0, 0.0, 0.0, 1.0}});
	gradientTexture->SetStop(0.5, qbVector<double>{std::vector<double>{0.0, 1.0, 0.0, 1.0}});
	gradientTexture->SetStop(1.0, qbVector<double>{std::vector<double>{0.0, 0.0, 1.0, 1.0}});

	wallTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
							  0.0,
							  qbVector<double>{std::vector<double>{16.0, 16.0}});

	wallTexture->SetColor(qbVector<double>{std::vector<double>{0.529, 0.808, 0.922, 1.0}},
						  qbVector<double>{std::vector<double>{1.0, 1.0, 1.0, 1.0}});

	wallTexture2->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
							   0.0,
							   qbVector<double>{std::vector<double>{16.0, 16.0}});

	wallTexture2->SetColor(qbVector<double>{std::vector<double>{0.961, 0.961, 0.863, 1.0}},
						   qbVector<double>{std::vector<double>{1.0, 1.0, 1.0, 1.0}});

	imageTexture->LoadImage("/Library/Projects/Vulkan/RT/image1.png");
	imageTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
							   0.0,
							   qbVector<double>{std::vector<double>{1.0, 1.0}});

	// **************************************************************************************
	// Create some materials.
	// **************************************************************************************
	auto floorMaterial = std::make_shared<qbRT::SimpleMaterial>(qbRT::SimpleMaterial());
	auto ceilingMaterial = std::make_shared<qbRT::SimpleRefractive>(qbRT::SimpleRefractive());
	auto wallMaterial = std::make_shared<qbRT::SimpleMaterial>(qbRT::SimpleMaterial());
	auto wallMaterial2 = std::make_shared<qbRT::SimpleMaterial>(qbRT::SimpleMaterial());
	auto imageMaterial = std::make_shared<qbRT::SimpleMaterial>(qbRT::SimpleMaterial());
	auto sphereMaterial = std::make_shared<qbRT::SimpleMaterial>(qbRT::SimpleMaterial());
	auto sphereMaterial2 = std::make_shared<qbRT::SimpleMaterial>(qbRT::SimpleMaterial());
	auto sphereMaterial3 = std::make_shared<qbRT::SimpleMaterial>(qbRT::SimpleMaterial());
	auto coneMaterial = std::make_shared<qbRT::SimpleMaterial>(qbRT::SimpleMaterial());
	auto glassMaterial = std::make_shared<qbRT::SimpleRefractive>(qbRT::SimpleRefractive());

	// **************************************************************************************
	// Setup the materials.
	// **************************************************************************************
	floorMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	floorMaterial->m_reflectivity = 0.25;
	floorMaterial->m_shininess = 0.0;
	floorMaterial->AssignTexture(floorTexture);

	ceilingMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	ceilingMaterial->m_reflectivity = 0.25;
	ceilingMaterial->m_shininess = 0.0;
	ceilingMaterial->m_translucency = 0.9;
	ceilingMaterial->m_ior = 1.57;
	ceilingMaterial->AssignTexture(floorTexture);

	wallMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	wallMaterial->m_reflectivity = 0.25;
	wallMaterial->m_shininess = 0.0;
	wallMaterial->AssignTexture(wallTexture);

	wallMaterial2->m_baseColor = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	wallMaterial2->m_reflectivity = 0.25;
	wallMaterial2->m_shininess = 0.0;
	wallMaterial2->AssignTexture(wallTexture2);

	imageMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 0.125, 0.125}};
	imageMaterial->m_reflectivity = 0.0;
	imageMaterial->m_shininess = 0.0;
	imageMaterial->AssignTexture(imageTexture);

	sphereMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 0.2, 0.2}};
	sphereMaterial->m_reflectivity = 0.8;
	sphereMaterial->m_shininess = 32.0;

	sphereMaterial2->m_baseColor = qbVector<double>{std::vector<double>{0.2, 1.0, 0.2}};
	sphereMaterial2->m_reflectivity = 0.8;
	sphereMaterial2->m_shininess = 32.0;

	sphereMaterial3->m_baseColor = qbVector<double>{std::vector<double>{0.2, 0.2, 1.0}};
	sphereMaterial3->m_reflectivity = 0.8;
	sphereMaterial3->m_shininess = 32.0;

	coneMaterial->m_baseColor = qbVector<double>{std::vector<double>{0.8, 0.8, 0.2}};
	coneMaterial->m_reflectivity = 0.15;
	coneMaterial->m_shininess = 32.0;

	glassMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	glassMaterial->m_reflectivity = 0.25;
	glassMaterial->m_shininess = 32.0;
	glassMaterial->m_translucency = 0.5;
	glassMaterial->m_ior = 1.57;
	glassMaterial->AssignTexture(gradientTexture);

	// **************************************************************************************
	// Create and setup objects.
	// **************************************************************************************
	auto floor = std::make_shared<qbRT::ObjPlane>(qbRT::ObjPlane());
	floor->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}},
										   qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
										   qbVector<double>{std::vector<double>{16.0, 16.0, 1.0}}});
	floor->AssignMaterial(floorMaterial);

	auto ceiling = std::make_shared<qbRT::ObjPlane>(qbRT::ObjPlane());
	ceiling->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{0.0, 0.0, -4.0}},
											 qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
											 qbVector<double>{std::vector<double>{16.0, 16.0, 0.1}}});
	ceiling->AssignMaterial(ceilingMaterial);

	// **************************************************************************************
	auto wall = std::make_shared<qbRT::ObjPlane>(qbRT::ObjPlane());
	wall->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{-2.0, 0.0, 1.0}},
										  qbVector<double>{std::vector<double>{0.0, M_PI / 2.0, 0.0}},
										  qbVector<double>{std::vector<double>{16.0, 16.0, 1.0}}});
	wall->AssignMaterial(wallMaterial2);

	auto wall2 = std::make_shared<qbRT::ObjPlane>(qbRT::ObjPlane());
	wall2->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{0.0, 6.0, 1.0}},
										   qbVector<double>{std::vector<double>{M_PI / 2.0, 0.0, 0.0}},
										   qbVector<double>{std::vector<double>{16.0, 16.0, 1.0}}});
	wall2->AssignMaterial(wallMaterial);

	// **************************************************************************************
	auto imagePlane = std::make_shared<qbRT::ObjPlane>(qbRT::ObjPlane());
	imagePlane->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{2.0, 5.0, -0.75}},
												qbVector<double>{std::vector<double>{-M_PI / 2.0, 0.0, 0.0}},
												qbVector<double>{std::vector<double>{1.75, 1.75, 1.0}}});
	imagePlane->AssignMaterial(imageMaterial);

	// **************************************************************************************
	auto sphere = std::make_shared<qbRT::ObjSphere>(qbRT::ObjSphere());
	sphere->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{-2.0, -2.0, 0.25}},
											qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
											qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}}});
	sphere->AssignMaterial(sphereMaterial);

	// **************************************************************************************
	auto sphere2 = std::make_shared<qbRT::ObjSphere>(qbRT::ObjSphere());
	sphere2->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{-2.0, -0.5, 0.25}},
											 qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
											 qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}}});
	sphere2->AssignMaterial(sphereMaterial2);

	// **************************************************************************************
	auto sphere3 = std::make_shared<qbRT::ObjSphere>(qbRT::ObjSphere());
	sphere3->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{-2.0, -1.25, -1.0}},
											 qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
											 qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}}});
	sphere3->AssignMaterial(sphereMaterial3);

	// **************************************************************************************
	auto cylinder = std::make_shared<qbRT::Cylinder>(qbRT::Cylinder());
	cylinder->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{1.0, -2.0, 0.25}},
											  qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
											  qbVector<double>{std::vector<double>{0.25, 0.25, 0.25}}});
	cylinder->AssignMaterial(glassMaterial);

	// **************************************************************************************
	auto cone = std::make_shared<qbRT::Cone>(qbRT::Cone());
	cone->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{1.0, -2.0, 0.50}},
										  qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
										  qbVector<double>{std::vector<double>{0.25, 0.25, 0.25}}});
	cone->AssignMaterial(coneMaterial);

	// **************************************************************************************
	auto box = std::make_shared<qbRT::Box>(qbRT::Box());
	box->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
										 qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
										 qbVector<double>{std::vector<double>{0.5, 2.0, 1.0}}});
	box->AssignMaterial(coneMaterial);

	// **************************************************************************************
	auto rmTorus = std::make_shared<qbRT::RM::Torus>(qbRT::RM::Torus());
	rmTorus->SetRadii(0.7, 0.3);
	rmTorus->SetTransformMatrix(qbRT::GTform{qbVector<double>{std::vector<double>{2.0, -1.25, 0.25}},
											 qbVector<double>{std::vector<double>{-M_PI / 2.0, 0.0, 0.0}},
											 qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}}});
	rmTorus->AssignMaterial(glassMaterial);

	// **************************************************************************************
	// Put the objects into the scene.
	// **************************************************************************************
	m_objectList.push_back(floor);
	// m_objectList.push_back(ceiling);
	m_objectList.push_back(imagePlane);
	// m_objectList.push_back(sphere);
	// m_objectList.push_back(sphere2);
	// m_objectList.push_back(sphere3);
	// m_objectList.push_back(box);
	// m_objectList.push_back(cone);
	m_objectList.push_back(cylinder);
	m_objectList.push_back(rmTorus);

	m_objectList.push_back(wall);
	m_objectList.push_back(wall2);

	// **************************************************************************************
	// Construct and setup the lights.
	// **************************************************************************************
	m_lightList.push_back(std::make_shared<qbRT::PointLight>(qbRT::PointLight()));
	m_lightList.at(0)->m_location = qbVector<double>{std::vector<double>{3.0, -10.0, -5.0}};
	m_lightList.at(0)->m_color = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	m_lightList.at(0)->m_intensity = 16.0;

	m_lightList.push_back(std::make_shared<qbRT::PointLight>(qbRT::PointLight()));
	m_lightList.at(1)->m_location = qbVector<double>{std::vector<double>{0.0, -10.0, -5.0}};
	m_lightList.at(1)->m_color = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	m_lightList.at(1)->m_intensity = 8.0;

	// m_lightList.push_back(std::make_shared<qbRT::PointLight>(qbRT::PointLight()));
	// m_lightList.at(0)->m_location = qbVector<double>{std::vector<double>{1.0, 5.0, -3.0}};
	// m_lightList.at(0)->m_color = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
	// m_lightList.at(0)->m_intensity = 2.0;

	// m_lightList.push_back(std::make_shared<qbRT::PointLight>(qbRT::PointLight()));
	// m_lightList.at(1)->m_location = qbVector<double>{std::vector<double>{1.0, 3.0, 0.50}};
	// m_lightList.at(1)->m_color = qbVector<double>{std::vector<double>{1.0, 0.753, 0.790}};
	// m_lightList.at(1)->m_intensity = 2.0;
}

// Function to perform the rendering.
bool qbRT::Scene::Render(qbImage &outputImage)
{
	// Get the dimensions of the output image.
	int xSize = outputImage.GetXSize();
	int ySize = outputImage.GetYSize();

	// Loop over each pixel in our image.
	qbRT::Ray cameraRay;
	qbVector<double> intPoint(3);
	qbVector<double> localNormal(3);
	qbVector<double> localColor(3);
	double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
	double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
	double minDist = 1e6;
	double maxDist = 0.0;
	for (int y = 0; y < ySize; ++y)
	{
		// Display progress.
		std::cout << "Processing line " << y << " of " << ySize << "." << " \r";
		std::cout.flush();

		for (int x = 0; x < xSize; ++x)
		{
			// Normalize the x and y coordinates.
			double normX = (static_cast<double>(x) * xFact) - 1.0;
			double normY = (static_cast<double>(y) * yFact) - 1.0;

			// Generate the ray for this pixel.
			m_camera.GenerateRay(normX, normY, cameraRay);

			// Test for intersections with all objects in the scene.
			std::shared_ptr<qbRT::ObjectBase> closestObject;
			qbVector<double> closestIntPoint{3};
			qbVector<double> closestLocalNormal{3};
			qbVector<double> closestLocalColor{3};
			bool intersectionFound = CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);

			/* Compute the illumination for the closest object, assuming that there
				was a valid intersection. */
			if (intersectionFound)
			{
				// Check if the object has a material.
				if (closestObject->m_hasMaterial)
				{
					// Use the material to compute the color.
					qbRT::MaterialBase::m_reflectionRayCount = 0;
					qbVector<double> color = closestObject->m_pMaterial->ComputeColor(m_objectList, m_lightList,
																					  closestObject, closestIntPoint,
																					  closestLocalNormal, cameraRay);
					outputImage.SetPixel(x, y, color.GetElement(0), color.GetElement(1), color.GetElement(2));
				}
				else
				{
					// Use the basic method to compute the color.
					qbVector<double> matColor = qbRT::MaterialBase::ComputeDiffuseColor(m_objectList, m_lightList,
																						closestObject, closestIntPoint,
																						closestLocalNormal, closestObject->m_baseColor);
					outputImage.SetPixel(x, y, matColor.GetElement(0), matColor.GetElement(1), matColor.GetElement(2));
				}
			}
		}
	}

	return true;
}

// Function to cast a ray into the scene.
bool qbRT::Scene::CastRay(qbRT::Ray &castRay, std::shared_ptr<qbRT::ObjectBase> &closestObject,
						  qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal,
						  qbVector<double> &closestLocalColor)
{
	qbVector<double> intPoint{3};
	qbVector<double> localNormal{3};
	qbVector<double> localColor{3};
	double minDist = 1e6;
	bool intersectionFound = false;
	for (auto currentObject : m_objectList)
	{
		bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

		// If we have a valid intersection.
		if (validInt)
		{
			// Set the flag to indicate that we found an intersection.
			intersectionFound = true;

			// Compute the distance between the camera and the point of intersection.
			double dist = (intPoint - castRay.m_point1).norm();

			/* If this object is closer to the camera than any one that we have
				seen before, then store a reference to it. */
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

	return intersectionFound;
}
