#include "InitializationException.h"

InitializationException::InitializationException(const char* msg, const char* func_) :
	std::exception(msg),
	func(func_)
{
}


const char* InitializationException::get_func() const
{
	return func;
}

const char* InitializationException::get_msg() const
{
	return msg;
}

const char* InitializationException::get_type() const
{
	return type;
}
