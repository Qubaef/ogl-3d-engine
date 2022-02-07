#pragma once

#include "SimplexNoise.h"

struct SimplexNoiseWrapper
{
	float amplitude = 150.0f;
	float frequency = 0.0018f;
	float lacunarity = 3.0f;
	float persistance = 0.25f;
	int terrainLayers = 5;

	float noiseOffsetX = 50.f;
	float noiseOffsetZ = 50.f;

	float getSimplexNoiseValue(float x, float z)
	{
		return amplitude + amplitude * SimplexNoise(frequency, amplitude, lacunarity, persistance).fractal(terrainLayers, noiseOffsetX + x, noiseOffsetZ + z);
	}

	bool operator== (const SimplexNoiseWrapper& s) const
	{
		return (amplitude == s.amplitude) &&
			(frequency == s.frequency) &&
			(lacunarity == s.lacunarity) &&
			(persistance == s.persistance) &&
			(terrainLayers == s.terrainLayers) &&
			(noiseOffsetX == s.noiseOffsetX) &&
			(noiseOffsetZ == s.noiseOffsetZ);
	}
};
