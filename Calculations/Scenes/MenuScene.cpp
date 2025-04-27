#include "pch.h"
#include "MenuScene.h"
#include <Graphics/Texture.h>
#include <System/Services.h>
#include <System/Collision.h>
#include <System/SceneManager.h>

constexpr const float c_ButtonWidth = 256.0f;
constexpr const float c_ButtonHeight = 128.0f;
constexpr const float c_ButtonSpacing = 64.0f;

MenuScene::MenuScene(SceneManager& manager) : Scene(manager)
{
	m_ExitButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	m_PlayButtonRect = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };

	m_BackgroundTexture = nullptr;
	Texture::LoadPNG("Content/Menu/Background.png", m_BackgroundTexture);

	m_PlayButtonTexture = nullptr;
	Texture::LoadPNG("Content/Menu/PlayButton.png", m_PlayButtonTexture);

	m_ExitButtonTexture = nullptr;
	Texture::LoadPNG("Content/Menu/ExitButton.png", m_ExitButtonTexture);
}

MenuScene::~MenuScene()
{
	if (m_BackgroundTexture != nullptr)
	{
		SDL_DestroyTexture(m_BackgroundTexture);
		m_BackgroundTexture = nullptr;
	}
}

void MenuScene::OnEnter()
{
}

void MenuScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	{
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			int X = e.button.x;
			int Y = e.button.y;
			if (Collision::PointInRect(X, Y, m_PlayButtonRect))
			{
				m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_BATTLE);
			}
			else if (Collision::PointInRect(X, Y, m_ExitButtonRect))
			{
				SDL_Event event = { SDL_EVENT_QUIT };
				SDL_PushEvent(&event);
			}
		}
	}
	break;

	default:
		break;
	}

}

void MenuScene::OnExit()
{
}

void MenuScene::Update(const float& deltaTime)
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(&Services::GetWindow(), &width, &height);

	const float buttonSizeRatio = c_ButtonHeight / c_ButtonWidth;
	const float windowScaling = width / TARGET_RESOLUTION_X;
	const float scaledWidth = c_ButtonWidth * windowScaling;
	const float scaledHeight = scaledWidth * buttonSizeRatio;

	m_ExitButtonRect.w = scaledWidth;
	m_ExitButtonRect.h = scaledHeight;
	m_ExitButtonRect.x = (width / 2) - (m_ExitButtonRect.w / 2);
	m_ExitButtonRect.y = ((height / 3) * 2);

	m_PlayButtonRect.w = scaledWidth * 1.25;
	m_PlayButtonRect.h = scaledHeight * 1.25;
	m_PlayButtonRect.x = (width / 2) - (m_PlayButtonRect.w / 2);
	m_PlayButtonRect.y = m_ExitButtonRect.y - m_PlayButtonRect.h - c_ButtonSpacing;
}

void MenuScene::Render(SDL_Renderer& renderer) const
{
	if (m_BackgroundTexture != nullptr)
	{
		SDL_RenderTexture(&renderer, m_BackgroundTexture, nullptr, nullptr);
	}

	if (m_PlayButtonTexture != nullptr)
	{
		SDL_RenderTexture(&renderer, m_PlayButtonTexture, nullptr, &m_PlayButtonRect);
	}

	if (m_ExitButtonTexture != nullptr)
	{
		SDL_RenderTexture(&renderer, m_ExitButtonTexture, nullptr, &m_ExitButtonRect);
	}

	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderDebugText(&renderer, 64, 64, "MENU SCENE");
}
