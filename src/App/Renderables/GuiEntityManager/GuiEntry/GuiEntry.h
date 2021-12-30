#pragma once
#include <vector>
#include <string>

#include "App/Renderables/GuiEntityManager/IDisplayable.h"
#include "EntryType.h"


struct GuiEntry : public IDisplayable
{
private:
	static const char PATH_SPLITTER = '/';
	std::string name;
	std::vector<IDisplayable*> childrenList;
	ENTRY_TYPE type;

	// Get path and split it into name of the next child, and the rest of the path
	// Returns "", if no more children
	std::pair<std::string, std::string> getNextChild(const std::string& path);
public:

	GuiEntry(std::string name, ENTRY_TYPE type = REGULAR);

	// Display entry and all its children
	void display() override;

	void addChild(IDisplayable* displayable);

	// Create branch for given path
	// If child with expected name exists, create branch inside it
	// If it doesn't, create new entry and branch inside it
	GuiEntry& createBranch(std::string path);

	// Find entry with given path
	// If entry with given path exists, return it
	// If it doesn't, return null
	GuiEntry* findEntry(std::string path);
};
