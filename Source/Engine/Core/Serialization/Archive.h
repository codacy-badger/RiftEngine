// Copyright 2015-2019 Piperift - All rights reserved
#pragma once

#include <EASTL/stack.h>
#include <nlohmann/json.hpp>

#include "Core/TypeTraits.h"
#include "Core/Object/ObjectPtr.h"

#include "MemoryReader.h"
#include "MemoryWriter.h"
#include <fstream>

using json = nlohmann::json;


class Archive {

	const bool bReads = false;

public:

	Archive() : bReads(false) {}
	Archive(bool bReads) : bReads(bReads) {}
	virtual ~Archive() = default;


	template<typename T>
	FORCEINLINE Archive& operator()(const char* name, T& val) {
		CustomSerializeOrNot(*this, name, val);
		return *this;
	}

	virtual void Serialize(const char* name, uint8& val) = 0;

	virtual void Serialize(const char* name, int32& val) = 0;

	virtual void Serialize(const char* name, uint32& val) = 0;

	virtual void Serialize(const char* name, float& val) = 0;

	virtual void Serialize(const char* name, String& val) = 0;

	template<typename T>
	void Serialize(const char* name, GlobalPtr<T>& val) {
		BeginObject(name);
		/*if (IsLoading())
		{
			// #TODO: Create Object
			val->Serialize(*this);
		}

		if (val)
		{
			val->Serialize(*this);
		}*/
		EndObject();
	}

	template<typename T>
	void Serialize(const char* name, Ptr<T>& val) {
		Name ptrName;
		if (IsSaving())
			ptrName = val ? val->GetName() : TX("None");

		Serialize(name, ptrName);

		/*if(IsLoading())
			Find and Assign object */
	}

	template<typename T>
	void Serialize(const char* name, eastl::vector<T>& val)
	{
		BeginObject(name);
		if (IsLoading())
		{
			// Ideally should read size form json array
			Serialize(TX("num"), val.size());
			// Read each Item
		}
		else
		{
			int32 num;
			Serialize(TX("num"), num);
			val.reserve(num);
			// Save Items
		}
		EndObject();
	}

	virtual void BeginObject(const char* name) = 0;
	virtual void EndObject() = 0;

	FORCEINLINE bool IsLoading() { return bReads; }
	FORCEINLINE bool IsSaving() { return !bReads; }

private:
	/**
	 * Selection of Serialize call.
	 */
	template<class T>
	FORCEINLINE typename eastl::enable_if_t<!ClassTraits<T>::HasCustomSerialize, bool> CustomSerializeOrNot(Archive& ar, const char* name, T& val)
	{
		ar.Serialize(name, val);
		return true;
	}

	template<class T>
	FORCEINLINE typename eastl::enable_if_t<ClassTraits<T>::HasCustomSerialize, bool> CustomSerializeOrNot(Archive& ar, const char* name, T& val)
	{
		return val.Serialize(ar, name);
	}
};


class JsonArchive : public Archive {

	json baseData;
	eastl::stack<json*> depthData;


public:

	JsonArchive() : Archive(), baseData{}, depthData{} {}
	virtual ~JsonArchive() = default;

	String GetDataString() { return baseData.dump(); }

private:

	virtual void BeginObject(const char* name) override {
		depthData.push(&Data()[name]);
	}

	virtual void EndObject() override {
		depthData.pop();
	}


	virtual void Serialize(const char* name, uint8& val) override {
		if (IsLoading())
			val = Data()[name].get<uint8>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(const char* name, int32& val) override {
		if (IsLoading())
			val = Data()[name].get<int32>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(const char* name, uint32& val) override {
		if (IsLoading())
			val = Data()[name].get<uint32>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(const char* name, float& val) override {
		if (IsLoading())
			val = Data()[name].get<float>();
		else
			Data()[name] = val;
	}

	virtual void Serialize(const char* name, String& val) override {
		if (IsLoading())
			val = Data()[name].get<String>();
		else
			Data()[name] = val;
	}

	json& Data() {
		if(depthData.size() > 0)
			return *depthData.top();
		return baseData;
	}
};
