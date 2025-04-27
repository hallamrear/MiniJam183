#include "pch.h"
#include "MapScene.h"

MapScene::MapScene(SceneManager& manager) : Scene(manager)
{
}

MapScene::~MapScene()
{
}

void MapScene::OnEnter()
{
}

void MapScene::HandleEvent(const SDL_Event& e)
{
}

void MapScene::OnExit()
{
}

void MapScene::Update(const float& deltaTime)
{
}

void MapScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderDebugText(&renderer, 64, 64, "MAP SCENE");
}
