#include "IntPropertyNotifyModifier.h"

#include "imgui.h"
#include "App/Renderables/GuiEntityManager/Messages/OnPropertyChangeMessage.h"

IntPropertyNotifyModifier::
IntPropertyNotifyModifier(const char* recipientName, std::string name, int minValue, int maxValue, int defaultValue, int value)
	: GuiProperty(std::move(name)), recipientName(recipientName), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
{
}

void IntPropertyNotifyModifier::display()
{
	ImGui::DragInt(name.c_str(), &value, 1, minValue, maxValue, "%d", ImGuiSliderFlags_AlwaysClamp);
	if (ImGui::Button("Update"))
	{
		sendMessage(new OnPropertyChangeMessage(name.c_str(), value), recipientName);
	}
}