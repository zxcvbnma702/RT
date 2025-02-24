#ifndef CHECKER_H
#define CHECKER_H

#include "texturebase.hpp"

namespace qbRT
{
	namespace Texture
	{
		class Checker : public TextureBase
		{
			public:
				// Constructor / destructor.
				Checker();
				virtual ~Checker() override;
			
				// Function to return the color.
				virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;
			
				// Function to set the colors.
				void SetColor(const qbVector<double> &inputColor1, const qbVector<double> &inputColor2);
			
		private:
			qbVector<double> m_color1 {4};
			qbVector<double> m_color2 {4};
			
		};
	}
}

#endif