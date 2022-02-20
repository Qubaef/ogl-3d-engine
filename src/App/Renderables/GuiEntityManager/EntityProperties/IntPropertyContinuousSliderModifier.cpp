#include "IntPropertyContinuousSliderModifier.h"

#include "imgui.h"

IntPropertyContinuousSliderModifier::
IntPropertyContinuousSliderModifier(std::string name, int minValue, int maxValue, int& value)
	: GuiProperty(std::move(name)), minValue(minValue), maxValue(maxValue), value(value)
{
}

void IntPropertyContinuousSliderModifier::display()
{
	ImGui::SliderInt(name.c_str(), &value, minValue, maxValue, "%d", ImGuiSliderFlags_AlwaysClamp);
}