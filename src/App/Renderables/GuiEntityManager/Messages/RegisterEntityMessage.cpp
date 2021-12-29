#include "RegisterEntityMessage.h"

RegisterEntityMessage::RegisterEntityMessage(const char* parentPath)
	: parentPath(parentPath)
{
}

RegisterEntityMessage::~RegisterEntityMessage()
{
}

const char* RegisterEntityMessage::getParentPath() const
{
	return parentPath;
}
