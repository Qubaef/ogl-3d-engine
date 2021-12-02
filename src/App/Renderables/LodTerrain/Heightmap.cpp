#include "Heightmap.h"

#include "Engine/Engine.h"
#include "Engine/Logger/Logger.h"
#include "Engine/Include/Common.h"
#include "Engine/Common/ImageLibWrapper.h"

Heightmap::Heightmap(int size, int density)
	: size(size), density(density)
{
}

Heightmap::Heightmap(const char* filename) : size(size), density(density)
{
	int w, h;
	float* data = new float[density * density];
	data = (float*)ImageLibWrapper::image_load(filename, &w, &h, 0, 4);

	if (w != h)
	{
		LOG.ERROR("Unable to properly load heightmap.");
	}
}

void Heightmap::update()
{
	ZoneScoped;

	float* data = new float[density * density];

	for (int i = 0; i < density; i++)
	{
		for (int j = 0; j < density; j++)
		{
			data[(j + i * (density))] = 0;
		}
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, density, density);
	glTexImage2D(GL_TEXTURE_2D, 0, 0, 0, density, density, GL_RED, GL_FLOAT, data);

	delete[] data;
}

void Heightmap::update(SimplexNoiseWrapper noise)
{
	ZoneScoped;

	float* data = new float[density * density];

	for (int i = 0; i < density; i++)
	{
		for (int j = 0; j < density; j++)
		{
			data[(j + i * (density))] = noise.getSimplexNoiseValue(size / (float)density * i, size / (float)density * j);
		}
	}

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, density, density);
	glTexImage2D(GL_TEXTURE_2D, 0, 0, 0, density, density, GL_RED, GL_FLOAT, data);

	delete[] data;
}

void Heightmap::updateGpu(Engine* enginePtr)
{
	ZoneScoped;

	// Get and setup shader
	Shader* shaderPtr = enginePtr->getShaderByName("HeightmapShader");
	shaderPtr->use();
	shaderPtr->set_float("size", size);
	shaderPtr->set_float("density", density);
	shaderPtr->set_float("frequency", 2600);
	shaderPtr->set_float("amplitude", 2500);

	// Init texture
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	
	// Only Red channel 32 bit
	if(true)
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, density, density);
		glBindImageTexture(0, textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_R32F);

		// Launch compute shader
		LOG.INFO("Launching compute shader heightmap generation. Work groups: %d, %d, %d\n", density / workgroupDiv, density / workgroupDiv, 1);
		glDispatchCompute(density / workgroupDiv, density / workgroupDiv, 1);

		// Make sure writing to image has finished before read
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}
	else
	{
		// RGBA 8 bits per channel
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, density, density);
		glBindImageTexture(0, textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

		// Launch compute shader
		glDispatchCompute(density / 25, density / 25, 1);

		// Make sure writing to image has finished before read
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		//// Read texture from GPU
		//float* data = new float[density * density];
		//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, (void*)data);
	}
	
	// Download texture to validate data
	if (true)
	{
		// Read texture from GPU
		float* data = new float[density * density];
		
		if(true)
		{
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, (void*)data);
		}
		else
		{
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)data);
		}

		// Find min and max height
		float max = data[0];
		float min = data[0];
		for (int i = 0; i < density * density; i++)
		{
			max = glm::max(max, data[i]);
			min = glm::min(min, data[i]);
		}

		saveToFile(data);

		delete[] data;
	}
}

void Heightmap::saveToFile(float* data)
{
	ImageLibWrapper::image_write_tga(filename, density, density, 4, data);
}

void Heightmap::erode(int droplets)
{
}

void Heightmap::generateImage()
{
}

Heightmap::~Heightmap()
{
}