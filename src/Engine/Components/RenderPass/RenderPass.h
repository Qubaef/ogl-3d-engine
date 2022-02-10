#pragma once

#include <vector>

class Engine;
class IProcessable;

//
// Abstract interface of render pass, which instance can be registered in an Engine
// and executed every frame with all engine's processables
//

class RenderPass
{
public:
	RenderPass(Engine& engine);

	void render() const;

	virtual void preRender() = 0;

	virtual void postRender() = 0;

	virtual ~RenderPass() = default;

protected:
	const std::vector<IProcessable*>& processableList;
	Engine& engine;
};
