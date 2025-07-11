#include "pch.h"
#include "RandomEventScene.h"
#include <System/Input.h>
#include <System/SceneManager.h>
#include <Graphics/Animation.h>
#include <System/Services.h>

RandomEventScene::RandomEventScene(SceneManager& manager)
	: Scene(manager)
{
	std::vector<AnimationDetails> details = 
	{ 
		{1, 0.0f, false, true},
		{1, 0.0f, false, true},
		{1, 0.0f, false, true}
	};

	m_RandomEventAtlas = new AnimationController("Content/Random Encounters/AtlasImage.png", 1, details);

	int width = 0;
	int height = 0;
	SDL_GetWindowSize(&Services::GetWindow(), &width, &height);
	m_RandomEventImageDstRect.w = m_RandomEventAtlas->GetFrameSizeX();
	m_RandomEventImageDstRect.h = m_RandomEventAtlas->GetFrameSizeX();
	m_RandomEventImageDstRect.x = (width / 2) - (m_RandomEventImageDstRect.w / 2);
	m_RandomEventImageDstRect.y = (height / 2) - (m_RandomEventImageDstRect.h / 2);
}

RandomEventScene::~RandomEventScene()
{
	if (m_RandomEventAtlas != nullptr)
	{
		delete m_RandomEventAtlas;
		m_RandomEventAtlas = nullptr;
	}
}

float sclickCooldown = 2.0f;

void RandomEventScene::OnEnter()
{
	sclickCooldown = 2.0f;

	GenerateRandomEncounter();
}

void RandomEventScene::GenerateRandomEncounter()
{
	int r = rand() % ENCOUNTER_TYPE::COUNT;

	m_GeneratedEncounter = (ENCOUNTER_TYPE)(r);

	switch (m_GeneratedEncounter)
	{
	case RandomEventScene::RANDOM_ENCOUNTER_NOTHING:
		SDL_Log("%i RANDOM_ENCOUNTER_NOTHING", r);
		break;
	case RandomEventScene::RANDOM_ENCOUNTER_FREE_FULL_HEAL:
		SDL_Log("%i RANDOM_ENCOUNTER_FREE_FULL_HEAL", r);
		break;
	case RandomEventScene::RANDOM_ENCOUNTER_FREE_CARM:
		SDL_Log("%i RANDOM_ENCOUNTER_FREE_CARM", r);
		break;
	case RandomEventScene::RANDOM_ENCOUNTER_RANDOM_FREE_HEAL:
		SDL_Log("%i RANDOM_ENCOUNTER_RANDOM_FREE_HEAL", r);
		break;

	default:
	case RandomEventScene::RANDOM_ENCOUNTER_UNKNOWN_OR_NOT_SET:
	case RandomEventScene::COUNT:
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Generated an invalid random event %i. :(", r);
		break;
	}

}

void RandomEventScene::DestroyRandomEncounter()
{
	switch (m_GeneratedEncounter)
	{
	default:
		break;
	}

	m_GeneratedEncounter = RandomEventScene::RANDOM_ENCOUNTER_UNKNOWN_OR_NOT_SET;
}

void RandomEventScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_KEY_DOWN:
	{
		switch (e.key.key)
		{
		case SDLK_1: { m_RandomEventAtlas->SetAnimationId(0); } break;
		case SDLK_2: { m_RandomEventAtlas->SetAnimationId(1); } break;
		case SDLK_3: { m_RandomEventAtlas->SetAnimationId(2); } break;

		default:
			break;
		}
	}
	break;

	default:
		break;
	}
}

void RandomEventScene::OnExit()
{
	DestroyRandomEncounter();
}

void RandomEventScene::Update(const float& deltaTime)
{
	if(sclickCooldown > 0.0f)
		sclickCooldown -= deltaTime;

	if (m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON) && sclickCooldown <= 0.0f)
	{
		m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAP);
	}

	if (m_RandomEventAtlas != nullptr)
	{
		m_RandomEventAtlas->Update(deltaTime);
	}
}

void RandomEventScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(&renderer, 10, 10, "SCENE_IDENTIFIER::SCENE_RANDOM_EVENT");
	SDL_RenderDebugText(&renderer, 256, 256, "This scene will give you a random event e.g. free cards, free heal, lose money etc.");

	if (m_RandomEventAtlas != nullptr)
	{
		m_RandomEventAtlas->Render(renderer, m_RandomEventImageDstRect, false);
	}
}
