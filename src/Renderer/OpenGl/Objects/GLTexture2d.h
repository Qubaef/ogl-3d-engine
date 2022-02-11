#pragma once

#include "Engine/Include/Common.h"

class GLTexture2d
{
public:
	GLTexture2d() = default;

	unsigned getId() const
	{
		return id;
	}

	void init(int internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type,
		GLint minFilter, GLint magFilter, GLint wrapMethodLint)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(
			GL_TEXTURE_2D,			// Target
			0,						// Level
			internalFormat,			// Internal format
			width,					// Width	
			height,					// Height
			0,						// Border
			format,					// Format		
			type,					// Type		
			nullptr					// Data		
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMethodLint);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMethodLint);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void setBorder(const float borderColor[4]) const
	{
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void setSwizzle(const GLint swizzleMask[4]) const
	{
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void bind(int bindingIndex) const
	{
		glActiveTexture(bindingIndex);
		glBindTexture(GL_TEXTURE_2D, id);
	}

private:
	unsigned id = 0;
};
