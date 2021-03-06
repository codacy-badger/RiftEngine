// Copyright 2015-2019 Piperift - All rights reserved

#include "UIManager.h"
#include "imgui/imgui_impl_sdl.h"


void UIManager::Prepare()
{
	// Setup style
	ImGui::StyleColorsDark();

#if WITH_EDITOR
	editor = Create<EditorManager>(GetSelf());
#endif
}

void UIManager::OnSDLEvent(SDL_Event* ev)
{
	ImGui_ImplSDL2_ProcessEvent(ev);
}

void UIManager::Tick(float deltaTime)
{
	ZoneScopedNC("UI", 0x459bd1);
	ImGui::NewFrame();

#if WITH_EDITOR
	editor->Tick(deltaTime);
#endif
}
