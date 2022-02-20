#pragma once

#include "GuiProperty.h"
#include <glm/vec3.hpp>

struct Vec3PropertyWatcher : GuiProperty
{
	const glm::vec3& value;

	Vec3PropertyWatcher(std::string name, const glm::vec3& value);

	void display() override;

	~Vec3PropertyWatcher() override = default;
};
