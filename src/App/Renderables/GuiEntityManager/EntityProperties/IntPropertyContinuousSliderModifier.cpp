#include "IntPropertyContinuousSliderModifier.h"

#include "imgui.h"

IntPropertyContinuousSliderModifier::
IntPropertyContinuousSliderModifier(std::string name, int minValue, int maxValue, int defaultValue, int& value)
	: GuiProperty(std::move(name)), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void IntPropertyContinuousSliderModifier::display()
{
	ImGui::SliderInt(name.c_str(), &value, minValue, maxValue, "%d", ImGuiSliderFlags_AlwaysClamp);
}