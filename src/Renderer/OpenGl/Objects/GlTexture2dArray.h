#pragma once
#include "Engine/Include/Common.h"

class GLTexture2dArray
{
public:
	GLTexture2dArray() = default;

	unsigned getId() const
	{
		return id;
	}

	void init(int internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type,
		GLint minFilter, GLint magFilter, GLint wrapMethodLint)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);

		glTexImage3D(
			GL_TEXTURE_2D_ARRAY,	// Target
			0,						// Level
			internalFormat,			// Internal format
			width,					// Width	
			height,					// Height
			depth,					// Depth
			0,						// Border
			format,					// Format
			type,					// Type
			nullptr					// Data
		);

		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapMethodLint);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapMethodLint);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	void setBorder(const float borderColor[4]) const
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);
		glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	}

	void bind(int bindingIndex) const
	{
		glActiveTexture(bindingIndex);
		glBindTexture(GL_TEXTURE_2D_ARRAY, id);
	}

private:
	unsigned id = 0;
};


