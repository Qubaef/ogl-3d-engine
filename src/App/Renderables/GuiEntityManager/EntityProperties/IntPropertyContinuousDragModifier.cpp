#include "IntPropertyContinuousDragModifier.h"

#include "imgui.h"

IntPropertyContinuousDragModifier::
IntPropertyContinuousDragModifier(std::string name, int minValue, int maxValue, int& value)
	: GuiProperty(std::move(name)), minValue(minValue), maxValue(maxValue), value(value)
{
}

void IntPropertyContinuousDragModifier::display()
{
	ImGui::DragInt(name.c_str(), &value, 1, minValue, maxValue, "%d", ImGuiSliderFlags_AlwaysClamp);
}