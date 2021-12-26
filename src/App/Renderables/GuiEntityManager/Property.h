#pragma once

struct Property
{
	const char* name;

	Property(const char* name)
		: name(name)
	{
	}

	virtual void display() = 0;

	virtual ~Property() = default;	
};
