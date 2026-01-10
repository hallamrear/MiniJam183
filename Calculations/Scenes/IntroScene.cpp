#include "pch.h"
#include "IntroScene.h"
#include <SDL3_image/SDL_image.h>
#include <System/Services.h>
#include <System/SceneManager.h>
#include <System/Input.h>

IntroScene::IntroScene(SceneManager& manager) : Scene(manager), m_Window(Services::GetWindow()), m_Renderer(Services::GetRenderer())
{
	m_IntroGif = IMG_LoadAnimation("Content\\Intro.gif");
	m_HasFinished = false;

	if (m_IntroGif != nullptr)
	{
		SDL_Log("Loaded intro successfully.");
	}
	else
	{
		SDL_Log("Failed to load intro.");
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, SDL_GetError());
	}

	m_CurrentFrameTexture = nullptr;
	m_CurrentFrame = 0;
	m_CurrentFrameTimeElapsed = 0.0f;

	if (m_IntroGif != nullptr)
	{
		if (m_IntroGif->frames[m_CurrentFrame] != nullptr)
		{
			m_CurrentFrameTexture = SDL_CreateTextureFromSurface(&m_Renderer, m_IntroGif->frames[m_CurrentFrame]);
		}
	}

	m_DrawRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
}

IntroScene::~IntroScene()
{
	if (m_IntroGif)
	{
		IMG_FreeAnimation(m_IntroGif);
		m_IntroGif = nullptr;
	}
}

void IntroScene::OnEnter()
{
	if (m_IntroGif == nullptr)
	{
		m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAIN_MENU);
	}
}

void IntroScene::OnExit()
{
	if (m_IntroGif)
	{
		IMG_FreeAnimation(m_IntroGif);
		m_IntroGif = nullptr;
	}
}

void IntroScene::Update(const float& deltaTime)
{
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(&m_Window, &w, &h);

	m_DrawRect.x = 0.0f; m_DrawRect.y = 0.0f;
	m_DrawRect.w = 0.0f; m_DrawRect.h = 0.0f;

	if (m_IntroGif != nullptr)
	{
		if (m_IntroGif->frames != nullptr)
		{
			m_DrawRect.x = (w / 2) - (m_IntroGif->frames[m_CurrentFrame]->w / 2);
			m_DrawRect.y = (h / 2) - (m_IntroGif->frames[m_CurrentFrame]->h / 2);

			m_DrawRect.w = SDL_min(m_IntroGif->frames[m_CurrentFrame]->w, w);
			m_DrawRect.h = SDL_min(m_IntroGif->frames[m_CurrentFrame]->h, h);

			m_CurrentFrameTimeElapsed += deltaTime;

			float frameDelaySeconds = m_IntroGif->delays[m_CurrentFrame] / 1000.0f;

			if (m_CurrentFrameTimeElapsed > frameDelaySeconds)
			{
				if (m_CurrentFrameTexture != nullptr)
				{
					SDL_DestroyTexture(m_CurrentFrameTexture);
					m_CurrentFrameTexture = nullptr;
				}

				if (m_CurrentFrame < m_IntroGif->count - 1)
				{
					m_CurrentFrameTexture = SDL_CreateTextureFromSurface(&m_Renderer, m_IntroGif->frames[m_CurrentFrame]);
					m_CurrentFrame++;
					m_CurrentFrameTimeElapsed = 0.0f;
				}
				else
				{
					m_HasFinished = true;
				}
			}
		}
	}

	if (m_HasFinished)
	{
		m_CurrentFrameTimeElapsed += deltaTime;

		if (m_CurrentFrameTimeElapsed > 1.5f)
		{
			m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAIN_MENU);
		}
	}

	if (m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON))
	{
		m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_MAIN_MENU);
		m_InputManager.ConsumeButtonPress(Input::MOUSE_BUTTON::LEFT_BUTTON);
	}
}

void IntroScene::Render(SDL_Renderer& renderer) const
{
	SDL_FRect bgRect = { 0.0f, 0.0f, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT };
	SDL_SetRenderDrawColorFloat(&renderer, 0.0f, 0.0f, 0.0f, 1.0f);
	SDL_RenderFillRect(&renderer, &bgRect);

	if (m_IntroGif != nullptr && m_CurrentFrameTexture != nullptr)
	{
		SDL_RenderTexture(&renderer, m_CurrentFrameTexture, nullptr, &m_DrawRect);
	}
}