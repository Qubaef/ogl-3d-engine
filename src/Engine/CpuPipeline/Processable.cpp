#include "Processable.h"

Processable::Processable(Engine* enginePtr) :
	enginePtr(enginePtr)
{
	// If following methods are not overriden in child class,
	// proper bits will be set
	// This allows to check which methods are implemented
	definePreprocess();
	defineProcess();
	defineRender();
	// postprocess();
}

void Processable::definePreprocess()
{
	implementationStatus |= preprocessFlag;
}

bool Processable::ifDefinedPreprocess()
{
	return (implementationStatus & preprocessFlag);
}

void Processable::defineProcess()
{
	implementationStatus |= processFlag;
}

bool Processable::ifDefinedProcess()
{
	return (implementationStatus & processFlag);
}

void Processable::defineRender()
{
	implementationStatus |= renderFlag;
}

bool Processable::ifDefinedRender()
{
	return (implementationStatus & renderFlag);
}

//void Processable::postprocess()
//{
//	implementationStatus |= postprocessFlag;
//}
//
//bool Processable::ifDefinedPostprocess()
//{
//	return !(implementationStatus & (1 << 3));
//}
