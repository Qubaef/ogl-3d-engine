#include "IntPropertyNotifyModifier.h"

#include "imgui.h"

IntPropertyNotifyModifier::
IntPropertyNotifyModifier(const char* name, int minValue, int maxValue, int defaultValue, int value)
	: GuiProperty(name), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void IntPropertyNotifyModifier::display()
{
	ImGui::DragInt(name, &value, 1, minValue, maxValue, "%d%", ImGuiSliderFlags_AlwaysClamp);
	if (ImGui::Button("Update"))
	{
		ImGui::SameLine();

		// TODO: Notify the entity manager that the property has changed
		ImGui::Text("Update sent!");
	}
}

void IntPropertyNotifyModifier::getMessage()
{
	// TODO:
}
