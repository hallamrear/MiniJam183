#include "pch.h"
#include "RestScene.h"
#include <System/Input.h>
#include <System/SceneManager.h>

RestScene::RestScene(SceneManager& manager) : Scene(manager)
{

}

RestScene::~RestScene()
{

}

float clickCooldown = 2.0f;

void RestScene::OnEnter()
{
	clickCooldown = 2.0f;
}

void RestScene::HandleEvent(const SDL_Event& e)
{
	
}

void RestScene::OnExit()
{

}

void RestScene::Update(const float& deltaTime)
{
	if (clickCooldown > 0.0f)
		clickCooldown -= deltaTime;

	if (m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON) && clickCooldown <= 0.0f)
	{
		m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAP);
	}
}

void RestScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(&renderer, 10, 10, "SCENE_IDENTIFIER::SCENE_REST");
	SDL_RenderDebugText(&renderer, 256, 256, "This scene will give you the option to rest and heal for free.");
}
