#include "RenderPass.h"

#include "Engine/Engine.h"

RenderPass::RenderPass(Engine& engine) :
	processableList(engine.getProcessableList()),
	engine(engine)
{
}

void RenderPass::render() const
{
	//
	// Render all the processables
	//
	for (IProcessable* processablePtr : processableList)
	{
		processablePtr->render();
	}
}
