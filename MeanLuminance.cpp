#include "MeanLuminance.h"

MeanLuminance::MeanLuminance()
{
	for (uint32_t x = 0; x < width; ++x)
	{
		for (uint32_t y = 0; y < height; ++y)
		{
			Color c;
			c.r = float(rand() % 100) / 100.0f;
			c.g = float(rand() % 100) / 100.0f;
			c.b = float(rand() % 100) / 100.0f;
			Colors.emplace_back(c);
		}
	}
}

void MeanLuminance::Execute()
{
	auto start = std::chrono::system_clock::now();
	auto mlumOri = calculateMeanLuminance(width, height, Colors.data());
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff0 = end - start;

	auto mlumstart = std::chrono::system_clock::now();
	auto mlum = calculateMeanLuminanceUnRoll(width, height, Colors.data());
	auto mlumend = std::chrono::system_clock::now();
	std::chrono::duration<double> diff1 = mlumend - mlumstart;

	auto mlumSstart = std::chrono::system_clock::now();
	auto mlumS = calculateMeanLuminanceSIMD(width, height, Colors.data());
	auto mlumSend = std::chrono::system_clock::now();

	std::chrono::duration<double> diff2 = mlumSend - mlumSstart;

	std::cout << "Lum Ori " << mlumOri << " Lum Unroll " << mlum << " Lum SIMD " << mlumS << " s\n";
	std::cout << "Time Ori " << diff0.count() << " Time Unroll " << diff1.count() << " Time SIMD " << diff2.count() << " s\n";
}

float MeanLuminance::luminance(const Color& pixel)
{
	return pixel.r * 0.2126f + pixel.g * 0.7152f + pixel.b * 0.0722f;
}

__m128 MeanLuminance::luminanceSIMD_DOT(Color pixel)
{
	__m128 c, lumc, A, dot, ret;

	c = _mm_loadu_ps(pixel.FArr());
	c.m128_f32[3] = 0.0f;
	lumc = _mm_loadu_ps(lumcarr);

	dot = _mm_setzero_ps();
	A = _mm_mul_ps(c, lumc);
	A = _mm_hadd_ps(A, A);
	ret = _mm_add_ps(A, dot);
	ret = _mm_hadd_ps(ret, ret);
	return ret;
}

float MeanLuminance::calculateMeanLuminanceSIMD(uint32_t width, uint32_t height, const Color* pixels)
{
	__m128 meanLuminance = _mm_setzero_ps();
	__m128 tmp = _mm_setzero_ps();

	for (uint32_t x = 0u; x < width; ++x)
	{
		for (uint32_t y = 0u; y < height; y += 4)
		{
			const Color pixel0 = pixels[(y + 0) * width + x];
			const Color pixel1 = pixels[(y + 1) * width + x];
			const Color pixel2 = pixels[(y + 2) * width + x];
			const Color pixel3 = pixels[(y + 3) * width + x];

			__m128 A = _mm_setzero_ps();
			float V[4] = {};
			V[0] = luminanceSIMD_DOT(pixel0).m128_f32[0];
			V[1] = luminanceSIMD_DOT(pixel1).m128_f32[0];
			V[2] = luminanceSIMD_DOT(pixel2).m128_f32[0];
			V[3] = luminanceSIMD_DOT(pixel3).m128_f32[0];
			A = _mm_loadu_ps(V);
			meanLuminance = _mm_add_ps(A, meanLuminance);
		}
	}
	meanLuminance = _mm_hadd_ps(meanLuminance, meanLuminance);
	meanLuminance = _mm_add_ps(tmp, meanLuminance);
	meanLuminance = _mm_hadd_ps(meanLuminance, meanLuminance);
	return meanLuminance.m128_f32[0] / (width * height);
}

float MeanLuminance::calculateMeanLuminanceUnRoll(uint32_t width, uint32_t height, const Color* pixels)
{
	float meanLuminance = 0.0f;

	for (uint32_t x = 0u; x < width; ++x)
	{
		for (uint32_t y = 0u; y < height; y += 8)
		{
			const Color pixel0 = pixels[(y + 0) * width + x];
			const Color pixel1 = pixels[(y + 1) * width + x];
			const Color pixel2 = pixels[(y + 2) * width + x];
			const Color pixel3 = pixels[(y + 3) * width + x];
			const Color pixel4 = pixels[(y + 4) * width + x];
			const Color pixel5 = pixels[(y + 5) * width + x];
			const Color pixel6 = pixels[(y + 6) * width + x];
			const Color pixel7 = pixels[(y + 7) * width + x];
			meanLuminance += luminance(pixel0);
			meanLuminance += luminance(pixel1);
			meanLuminance += luminance(pixel2);
			meanLuminance += luminance(pixel3);
			meanLuminance += luminance(pixel4);
			meanLuminance += luminance(pixel5);
			meanLuminance += luminance(pixel6);
			meanLuminance += luminance(pixel7);
		}
	}

	return meanLuminance / (width * height);
}

float MeanLuminance::calculateMeanLuminance(uint32_t width, uint32_t height, const Color* pixels)
{
	float meanLuminance = 0.0f;

	for (uint32_t x = 0u; x < width; ++x)
	{
		for (uint32_t y = 0u; y < height; y++)
		{
			const Color pixel0 = pixels[(y + 0) * width + x];
			meanLuminance += luminance(pixel0);
		}
	}

	return meanLuminance / (width * height);
}
