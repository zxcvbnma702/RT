#pragma once

#include "materialbase.hpp"

namespace qbRT
{
    class SimpleRefractive : public MaterialBase
    {
    public:
        // Constructor / destructor.
        SimpleRefractive(/* args */);
        virtual ~SimpleRefractive() override;

        // Function to return the color.
        virtual qbVector<double> ComputeColor(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
                                              const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
                                              const std::shared_ptr<qbRT::ObjectBase> &currentObject,
                                              const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                              const qbRT::Ray &cameraRay) override;

        // Function to compute the specular highlights.
        qbVector<double> ComputeSpecular(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
                                         const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
                                         const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                         const qbRT::Ray &cameraRay);

        qbVector<double> ComputeTranslucency(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
                                             const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
                                             const std::shared_ptr<qbRT::ObjectBase> &currentObject,
                                             const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                             const qbRT::Ray &incidentRay);

    public:
        qbVector<double> m_baseColor{std::vector<double>{1.0, 0.0, 1.0}};
        // 反射率
        double m_reflectivity = 0.0;
        // 高光系数
        double m_shininess = 0.0;
        // 透明度
        double m_translucency = 0.0;
        // 折射率
        double m_ior = 1.0;
    };
}