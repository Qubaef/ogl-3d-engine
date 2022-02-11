#pragma once
#include "Engine/Include/Common.h"

class GLFramebuffer
{
public:
	GLFramebuffer() = default;

	unsigned getId() const
	{
		return id;
	}

	void init()
	{
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void attachTexture(GLenum attachment, GLuint textureId) const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, textureId, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void setDrawBuffer(GLenum bufferType) const
	{
		glNamedFramebufferDrawBuffer(id, bufferType);
	}

	void setReadBuffer(GLenum bufferType) const
	{
		glNamedFramebufferReadBuffer(id, bufferType);
	}

	bool validate() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			LOG.ERROR("Framebuffer is not complete!\n");
			return false;
		}

		return true;
	}

	void bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	static void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

private:
	unsigned id = 0;
};
