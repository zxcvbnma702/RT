#pragma once

#include "qbVector.h"
#include "qbMatrix.h"
#include "ray.hpp"

namespace RT {
	constexpr bool FWDTFORM = false;
	constexpr bool BCKTFORM = true;

	class GTform
	{
	public:
		GTform();
		~GTform();

		GTform(const qbMatrix2<double>& fwd, const qbMatrix2<double>& bck);

        // 平移、旋转、缩放
		void SetTransform(const qbVector<double>& translation,
			const qbVector<double>& rotation, const qbVector<double>& scale);

		// Functions to return transform matrix
		qbMatrix2<double> GetForward() const;
		qbMatrix2<double> GetBackWard() const;

		RT::Ray Apply(const RT::Ray inputRay, bool dirFlag);
		qbVector<double> Apply(const qbVector<double>& inputVector, bool dirFlag);

		friend GTform operator* (const RT::GTform& lhs, const RT::GTform& rhs);

		GTform operator= (const GTform& rhs);

		void PrintMatrix(bool dirFlag);

		GTform Print(const qbMatrix2<double>& matrix);

		static void PrintVector(const qbVector<double>& vector);

	private:
		qbMatrix2<double> m_fwdtfm{ 4, 4 };
		qbMatrix2<double> m_bcktfm{ 4, 4 };
	};
}