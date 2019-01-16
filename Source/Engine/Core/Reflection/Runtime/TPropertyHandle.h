// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <vector>
#include <functional>

#include "PropertyHandle.h"
#include "Core/Reflection//Property.h"


/**
 * Base logic implementation for templated property handles.
 * Use TPropertyHandle instead
 */
template<typename VarType>
struct TPropertyHandle : public PropertyHandle
{
	using Access = std::function<VarType*(BaseObject*)>;

	Access access;


public:
	TPropertyHandle() : PropertyHandle({}, nullptr) {}
	TPropertyHandle(const Ptr<BaseObject>& instance, const Property* prop, const Access& access)
		: PropertyHandle(instance, prop), access{access}
	{}

	NOINLINE VarType* GetValuePtr() const
	{
		if (IsValid())
		{
			return access(*instance);
		}
		return nullptr;
	}

	bool GetValue(VarType& value) const
	{
		VarType* const valuePtr = GetValuePtr();
		if (valuePtr)
			value = *valuePtr;

		return valuePtr;
	}

	bool SetValue(const VarType& value) const
	{
		VarType* const valuePtr = GetValuePtr();
		if (valuePtr)
			*valuePtr = value;

		return valuePtr;
	}
};
