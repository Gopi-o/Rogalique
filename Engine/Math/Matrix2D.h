#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <array>
#include "Vector.h"


namespace Engine
{
	class Matrix2D
	{
	public:
		Matrix2D();
		Matrix2D(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);
		Matrix2D(Vector2Df position, float angle, Vector2Df scale);

		Matrix2D operator*(const Matrix2D& other) const;

		const std::array<std::array<float, 3>, 3>& GetMatrix() const;
		Matrix2D GetInversed() const;
		void Print() const;

	private:
		std::array<std::array<float, 3>, 3>m;
	};
}

