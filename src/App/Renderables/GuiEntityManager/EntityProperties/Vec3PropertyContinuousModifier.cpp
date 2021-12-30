#include "Vec3PropertyContinuousModifier.h"

#include "imgui.h"

Vec3PropertyContinuousModifier::
Vec3PropertyContinuousModifier(const char* name, float minValue, float maxValue, float defaultValue, glm::vec3& value)
	: GuiProperty(name), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void Vec3PropertyContinuousModifier::display()
{
	ImGui::DragFloat3(name, reinterpret_cast<float*>(&value), 0.5f, minValue, maxValue,
		nullptr, ImGuiSliderFlags_AlwaysClamp);
}
