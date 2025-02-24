#include "checker.hpp"

qbRT::Texture::Checker::Checker()
{
    m_color1 = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0, 1.0}};
    m_color2 = qbVector<double>{std::vector<double>{0.2, 0.2, 0.2, 1.0}};
}

qbRT::Texture::Checker::~Checker()
{
}

qbVector<double> qbRT::Texture::Checker::GetColor(const qbVector<double> &uvCoords)
{
    // 将传入的UV坐标存储到inputLoc
    qbVector<double> inputLoc = uvCoords;

    // 对传入的UV坐标应用某种变换，返回新的坐标newLoc
    qbVector<double> newLoc = ApplyTransform(inputLoc);

    // 获取变换后的U坐标
    auto newU = newLoc.GetElement(0);

    // 获取变换后的V坐标
    auto newV = newLoc.GetElement(1);

    // 定义一个颜色向量localColor，长度为4，通常表示RGBA值
    qbVector<double> localColor{4};

    // 计算格子的位置
    int check = static_cast<int>(floor(newU)) + static_cast<int>(floor(newV));

    // 判断所在的格子是黑格还是白格，并赋予相应的颜色
    if (check % 2 == 0)
    {
        localColor = m_color1; // 偶数格使用颜色1
    }
    else
    {
        localColor = m_color2; // 奇数格使用颜色2
    }

    // 返回计算得到的颜色
    return localColor;
}

void qbRT::Texture::Checker::SetColor(const qbVector<double> &inputColor1, const qbVector<double> &inputColor2)
{
    m_color1 = inputColor1;
    m_color2 = inputColor2;
}
