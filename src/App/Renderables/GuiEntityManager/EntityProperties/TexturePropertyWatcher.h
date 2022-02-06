#pragma once

#include "GuiProperty.h"

struct TexturePropertyWatcher : GuiProperty
{
	const unsigned textureId;
	const int textureW;
	const int textureH;
	int displayW;
	int displayH;

	void display() override;
	TexturePropertyWatcher(std::string name, unsigned textureId, int textureW, int textureH, int displayW, int displayH);
	~TexturePropertyWatcher() override = default;
};
