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
	int valueCopy = value;
	ImGui::DragInt(name.c_str(), &valueCopy,
		0, 0, 0,
		nullptr, ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_NoInput);
}
