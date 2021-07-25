#pragma once
#include "../Include/Common.h"

struct Vbo
{
	unsigned id;

	Vbo()
	{
		glGenBuffers(1, &id);
	}

	
};

