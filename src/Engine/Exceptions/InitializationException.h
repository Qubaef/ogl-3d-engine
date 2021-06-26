#pragma once
#include <exception>

class InitializationException : public std::exception
{
private:
	const char* type = "Error while initializing program";
	const char* func;
	const char* msg;

public:
	InitializationException(const char* msg, const char* func_);

	const char* get_func() const;
	const char* get_msg() const;
	const char* get_type() const;
};
