#include "pch.h"
#include "RandomEventScene.h"
#include <System/Input.h>
#include <System/SceneManager.h>

RandomEventScene::RandomEventScene(SceneManager& manager)
	: Scene(manager)
{

}

RandomEventScene::~RandomEventScene()
{


}

float sclickCooldown = 2.0f;

void RandomEventScene::OnEnter()
{
	sclickCooldown = 2.0f;
}

void RandomEventScene::HandleEvent(const SDL_Event& e)
{

}

void RandomEventScene::OnExit()
{

}

void RandomEventScene::Update(const float& deltaTime)
{
	if(sclickCooldown > 0.0f)
		sclickCooldown -= deltaTime;

	if (m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON) && sclickCooldown <= 0.0f)
	{
		m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAP);
	}
}

void RandomEventScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(&renderer, 10, 10, "SCENE_IDENTIFIER::SCENE_RANDOM_EVENT");
	SDL_RenderDebugText(&renderer, 256, 256, "This scene will give you a random event e.g. free cards, free heal, lose money etc.");
}
