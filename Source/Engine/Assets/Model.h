// © 2019 Miguel Fernández Arce - All rights reserved

#pragma once

#include "CoreObject.h"
#include "Core/Assets/AssetData.h"


class Model : public AssetData
{
	CLASS(Model, AssetData)

public:

	virtual bool PostLoad() override;
};
