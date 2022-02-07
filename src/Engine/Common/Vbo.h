#pragma once
#include "Engine/Include/Common.h"

struct Vbo
{
	unsigned id;

	Vbo()
	{
		glGenBuffers(1, &id);
	}

	
};

