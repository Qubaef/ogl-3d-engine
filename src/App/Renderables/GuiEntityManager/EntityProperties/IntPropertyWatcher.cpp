#include "IntPropertyWatcher.h"

#include "imgui.h"

IntPropertyWatcher::
IntPropertyWatcher(std::string name, const int& value)
	: GuiProperty(name), value(value)
{
}

void IntPropertyWatcher::display()
{
	ImGui::LabelText(name.c_str(), "%d", value);
}
