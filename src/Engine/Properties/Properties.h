#pragma once

#include "PropertiesConst.h"
#include "PropertiesRender.h"
#include "PropertiesTime.h"

struct Properties
{
	PropertiesRender render;
	PropertiesTime time;
	PropertiesConst consts;
};
