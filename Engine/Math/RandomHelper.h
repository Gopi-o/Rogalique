#pragma once
#include <random>

class RandomHelper
{
public:
	static void Init();
	static int Int(int min, int max);
	static float Float(float min, float max);

private:
	static std::random_device rd;
	static std::mt19937 gen;
};