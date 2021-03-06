// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "CoreEngine.h"
#include <EASTL/type_traits.h>

#include "BaseStruct.h"
#include "EngineTypes.h"
#include "Core/Reflection/Reflection.h"
#include "Core/Memory/Allocator.h"


class Class;
class Object;

template<typename Type>
class Ptr;


class BaseObject : public BaseStruct {
protected:
	BaseObject() : BaseStruct() {}

public:

	void Destroy() { BeforeDestroy(); }

	Class* GetClass() const;
	Ptr<Object> GetSelf() const;

protected:

	virtual void BeforeDestroy() {}
};
