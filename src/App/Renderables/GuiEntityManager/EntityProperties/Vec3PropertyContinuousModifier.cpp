#include "Vec3PropertyContinuousModifier.h"

#include "imgui.h"

Vec3PropertyContinuousModifier::
Vec3PropertyContinuousModifier(std::string name, float minValue, float maxValue, float defaultValue, glm::vec3& value)
	: GuiProperty(std::move(name)), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void Vec3PropertyContinuousModifier::display()
{
	ImGui::DragFloat3(name.c_str(), reinterpret_cast<float*>(&value), (maxValue - minValue) / 1000, minValue, maxValue,
		nullptr, ImGuiSliderFlags_AlwaysClamp);
}
