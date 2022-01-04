#include "IntPropertyContinuousModifier.h"

#include "imgui.h"

IntPropertyContinuousModifier::
IntPropertyContinuousModifier(std::string name, int minValue, int maxValue, int defaultValue, int& value)
	: GuiProperty(std::move(name)), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void IntPropertyContinuousModifier::display()
{
	ImGui::DragInt(name.c_str(), &value, 1, minValue, maxValue, "%d", ImGuiSliderFlags_AlwaysClamp);
}