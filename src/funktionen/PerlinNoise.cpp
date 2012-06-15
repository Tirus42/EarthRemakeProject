/*
 * PerlinNoise.cpp
 *
 *  Created on: 11.05.2011
 *      Author: Simon
 */
#include <math.h>
#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
	m_Frequency = 0.0175f;
	m_Persistence = 0.4f;
	m_Octaves = 8;
	m_Amplitude = 1.0f;

	m_CloudCoverage = 0.3f;
	m_CloudDensity = 1.0f;

	m_PreRenderSize = 0;
	m_PreRender = 0;

	r1 = 15731;
	r2 = 789221;
	r3 = 1376312589;
}

PerlinNoise::~PerlinNoise()
{
	if (m_PreRender) delete m_PreRender;
}

void PerlinNoise::PreRender(int size)
{
	m_PreRenderSize = size;
	m_PreRender = new float[size * size];

	for (int y = 0; y < size; ++y)
		for (int x = 0; x < size; ++x)
			m_PreRender[x + m_PreRenderSize * y] = Noise2D(x, y, true);
}

float PerlinNoise::Noise2D(int x, int y, bool forceCalc)
{
	if (!forceCalc && m_PreRender)
	{
		if (x >= 0 && x < m_PreRenderSize && y >= 0 && y < m_PreRenderSize)
			return m_PreRender[x + m_PreRenderSize * y];
		else
			return 0.0f;
	}

	float val = 0.0f;

	float frequency = m_Frequency;
	float amplitude = m_Amplitude;

	for (int i = 0; i < m_Octaves; ++i)
	{
		val += Smooth(x * frequency, y * frequency) * amplitude;
		frequency *= 2;
		amplitude *= m_Persistence;
	}

	val = (val + m_CloudCoverage) * m_CloudDensity;

	if (val <-1.0f) val =-1.0f;
	if (val > 1.0f) val = 1.0f;

	return val;
}

float PerlinNoise::Noise(int x, int y)
{
	int n = x + y * 57;
	n = (n << 13) ^ n;

	return (float)(1.0 - ((n * (n * n * r1 + r2) + r3) & 0x7fffffff) / 1073741824.0);
}

float PerlinNoise::Interpolate(float x, float y, float a)
{
	float val = (1.0f - cosf(a * 3.14159f)) * 0.5f;
	return x * (1.0f - val) + y * val;
}

float PerlinNoise::Smooth(float x, float y)
{
	float n1 = Noise((int)x    , (int)y    );
	float n2 = Noise((int)x + 1, (int)y    );
	float n3 = Noise((int)x    , (int)y + 1);
	float n4 = Noise((int)x + 1, (int)y + 1);

	float i1 = Interpolate(n1, n2, x - (int)x);
	float i2 = Interpolate(n3, n4, x - (int)x);

	return Interpolate(i1, i2, y - (int)y);
}
