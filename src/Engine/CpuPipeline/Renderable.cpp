#include "Renderable.h"
#include "../Engine.h"

Renderable::Renderable(Engine* enginePtr) :
	enginePtr(enginePtr)
{
	ZoneScoped;
}

void Renderable::processPerFrame()
{
	ZoneScopedN("Renderable - ProcessPerFrame");
	
	enginePtr->registerRenderTask(this);
}
