// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include "BaseObject.h"
#include "ObjectPtr.h"
#include "../Reflection/TClass.h"
#include "../Serialization/Archive.h"


class Object;
class World;


template<class ObjectType>
static GlobalPtr<ObjectType> Create(Class* objectClass, const Ptr<Object> owner = {}) {
	static_assert(eastl::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

	if (objectClass)
	{
		return objectClass->CreateInstance(owner).Cast<ObjectType>();
	}
	return {};
}

template<class ObjectType>
static GlobalPtr<ObjectType> Create(const Ptr<BaseObject> owner = {}) {
	static_assert(eastl::is_convertible< ObjectType, Object >::value, "Type is not an Object!");

	return GlobalPtr<ObjectType>::Create(owner);
}


class Object : public BaseObject {
	ORPHAN_CLASS(Object)

private:

	PROP(Name, name)
	Name name;

	Ptr<BaseObject> self;
	Class* ownClass;
	Ptr<BaseObject> owner;


public:

	Object() : BaseObject(), self{}, ownClass{ nullptr }, owner{} {};

	void PreConstruct(Ptr<BaseObject>&& inSelf, Class* inClass, const Ptr<BaseObject>& inOwner) {
		ownClass = inClass;
		self = inSelf;
		owner = inOwner;
	}
	virtual void Construct() {}

	virtual bool Serialize(Archive& ar) {
		SerializeReflection(ar);
		return true;
	}


	Ptr<Object> GetOwner() const { return owner.Cast<Object>(); }

	Ptr<Object> GetSelf() const { return self.Cast<Object>(); }

	Class* GetClass() const { return ownClass; }


	void SetName(Name newName) { name = eastl::move(newName); }
	Name GetName() { return name; }

	virtual Ptr<World> GetWorld() const;
};
