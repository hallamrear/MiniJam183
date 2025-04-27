#include "pch.h"
#include "TestScene.h"

TestScene::TestScene(SceneManager& manager) : Scene(manager)
{

}

TestScene::~TestScene()
{

}

void TestScene::OnEnter()
{

}

void TestScene::HandleEvent(const SDL_Event& e)
{
}

void TestScene::OnExit()
{

}

void TestScene::Update(const float& deltaTime)
{

}

void TestScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderDebugText(&renderer, 64, 64, "TEST SCENE");
}
