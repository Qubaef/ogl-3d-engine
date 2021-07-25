#include "../../../tracy/Tracy.hpp"
#include "InitializationException.h"

InitializationException::InitializationException(const char* msg, const char* func_) :
	std::exception(msg),
	func(func_)
{
	ZoneScoped;
}


const char* InitializationException::get_func() const
{
	ZoneScoped;
	return func;
}

const char* InitializationException::get_msg() const
{
	ZoneScoped;
	return msg;
}

const char* InitializationException::get_type() const
{
	ZoneScoped;
	return type;
}

