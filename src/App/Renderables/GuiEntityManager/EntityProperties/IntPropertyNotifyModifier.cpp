#include "IntPropertyNotifyModifier.h"

#include "imgui.h"
#include "App/Renderables/GuiEntityManager/Messages/OnPropertyChangeMessage.h"

IntPropertyNotifyModifier::
IntPropertyNotifyModifier(const char* recipientName, const char* name, int minValue, int maxValue, int defaultValue, int value)
	: GuiProperty(name), recipientName(recipientName), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void IntPropertyNotifyModifier::display()
{
	ImGui::DragInt(name, &value, 1, minValue, maxValue, "%d%", ImGuiSliderFlags_AlwaysClamp);
	if (ImGui::Button("Update"))
	{
		ImGui::SameLine();

		sendMessage(new OnPropertyChangeMessage(name, value),
			recipientName);
		ImGui::Text("Update sent!");
	}
}