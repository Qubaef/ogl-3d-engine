#include "Processable.h"

Processable::Processable(Engine* enginePtr) :
	enginePtr(enginePtr)
{
	// If following methods are not overriden in child class,
	// proper bits will be set
	// This allows to check which methods are implemented
	preprocess();
	process();
	render();
	// postprocess();
}

void Processable::preprocess()
{
	implementationStatus |= preprocessFlag;
}

bool Processable::ifDefinedPreprocess()
{
	return (implementationStatus & (1 << 0));
}

void Processable::process()
{
	implementationStatus |= processFlag;
}

bool Processable::ifDefinedProcess()
{
	return (implementationStatus & (1 << 1));
}

void Processable::render()
{
	implementationStatus |= renderFlag;
}

bool Processable::ifDefinedRender()
{
	return (implementationStatus & (1 << 2));
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
