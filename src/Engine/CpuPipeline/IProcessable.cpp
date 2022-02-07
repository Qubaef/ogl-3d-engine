#include "IProcessable.h"

IProcessable::IProcessable(Engine& engine) :
	engine(engine)
{
	// If following methods are not overriden in child class,
	// proper bits will be set
	// This allows to check which methods are implemented
	definePreprocess();
	defineProcess();
	defineRender();
	// postprocess();
}

void IProcessable::definePreprocess()
{
	implementationStatus |= preprocessFlag;
}

bool IProcessable::ifDefinedPreprocess()
{
	return (implementationStatus & preprocessFlag);
}

void IProcessable::defineProcess()
{
	implementationStatus |= processFlag;
}

bool IProcessable::ifDefinedProcess()
{
	return (implementationStatus & processFlag);
}

void IProcessable::defineRender()
{
	implementationStatus |= renderFlag;
}

bool IProcessable::ifDefinedRender()
{
	return (implementationStatus & renderFlag);
}

//void IProcessable::postprocess()
//{
//	implementationStatus |= postprocessFlag;
//}
//
//bool IProcessable::ifDefinedPostprocess()
//{
//	return !(implementationStatus & (1 << 3));
//}
