#include "pch.h"
#include "ShopScene.h"

ShopScene::ShopScene(SceneManager& manager) : Scene(manager)
{
}

ShopScene::~ShopScene()
{
}

void ShopScene::OnEnter()
{
}

void ShopScene::HandleEvent(const SDL_Event& e)
{
}

void ShopScene::OnExit()
{
}

void ShopScene::Update(const float& deltaTime)
{
}

void ShopScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderDebugText(&renderer, 64, 64, "SHOP SCENE");
}
