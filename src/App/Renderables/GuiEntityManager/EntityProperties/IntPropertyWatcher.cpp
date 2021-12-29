#include "IntPropertyWatcher.h"

#include "imgui.h"

IntPropertyWatcher::
IntPropertyWatcher(const char* name, const int& value)
	: GuiProperty(name), value(value)
{
}

void IntPropertyWatcher::display()
{
	ImGui::LabelText(name, "%d", value);
}

void IntPropertyWatcher::getMessage()
{
}
