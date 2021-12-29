#include "GuiEntry.h"

#include "imgui.h"

std::pair<std::string, std::string> GuiEntry::getNextChild(const std::string& path)
{
	// Find position of first PATH_SPLITTER
	size_t pos = path.find(PATH_SPLITTER);

	// If no more PATH_SPLITTER, return whole path as name and "" as rest of the path
	if (pos == std::string::npos)
	{
		return std::make_pair(path, "");
	}

	// Get the next child by splitting the path at the next '/'
	std::string name = path.substr(0, pos);

	// Get the rest of the path by removing the first child from the path
	std::string rest = path.substr(pos + 1);

	return std::make_pair(name, rest);
}

GuiEntry::GuiEntry(std::string name, ENTRY_TYPE type)
	: name(std::move(name)), type(type)
{
}

void GuiEntry::display()
{
	switch(type)
	{
	case ROOT:
		if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (IDisplayable* child : childrenList)
			{
				child->display();
				ImGui::Separator();
			}
		}
		break;
	case HEADER:
		if (ImGui::TreeNode(name.c_str()), ImGuiTreeNodeFlags_DefaultOpen)
		{
			for (IDisplayable* child : childrenList)
			{
				child->display();
			}
			ImGui::Separator();
			ImGui::TreePop();
		}
		break;
	case REGULAR:
		if (ImGui::TreeNode(name.c_str()))
		{
			for (IDisplayable* child : childrenList)
			{
				child->display();
			}
			ImGui::Separator();
			ImGui::TreePop();
		}
		break;
	default:
		break;
	}

}

void GuiEntry::addChild(IDisplayable *displayable)
{
	childrenList.push_back(displayable);
}

GuiEntry &GuiEntry::createBranch(const std::string path)
{
	// Process path
	const auto nextChildData = getNextChild(path);

	// Unpack next child data
	std::string entryName = nextChildData.first;
	std::string newPath = nextChildData.second;

	// If branch is finished (if entryName is empty), return this as the
	// lowest entry from the branch
	if (entryName.empty())
	{
		return *this;
	}

	// Find child with expected name
	for (IDisplayable *child : childrenList)
	{
		if(GuiEntry *guiEntry = dynamic_cast<GuiEntry *>(child))
		{
			if (guiEntry->name == entryName)
			{
				// If child with expected name exists, create branch relative to it
				return guiEntry->createBranch(newPath);
			}
		}
	}

	// If child with expected name doesn't exist, create new entry and branch relative to it
	GuiEntry *newEntry = new GuiEntry(entryName);
	addChild(newEntry);
	return newEntry->createBranch(newPath);
}

GuiEntry *GuiEntry::findEntry(const std::string path)
{
	// Process path
	const auto nextChildData = getNextChild(path);

	// Unpack next child data
	const std::string entryName = nextChildData.first;
	const std::string newPath = nextChildData.second;

	// If branch is finished (if entryName is empty), return this as the
	// lowest entry from the branch
	if (entryName.empty())
	{
		return this;
	}

	// Find child with expected name
	for (IDisplayable *child : childrenList)
	{
		if(GuiEntry *guiEntry = dynamic_cast<GuiEntry *>(child))
		{
			if (guiEntry->name == entryName)
			{
				// If child with expected name exists, create branch relative to it
				return guiEntry->findEntry(newPath);
			}
		}
	}

	return nullptr;
}
