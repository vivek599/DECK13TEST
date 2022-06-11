#pragma once


static const float lumcarr[] = { 0.2126f, 0.7152f, 0.0722f, 0.0f };
#include <iostream>
#include <pmmintrin.h>
#include <vector>
#include <chrono>

using namespace std;

class MeanLuminance
{
public:
	MeanLuminance();
	virtual ~MeanLuminance()
	{

	}

	struct Color
	{
		float r, g, b;
		const float* FArr()
		{
			return &r;
		}
	};

	void Execute();

private:
	vector<Color> Colors;
	uint32_t width = 256;
	uint32_t height = 256;

	__m128 luminanceSIMD_DOT(Color pixel);
	float luminance(const Color& pixel);
	float calculateMeanLuminanceSIMD(uint32_t width, uint32_t height, const Color* pixels);
	float calculateMeanLuminanceUnRoll(uint32_t width, uint32_t height, const Color* pixels);
	float calculateMeanLuminance(uint32_t width, uint32_t height, const Color* pixels); 
};

