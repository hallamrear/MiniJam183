#include "pch.h"
#include "RestScene.h"
#include <System/Input.h>
#include <System/SceneManager.h>
#include <Graphics/Texture.h>
#include <System/Services.h>
#include <System/Collision.h>
#include <Gameplay/Player/Player.h>

RestScene::RestScene(SceneManager& manager) : Scene(manager), m_Window(Services::GetWindow())
{
	m_WindowWidth = 0;
	m_WindowHeight = 0;
	m_HealAmount = 0;
	m_HasBeenUsed = false;
	m_ClickCooldown = 2.0f;
	m_ReturnButton = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_CampfireRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_DetailTextOffset = SDL_Point{ 0, 0 };
	m_LitCampfireTexture = nullptr;
	Texture::LoadPNG("Content/Rest/LitCampfire.png", m_LitCampfireTexture);
	m_UnlitCampfireTexture = nullptr;
	Texture::LoadPNG("Content/Rest/UnlitCampfire.png", m_UnlitCampfireTexture);
	m_ReturnButtonTexture = nullptr;
	Texture::LoadPNG("Content/Rest/ReturnButton.png", m_ReturnButtonTexture);
	m_ReturnText = nullptr;
	Texture::LoadText("Return", m_ReturnText);
	m_DetailsText = nullptr;
	Texture::LoadText("TEST", m_DetailsText);
}

RestScene::~RestScene()
{
	Texture::Destroy(m_LitCampfireTexture);
	Texture::Destroy(m_UnlitCampfireTexture);
	Texture::Destroy(m_ReturnButtonTexture);
	Texture::Destroy(m_ReturnText);
	Texture::Destroy(m_DetailsText);
}

void RestScene::OnEnter()
{
	m_HasBeenUsed = false;
	m_ClickCooldown = 2.0f;

	Player& player = Services::GetPlayer();
	m_HealAmount = SDL_min(player.GetMaxHealth() - player.GetCurrentHealth(), 20);
	std::string str = "Rest here and heal for " + std::to_string(m_HealAmount);
	Texture::SetText(str.c_str(), m_DetailsText);

	Texture::QueryText(m_DetailsText, m_DetailTextOffset.x, m_DetailTextOffset.y);
}

void RestScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_MOUSE_BUTTON_UP:
	{
		m_ClickCooldown = -1.0f;
	}
	break;

	default:
		break;
	}
}

void RestScene::OnExit()
{
	m_HealAmount = 0;
	m_HasBeenUsed = true;
}

void RestScene::Update(const float& deltaTime)
{
	SDL_GetWindowSizeInPixels(&m_Window, &m_WindowWidth, &m_WindowHeight);

	if (m_ClickCooldown > 0.0f)
	{
		m_ClickCooldown -= deltaTime;
		return;
	}

	if (m_HasBeenUsed)
	{
		m_CampfireRect.w = m_UnlitCampfireTexture->w;
		m_CampfireRect.h = m_UnlitCampfireTexture->h;
		m_CampfireRect.x = (m_WindowWidth / 2) - (m_CampfireRect.w / 2);
		m_CampfireRect.y = (m_WindowHeight / 2) - (m_CampfireRect.h / 2);

		float ratio = (float)m_WindowHeight / (float)m_WindowWidth;
		int w = 0, h = 0;
		Texture::QueryText(m_ReturnText, w, h);
		m_ReturnButton.w = (w);
		m_ReturnButton.h = (h);
		m_ReturnButton.x = floor(m_WindowWidth / 15.0f);
		m_ReturnButton.y = m_WindowHeight - floor(m_WindowHeight / 10.0f) - m_ReturnButton.h;

		if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), m_ReturnButton) &&
			m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON))
		{
			m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAP);
		}
	}
	else
	{
		m_CampfireRect.w = m_LitCampfireTexture->w;
		m_CampfireRect.h = m_LitCampfireTexture->h;
		m_CampfireRect.x = (m_WindowWidth / 2) - (m_CampfireRect.w / 2);
		m_CampfireRect.y = (m_WindowHeight / 2) - (m_CampfireRect.h / 2);

		if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), m_CampfireRect) &&
			m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON))
		{
			m_HasBeenUsed = true;
			Player& player = Services::GetPlayer();
			player.Heal(m_HealAmount);
			m_HealAmount = 0;
		}
	}
}

void RestScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(&renderer, 10, 10, "SCENE_IDENTIFIER::SCENE_REST");

	if (m_HasBeenUsed)
	{
		if (m_UnlitCampfireTexture != nullptr)
		{
			SDL_RenderTexture(&renderer, m_UnlitCampfireTexture, nullptr, &m_CampfireRect);
		} 
		
		if (m_ReturnText != nullptr)
		{
			TTF_DrawRendererText(m_ReturnText, m_ReturnButton.x, m_ReturnButton.y);
		}
		SDL_RenderRect(&renderer, &m_ReturnButton);
	}
	else
	{
		if (m_LitCampfireTexture != nullptr)
		{
			SDL_RenderTexture(&renderer, m_LitCampfireTexture, nullptr, &m_CampfireRect);
		}

		if (m_DetailsText != nullptr)
		{
			float x = (m_CampfireRect.x - (m_CampfireRect.w / 2)) - (m_DetailTextOffset.x / 2);
			float y = (m_CampfireRect.y - (m_CampfireRect.h / 2));
			TTF_DrawRendererText(m_DetailsText, m_CampfireRect.x, m_CampfireRect.y - (2.0f * m_DetailTextOffset.y));
		}
	}
}
