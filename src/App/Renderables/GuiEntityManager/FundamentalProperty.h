#pragma once
#include <crtdbg.h>

#include "Property.h"

template <typename T>
struct FundamentalProperty : Property
{
	T minValue;
	T maxValue;
	T defaultValue;
	T value;

	FundamentalProperty(const char* name, T minValue, T maxValue, T defaultValue, T value)
		: Property(name), minValue(minValue), maxValue(maxValue), defaultValue(defaultValue), value(value)
	{
		if (!std::is_fundamental_v<T>) {
			// Property has to be fundamental type
			_ASSERT(false);
		}
	}

	/*FundamentalProperty(const FundamentalProperty& other)
		: Property(other),
		minValue(other.minValue),
		maxValue(other.maxValue),
		defaultValue(other.defaultValue),
		value(other.value)
	{
	}

	FundamentalProperty(FundamentalProperty&& other) noexcept
		: Property(other),
		minValue(other.minValue),
		maxValue(other.maxValue),
		defaultValue(other.defaultValue),
		value(other.value)
	{
	}

	FundamentalProperty& operator=(const FundamentalProperty& other)
	{
		if (this == &other)
			return *this;
		Property::operator =(other);
		minValue = other.minValue;
		maxValue = other.maxValue;
		defaultValue = other.defaultValue;
		value = other.value;
		return *this;
	}

	FundamentalProperty& operator=(FundamentalProperty&& other) noexcept
	{
		if (this == &other)
			return *this;
		Property::operator =(other);
		minValue = other.minValue;
		maxValue = other.maxValue;
		defaultValue = other.defaultValue;
		value = other.value;
		return *this;
	}*/

	void display() override
	{
		ImGui::Text("%s", name);
		ImGui::SameLine();
		ImGui::Text("%d", value);
		ImGui::Text("%d", maxValue);
		ImGui::Text("%d", minValue);
	}

	~FundamentalProperty() override
	{
	}
};
