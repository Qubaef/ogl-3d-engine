#include "Vec3PropertyWatcher.h"

#include <glm/common.hpp>

#include "imgui.h"

Vec3PropertyWatcher::
Vec3PropertyWatcher(std::string name, const glm::vec3& value)
	: GuiProperty(std::move(name)), value(value)
{
}

void Vec3PropertyWatcher::display()
{
	glm::vec3 valueCopy = value;
	ImGui::DragFloat3(name.c_str(), reinterpret_cast<float*>(&valueCopy),
		0, 0, 0,
		nullptr, ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoInput);
}
