#pragma once
#include "Engine/Include/Common.h"

class Ubo
{
public:
	Ubo() = default;

	unsigned getId() const
	{
		return id;
	}

	void init(int dataSize)
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferData(GL_UNIFORM_BUFFER, dataSize, NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void setData(int offset, int dataSize, void* data) const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, dataSize, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void bind(int bindingIndex) const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, id);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, id);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

private:
	unsigned id = 0;
};
