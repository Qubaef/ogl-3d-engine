#pragma once
#include <exception>

class InitializationException : public std::exception
{
private:
	const char* func;

public:
	InitializationException(const char* msg, const char* func_);

	const char* get_func() const;
};
