#include "pch.h"
#include "MenuScene.h"

MenuScene::MenuScene(SceneManager& manager) : Scene(manager)
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::OnEnter()
{
}

void MenuScene::HandleEvent(const SDL_Event& e)
{
}

void MenuScene::OnExit()
{
}

void MenuScene::Update(const float& deltaTime)
{
}

void MenuScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderDebugText(&renderer, 64, 64, "MENU SCENE");
}
