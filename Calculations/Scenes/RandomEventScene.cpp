#include "pch.h"
#include "RandomEventScene.h"
#include <System/Input.h>
#include <System/SceneManager.h>
#include <Graphics/Animation.h>
#include <System/Services.h>
#include <System/Collision.h>
#include <Gameplay/Player/Player.h>

RandomEventScene::RandomEventScene(SceneManager& manager)
	: Scene(manager)
{
	std::vector<AnimationDetails> details = 
	{ 
		{ 2, 0.0f, false, true },
		{ 2, 0.0f, false, true },
		{ 2, 0.0f, false, true },
		{ 2, 0.0f, false, true },
	};

	m_GeneratedEncounter = RandomEventScene::RANDOM_ENCOUNTER_UNKNOWN_OR_NOT_SET;
	m_RandomEventAtlas = new AnimationController("Content\\Random Encounters\\AtlasImage.png", 4, details);
	m_RandomEventImageDstRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_IsConsumed = false;
	m_ClickCooldown = c_ClickSafetyTimer;
}

RandomEventScene::~RandomEventScene()
{
	if (m_RandomEventAtlas != nullptr)
	{
		delete m_RandomEventAtlas;
		m_RandomEventAtlas = nullptr;
	}
}

void RandomEventScene::OnEnter()
{
	m_ClickCooldown = c_ClickSafetyTimer;
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
	case RandomEventScene::RANDOM_ENCOUNTER_FREE_CARD:
		SDL_Log("%i RANDOM_ENCOUNTER_FREE_CARD", r);
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

	if (m_RandomEventAtlas != nullptr)
	{
		m_RandomEventAtlas->SetAnimationId(m_GeneratedEncounter);
		m_RandomEventAtlas->SetCurrentFrame(0);
	}

	m_IsConsumed = false;
}

void RandomEventScene::DestroyRandomEncounter()
{
	switch (m_GeneratedEncounter)
	{
	default:
		break;
	}

	m_GeneratedEncounter = RandomEventScene::RANDOM_ENCOUNTER_UNKNOWN_OR_NOT_SET;
	m_IsConsumed = true;
}

void RandomEventScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_KEY_DOWN:
	{
		switch (e.key.key)
		{
		case SDLK_1: { m_RandomEventAtlas->SetCurrentFrame(0); } break;
		case SDLK_2: { m_RandomEventAtlas->SetCurrentFrame(2); } break;

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

void RandomEventScene::ApplyEncounterChanges()
{
	switch (m_GeneratedEncounter)
	{
	case RandomEventScene::RANDOM_ENCOUNTER_UNKNOWN_OR_NOT_SET:
		break;
	case RandomEventScene::RANDOM_ENCOUNTER_NOTHING:
		break;
	case RandomEventScene::RANDOM_ENCOUNTER_FREE_FULL_HEAL:
		
		break;
	case RandomEventScene::RANDOM_ENCOUNTER_FREE_CARD:
	{
		int r = rand() % 10 + 1;

		//Random Number
		if (r > 5)
		{
			int random_value = rand() % c_BasicNumberCardMaxValue;
			m_Player.GetDeck().AddCard(NumberCard(random_value));
			printf("num card %i\n", random_value);
		}
		else //random operand
		{
			int random_op = rand() % 4;
			OPERAND_TYPE op = (OPERAND_TYPE)(random_op);
			m_Player.GetDeck().AddCard(OperandCard(op));
			printf("op card %i\n", random_op);
		}
	}
		break;
	case RandomEventScene::RANDOM_ENCOUNTER_RANDOM_FREE_HEAL:
		break;

	case RandomEventScene::COUNT:
	default:
		break;
	}

	m_IsConsumed = true;
}

void RandomEventScene::Update(const float& deltaTime)
{
	if (m_ClickCooldown < 0.0f)
	{
		if (m_IsConsumed)
		{
			if (m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON))
			{
				m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAP);
			}
		}
		else
		{
			if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), m_RandomEventImageDstRect) &&
				m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON))
			{
				ApplyEncounterChanges();
				m_ClickCooldown = c_ClickSafetyTimer;
			}
		}
	}
	else
	{
		m_ClickCooldown -= deltaTime;
	}

	if (m_RandomEventAtlas != nullptr)
	{
		m_RandomEventAtlas->Update(deltaTime);

		int width = 0;
		int height = 0;
		SDL_GetWindowSize(&Services::GetWindow(), &width, &height);

		float size = SDL_min(m_RandomEventAtlas->GetFrameSizeX(), m_RandomEventAtlas->GetFrameSizeY());
		size = SDL_min(height, size);
		size = SDL_min(width, size);
		m_RandomEventImageDstRect.w = size;
		m_RandomEventImageDstRect.h = size;
		m_RandomEventImageDstRect.x = (width / 2) - (m_RandomEventImageDstRect.w / 2);
		m_RandomEventImageDstRect.y = (height / 2) - (m_RandomEventImageDstRect.h / 2);
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
