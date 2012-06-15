/*
 * PerlinNoise.h
 *
 *  Created on: 11.05.2011
 *      Author: Simon
 */

#ifndef PERLINNOISE_H_
#define PERLINNOISE_H_

// Calculates a PerlinNoise
class PerlinNoise
{
	float m_Frequency;		// Number of values between each 2D Point
	float m_Persistence;	// Constant multiplier in each iteration
	int m_Octaves;			// Number of iteration
	float m_Amplitude;

	float m_CloudCoverage;
	float m_CloudDensity;

	int m_PreRenderSize;
	float *m_PreRender;

public:
	PerlinNoise();
	virtual ~PerlinNoise();

	// Prerenders the noise based on the given size parameter.
	void PreRender(int size);

	// Returns the value of the noise of the position
	// x and y, if forceCalc is set to true, no precalculated
	// value will be used.
	float Noise2D(int x, int y, bool forceCalc = false);

private:
	int r1, r2, r3;
	float Noise(int x, int y);
	float Interpolate(float x, float y, float a);
	float Smooth(float x, float y);
};

#endif /* PERLINNOISE_H_ */
