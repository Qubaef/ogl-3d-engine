#include "BoolPropertyTrueFalseSwitch.h"

#include "imgui.h"

BoolPropertyTrueFalseSwitch::
BoolPropertyTrueFalseSwitch(std::string name, bool defaultValue, bool& value)
	: GuiProperty(std::move(name)), defaultValue(defaultValue), value(value)
{
}

void BoolPropertyTrueFalseSwitch::display()
{
	ImGui::Checkbox(name.c_str(), &value);
}