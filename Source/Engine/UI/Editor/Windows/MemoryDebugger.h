// Copyright 2015-2019 Piperift - All rights reserved

#pragma once

#if WITH_EDITOR

#include "UI/Editor/EditorWindow.h"


class MemoryDebugger : public EditorWindow {
	CLASS(MemoryDebugger, EditorWindow)

protected:

	virtual void Build() override;

	virtual void Tick() override;
};

#endif
