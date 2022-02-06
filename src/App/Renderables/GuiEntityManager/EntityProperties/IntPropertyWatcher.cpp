#include "IntPropertyWatcher.h"

#include <utility>

#include "imgui.h"

IntPropertyWatcher::
IntPropertyWatcher(std::string name, const int& value)
	: GuiProperty(std::move(name)), value(value)
{
}

void IntPropertyWatcher::display()
{
	ImGui::LabelText(name.c_str(), "%d", value);
}
