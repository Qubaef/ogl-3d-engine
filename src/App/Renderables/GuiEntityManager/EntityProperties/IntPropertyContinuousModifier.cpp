#include "IntPropertyContinuousModifier.h"

#include "imgui.h"

IntPropertyContinuousModifier::
IntPropertyContinuousModifier(const char* name, int minValue, int maxValue, int defaultValue, int& value)
	: GuiProperty(name), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void IntPropertyContinuousModifier::display()
{
	ImGui::DragInt(name, &value, 1, minValue, maxValue, "%d", ImGuiSliderFlags_AlwaysClamp);
}

void IntPropertyContinuousModifier::getMessage()
{
}
