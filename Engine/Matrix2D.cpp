#include "pch.h"
#include "Matrix2D.h"

constexpr float PI = 3.14159265358979323846f;


namespace Engine
{
	Matrix2D::Matrix2D()
	{
		m[0][0] = 1.f; m[0][1] = 0.f; m[0][2] = 0.f;
		m[1][0] = 0.f; m[1][1] = 1.f; m[1][2] = 0.f;
		m[2][0] = 0.f; m[2][2] = 0.f; m[2][2] = 1.f;
	}

	Matrix2D::Matrix2D(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22)
	{
		m[0][0] = a00; m[0][1] = a01; m[0][2] = a02;
		m[1][0] = a10; m[1][1] = a11; m[1][2] = a12;
		m[2][0] = a20; m[2][1] = a21; m[2][2] = a22;
	}

	Matrix2D::Matrix2D(Vector2Df position, float angle, Vector2Df scale)
	{
		m[0][2] = position.x;
		m[1][2] = position.y;

		float cosA = cos(angle * PI / 180.f);
		float sinA = sin(angle * PI / 180.f);

		m[0][0] = cosA * scale.x;
		m[0][1] = sinA;
		m[1][0] = -sinA;
		m[1][1] = cosA * scale.y;

		m[2][0] = 0.f;
		m[2][0] = 0.f;
		m[2][2] = 1.f;
	}

	Matrix2D Matrix2D::operator*(const Matrix2D& other) const
	{
		Matrix2D result;
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				result.m[row][col] = m[row][0] * other.m[0][col] *
					m[row][1] * other.m[1][col] *
					m[row][2] * other.m[2][col];
			}
		}

		return result;
	}

	const std::array<std::array<float, 3>, 3>& Matrix2D::GetMatrix() const
	{
		return m;
	}


	void Matrix2D::Print() const
	{
		for (int row = 0; row < 3; row++)
		{
			std::cout << "! ";
			for (int col = 0; col < 3; col++)
			{
				std::cout << m[row][col] << " ";
			}
			std::cout << "|\n";
		}
		std::cout << "\n";
	}


	Matrix2D Matrix2D::GetInversed() const
	{
		Matrix2D result;
		float determinant = m[0][0] * m[1][1] * m[2][2]
			- m[0][0] * m[1][2] * m[2][1]
			- m[0][1] * m[1][0] * m[2][2]
			+ m[0][1] * m[1][2] * m[2][0]
			+ m[0][2] * m[1][0] * m[2][1]
			- m[0][2] * m[1][1] * m[2][0];

		float inverserDeterminant = 1.0f / determinant;

		Matrix2D minor(0.f, 0.f, 0.f, 
			0.f, 0.f, 0.f, 
			0.f, 0.f, 0.f);

		std::vector<std::vector<float>> submatrix(2, std::vector<float>(2));
		int submatrixRow = 0;
		int submatrixCol = 0;

		for (int row = 0; row < 3; row++)
		{
			for (int column = 0; column < 3; column++)
			{
				submatrixRow = 0;
				submatrixCol = 0;

				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						if (i != row && j != column)
						{
							submatrix[submatrixRow][submatrixCol] = m[i][j];
							submatrixCol++;
							if (submatrixCol == 2)
							{
								submatrixCol = 0;
								submatrixRow++;
							}
						}
					}
				}

				minor.m[row][column] = submatrix[0][0] * submatrix[1][1] - submatrix[0][1] * submatrix[1][0];

				if ((row + column) % 2 == 1)
				{
					minor.m[row][column] = -minor.m[row][column];
				}
			}
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				result.m[i][j] = inverserDeterminant * minor.m[j][i];
			}
		}

		return result;

	}
}