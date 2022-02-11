#pragma once

#include "Include.h"

inline void copyTextureArrayLayerToGL_TEXTURE_2D(unsigned targetTexture, unsigned textureArray, int layer, int width, int height) {
	glCopyImageSubData(
		textureArray, GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer,
		targetTexture, GL_TEXTURE_2D, 0, 0, 0, 0,
		width, height, 1
	);
}