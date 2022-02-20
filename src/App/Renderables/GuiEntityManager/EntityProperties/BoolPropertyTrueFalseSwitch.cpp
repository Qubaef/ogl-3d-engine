#include "BoolPropertyTrueFalseSwitch.h"

#include "imgui.h"

BoolPropertyTrueFalseSwitch::BoolPropertyTrueFalseSwitch(std::string name, bool& value) :
	GuiProperty(std::move(name)), value(value)
{
}

void BoolPropertyTrueFalseSwitch::display()
{
	ImGui::Checkbox(name.c_str(), &value);
}
