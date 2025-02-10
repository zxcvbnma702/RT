#include "objectSphere.hpp"
#include "ray.hpp"

RT::ObjectSphere::ObjectSphere()
{
}

RT::ObjectSphere::~ObjectSphere()
{
}

// 测试给定光线是否与单位球（位于原点，半径为1）相交，并计算交点，法向量
bool RT::ObjectSphere::TestIntersections(const RT::Ray& castRay, qbVector<double>& intPoint, qbVector<double>& localNormal, qbVector<double>& localColor)
{
	// 获取并标准化光线的方向向量
	qbVector<double> vhat = castRay.m_Lab;
	vhat.Normalize(); // 将方向向量单位化

	// 光线与球的相交满足二次方程形式：a * t^2 + bt + c = 0
	// 其中，a = 1.0，因为方向向量已经单位化

	// 计算b系数，点乘光线起点和方向向量，再乘以2
	auto b = 2.0 * qbVector<double>::dot(castRay.m_Point1, vhat);

	// 计算c系数，点乘光线起点自身并减去球的半径平方，半径为1，所以直接减去1.0
	auto c = qbVector<double>::dot(castRay.m_Point1, castRay.m_Point1) - 1.0;

	// 计算判别式，判别式决定了二次方程根的性质
	auto intTest = (b * b) - 4.0 * c;

	// 如果判别式大于0，表示存在两个实根，即光线与球有两个交点
	if (intTest > 0.0)
	{
		// 计算判别式的平方根
		double numSqrt = sqrt(intTest);
		// 计算两个可能的交点t1和t2
		auto t1 = (-b + numSqrt) / 2.0;
		auto t2 = (-b - numSqrt) / 2.0;

		// 如果两个t值都小于0，表示光线交点在光线的反方向，不相交
		if (t1 < 0.0 && t2 < 0.0)
		{
			return false;
		}

		// 取较小的正t值，确保取到最近的交点
		double t = (t1 < t2 && t1 >= 0.0) ? t1 : t2;
		if (t < 0.0)
		{
			t = t1; // 确保至少有一个正t值有效
		}

		// 计算交点位置
		intPoint = castRay.m_Point1 + (vhat * t);

		// 计算交点处的法线，法线即为交点位置的单位化向量
		localNormal = intPoint;
		localNormal.Normalize();

		// 设置局部颜色（这里假设颜色是常量，可以根据需要进行修改）
		localColor = qbVector<double>({ 1.0, 1.0, 1.0 }); // 例如白色

		return true; // 返回true，表示光线与球相交
	}
	else
	{
		// 判别式小于等于0，表示没有实根，光线与球不相交
		return false;
	}
}