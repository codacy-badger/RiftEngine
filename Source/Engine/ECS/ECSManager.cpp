// Copyright 2015-2019 Piperift - All rights reserved

#include "ECSManager.h"

#include "Core/Serialization/Archive.h"

#include "Gameplay/Components/CEntity.h"
#include "Gameplay/Components/CTransform.h"
#include "Gameplay/Components/CMesh.h"
#include "Gameplay/Components/CCamera.h"
#include "Gameplay/Systems/SEditorCamera.h"
#include "Gameplay/Systems/SRenderMesh.h"
#include "Gameplay/Systems/SRenderCamera.h"
#include "Gameplay/Singletons/CGlobalCamera.h"


bool ECSManager::Serialize(Archive& ar)
{
	bool bResult = Super::Serialize(ar);

	if (ar.IsSaving())
	{
		Archive* arPtr = &ar;
		registry.each([this, arPtr](EntityId entity) {
			SerializeEntity(*arPtr, entity);
		});
	}
	else
	{
		registry.reset();
	}

	return bResult;
}

void ECSManager::SerializeEntity(Archive& ar, const EntityId& entity)
{
	ar.BeginObject("components");
	{
		SerializeComponent<CEntity>(ar, entity);
		SerializeComponent<CTransform>(ar, entity);
		SerializeComponent<CMesh>(ar, entity);
		SerializeComponent<CCamera>(ar, entity);
	}
	ar.EndObject();
}

void ECSManager::RegistrySystems()
{
	// #TODO: Externalize system registry
	RegistrySystem<SEditorCamera>();
	RegistrySystem<SRenderMesh>();
	RegistrySystem<SRenderCamera>();
}

void ECSManager::RegistrySingletons()
{
	RegistrySingleton<CGlobalCamera>();
}
