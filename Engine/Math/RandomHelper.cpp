#include "pch.h"
#include "RandomHelper.h"

std::random_device RandomHelper::rd;
std::mt19937 RandomHelper::gen(RandomHelper::rd());

void RandomHelper::Init()
{
}

int RandomHelper::Int(int min, int max)
{
	std::uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

float RandomHelper::Float(float min, float max)
{
	std::uniform_real_distribution<float> dist(min, max);
	return dist(gen);
}
