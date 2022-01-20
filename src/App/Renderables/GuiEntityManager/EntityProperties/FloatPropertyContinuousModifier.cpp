#include "FloatPropertyContinuousModifier.h"

#include "imgui.h"

FloatPropertyContinuousModifier::
FloatPropertyContinuousModifier(std::string name, float minValue, float maxValue, float defaultValue, float& value)
	: GuiProperty(std::move(name)), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void FloatPropertyContinuousModifier::display()
{
	ImGui::DragFloat(name.c_str(), &value, 1, minValue, maxValue, "%f");
}
