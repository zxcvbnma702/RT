#include "gtfm.hpp"

RT::GTform::GTform()
{
	m_fwdtfm.SetToIdentity();
	m_bcktfm.SetToIdentity();
}

RT::GTform::~GTform()
{
}

RT::GTform::GTform(const qbMatrix2<double>& fwd, const qbMatrix2<double>& bck)
{
	if (fwd.GetNumRows() != 4 || fwd.GetNumCols() != 4
		|| bck.GetNumCols() != 4 || bck.GetNumRows() != 4) {
		throw std::invalid_argument("Cannot construct GTfrom, not 4* 4");

		m_bcktfm = bck;
		m_fwdtfm = fwd;
	}
}

void RT::GTform::SetTransform(const qbVector<double>& translation, const qbVector<double>& rotation, const qbVector<double>& scale)
{
	qbMatrix2<double> translationMatrix{ 4, 4 };
	qbMatrix2<double> rotationMatrixX{ 4, 4 };
	qbMatrix2<double> rotationMatrixY{ 4, 4 };
	qbMatrix2<double> rotationMatrixZ{ 4, 4 };
	qbMatrix2<double> scaleMatrix{ 4, 4 };

	translationMatrix.SetToIdentity();
	rotationMatrixX.SetToIdentity();
	rotationMatrixY.SetToIdentity();
	rotationMatrixZ.SetToIdentity();
	scaleMatrix.SetToIdentity();

	translationMatrix.SetElement(0, 3, translation.GetElement(0));
	translationMatrix.SetElement(1, 3, translation.GetElement(1));
	translationMatrix.SetElement(2, 3, translation.GetElement(2));

	// Rotation matrices.
	rotationMatrixZ.SetElement(0, 0, cos(rotation.GetElement(2)));
	rotationMatrixZ.SetElement(0, 1, -sin(rotation.GetElement(2)));
	rotationMatrixZ.SetElement(1, 0, sin(rotation.GetElement(2)));
	rotationMatrixZ.SetElement(1, 1, cos(rotation.GetElement(2)));

	rotationMatrixY.SetElement(0, 0, cos(rotation.GetElement(1)));
	rotationMatrixY.SetElement(0, 2, sin(rotation.GetElement(1)));
	rotationMatrixY.SetElement(2, 0, -sin(rotation.GetElement(1)));
	rotationMatrixY.SetElement(2, 2, cos(rotation.GetElement(1)));

	rotationMatrixX.SetElement(1, 1, cos(rotation.GetElement(0)));
	rotationMatrixX.SetElement(1, 2, -sin(rotation.GetElement(0)));
	rotationMatrixX.SetElement(2, 1, sin(rotation.GetElement(0)));
	rotationMatrixX.SetElement(2, 2, cos(rotation.GetElement(0)));

	scaleMatrix.SetElement(0, 0, scale.GetElement(0));
	scaleMatrix.SetElement(1, 1, scale.GetElement(1));
	scaleMatrix.SetElement(2, 2, scale.GetElement(2));

	m_fwdtfm = translationMatrix *
		scaleMatrix *
		rotationMatrixX *
		rotationMatrixY *
		rotationMatrixZ;

	m_bcktfm = m_fwdtfm;
	m_bcktfm.Inverse();
}

qbMatrix2<double> RT::GTform::GetForward() const
{
	return m_fwdtfm;
}

qbMatrix2<double> RT::GTform::GetBackWard() const
{
	return m_bcktfm;
}

RT::Ray RT::GTform::Apply(const RT::Ray inputRay, bool dirFlag)
{
	RT::Ray outputRay;

	if (dirFlag)
	{
		// Apply the forward transform.
		outputRay.m_Point1 = this->Apply(inputRay.m_Point1, RT::FWDTFORM);
		outputRay.m_Point2 = this->Apply(inputRay.m_Point2, RT::FWDTFORM);
		outputRay.m_Lab = outputRay.m_Point2 - outputRay.m_Point1;
	}
	else
	{
		// Apply the backward transform.
		outputRay.m_Point1 = this->Apply(inputRay.m_Point1, RT::BCKTFORM);
		outputRay.m_Point2 = this->Apply(inputRay.m_Point2, RT::BCKTFORM);
		outputRay.m_Lab = outputRay.m_Point2 - outputRay.m_Point1;
	}

	return outputRay;
}

qbVector<double> RT::GTform::Apply(const qbVector<double>& inputVector, bool dirFlag)
{
	// Convert inputVector to a 4-element vector.
	std::vector<double> tempData{ inputVector.GetElement(0),
					inputVector.GetElement(1),
					inputVector.GetElement(2),
					1.0 };
	qbVector<double> tempVector{ tempData };

	// Create a vector for the result.
	qbVector<double> resultVector;

	if (dirFlag)
	{
		// Apply the forward transform.
		resultVector = m_fwdtfm * tempVector;
	}
	else
	{
		// Apply the backward transform.
		resultVector = m_bcktfm * tempVector;
	}

	// Reform the output as a 3-element vector.
	qbVector<double> outputVector{ std::vector<double> {	resultVector.GetElement(0),
								resultVector.GetElement(1),
								resultVector.GetElement(2) } };

	return outputVector;
}

RT::GTform RT::GTform::operator=(const GTform& rhs)
{
	// Make sure that we're not assigning to ourself.
	if (this != &rhs)
	{
		m_fwdtfm = rhs.m_fwdtfm;
		m_bcktfm = rhs.m_bcktfm;
	}

	return *this;
}

namespace RT {
	RT::GTform RT::operator*(const RT::GTform& lhs, const RT::GTform& rhs)
	{
		// Form the product of the two forward transforms.
		qbMatrix2<double> fwdResult = lhs.m_fwdtfm * rhs.m_fwdtfm;

		// Compute the backward transform as the inverse of the forward transform.
		qbMatrix2<double> bckResult = fwdResult;
		bckResult.Inverse();

		// Form the final result.
		RT::GTform finalResult(fwdResult, bckResult);

		return finalResult;
	}
}

void RT::GTform::PrintMatrix(bool dirFlag)
{
	if (dirFlag)
	{
		Print(m_fwdtfm);
	}
	else
	{
		Print(m_bcktfm);
	}
}

RT::GTform RT::GTform::Print(const qbMatrix2<double>& matrix)
{
	int nRows = matrix.GetNumRows();
	int nCols = matrix.GetNumCols();
	for (int row = 0; row < nRows; ++row)
	{
		for (int col = 0; col < nCols; ++col)
		{
			std::cout << std::fixed << std::setprecision(3) << matrix.GetElement(row, col) << " ";
		}
		std::cout << std::endl;
	}
}

void RT::GTform::PrintVector(const qbVector<double>& vector)
{
	int nRows = vector.GetNumDims();
	for (int row = 0; row < nRows; ++row)
	{
		std::cout << std::fixed << std::setprecision(3) << vector.GetElement(row) << std::endl;
	}
}