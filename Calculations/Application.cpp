#include "pch.h"
#include "Application.h"
#include <System/Services.h>
#include <System/SceneManager.h>
#include <System/Input.h>
#include <Gameplay/Player/Player.h>
#include <Gameplay/World/WorldMap.h>
#include <System/FontRenderer.h>

Application::Application()
{
	m_InputManager = nullptr;
	m_FontRenderer = nullptr;
	m_SceneManager = nullptr;
	m_WorldMap = nullptr;
	m_Player = nullptr;
	m_Renderer = nullptr;
	m_Window = nullptr;
	m_WindowFlags = 0;
	m_IsRunning = false;
	m_IsFullscreen = false;
}

Application::~Application()
{
	if (m_IsRunning)
	{
		Shutdown();

		assert(m_IsRunning);
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to properly shutdown game instance.");
	}
}

bool Application::Initialise()
{
	if (InitSDL() == false)
	{
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to start SDL.");
		return false;
	}

	SDL_Log("SDL setup complete.");

	if (Services::Initialise(m_Renderer, m_Window) == false)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to setup services.");
		return false;
	}

	m_FontRenderer = new FontRenderer();
	if (m_FontRenderer->Initialise(*m_Renderer, "Content/Fonts/m6x11.ttf") == false)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialise font renderer.");
		return false;
	}
	Services::ProvideFontRenderer(m_FontRenderer);

	m_Player = new Player();
	Services::ProvidePlayer(m_Player);

	m_InputManager = new Input();
	Services::ProvideInputManager(m_InputManager);

	m_WorldMap = new WorldMap();
	Services::ProvideWorldMap(m_WorldMap);

	m_SceneManager = new SceneManager();

#ifdef _DEBUG
	//m_SceneManager->ChangeScene(SCENE_IDENTIFIER::SCENE_MAP);
#endif

	m_IsRunning = true;
	return true;
}

bool Application::InitSDL()
{
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, PROJECT_NAME);
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, VERSION_STRING);
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, "com.hallamrear.calculations");
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, "Hallam Rear");
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Copyright (c) 2025 Hallam Rear");
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, "");
	SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "Game");

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD | SDL_INIT_EVENTS) == false)
	{
		SDL_LogError(SDL_LOG_PRIORITY_INFO, SDL_GetError());
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to initialise SDL subsystem.");
		return false;
	}

	SDL_Log("Initialised SDL Subsystems.");

	SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;

	#ifndef _DEBUG
	flags |= SDL_WINDOW_MAXIMIZED;
	#endif // _DEBUG

	m_Window = SDL_CreateWindow(WINDOW_TITLE, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, flags);

	if (m_Window == nullptr)
	{
		SDL_LogError(SDL_LOG_PRIORITY_INFO, SDL_GetError());
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to create window.");
		return false;
	}

	SDL_Log("Created window %s.", WINDOW_TITLE);

	m_Renderer = SDL_CreateRenderer(m_Window, NULL);

	if (m_Renderer == nullptr)
	{
		SDL_LogError(SDL_LOG_PRIORITY_INFO, SDL_GetError());
		SDL_LogError(SDL_LOG_PRIORITY_CRITICAL, "Failed to create renderer.");
		return false;
	}
	SDL_Log("Created renderer.");

	return true;
}

void Application::Shutdown()
{
	if (m_SceneManager != nullptr)
	{
		delete m_SceneManager;
		m_SceneManager = nullptr;
	}

	if (m_InputManager != nullptr)
	{
		delete m_InputManager;
		m_InputManager = nullptr;
	}

	if (m_FontRenderer != nullptr)
	{
		m_FontRenderer->Shutdown();
		delete m_FontRenderer;
		m_FontRenderer = nullptr;
	}

	Services::Shutdown();
	ShutdownSDL();
}

void Application::ShutdownSDL()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

void Application::ProcessEvents(const float& deltaTime)
{
	SDL_Event e{};

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_EVENT_WINDOW_RESIZED:
			{
				int width = 0;
				int height = 0;
				SDL_GetWindowSize(m_Window, &width, &height);
				SDL_RendererLogicalPresentation scalingType = SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;
				SDL_SetRenderLogicalPresentation(m_Renderer, width, height, scalingType);
			}
			break;

			case SDL_EVENT_QUIT:
			{
				m_IsRunning = false;
			}
			break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			case SDL_EVENT_MOUSE_BUTTON_UP:
			case SDL_EVENT_MOUSE_MOTION:
			case SDL_EVENT_MOUSE_WHEEL:
			{
				m_InputManager->HandleEvent(e);
			}
			break;

			case SDL_EVENT_KEY_DOWN:
			{
				switch (e.key.key)
				{
					case SDLK_F1: { if (m_SceneManager != nullptr) m_SceneManager->ChangeScene(SCENE_IDENTIFIER::SCENE_MAIN_MENU); } break;
					case SDLK_F2: { if (m_SceneManager != nullptr) m_SceneManager->ChangeScene(SCENE_IDENTIFIER::SCENE_MAP); } break;
					case SDLK_F3: { if (m_SceneManager != nullptr) m_SceneManager->ChangeScene(SCENE_IDENTIFIER::SCENE_SHOP); } break;
					case SDLK_F4: { if (m_SceneManager != nullptr) m_SceneManager->ChangeScene(SCENE_IDENTIFIER::SCENE_BATTLE); } break;

					case SDLK_F5:
					{
						bool success = SDL_SetWindowFullscreen(m_Window, !m_IsFullscreen);
						if (success)
						{
							m_IsFullscreen = !m_IsFullscreen;
						}
					}
					break;

					default:
						break;
				}
			}
			break;

			case SDL_EVENT_KEY_UP:
			{
				switch (e.key.key)
				{
					default: 
						break;
				}
			}
			break;

			default:
			{

			}
			break;
		}

		if (m_SceneManager != nullptr && m_IsRunning == true)
		{
			m_SceneManager->PassEventToScene(e);
		}
	}
}

void Application::Update(const float& deltaTime)
{
	if (m_SceneManager != nullptr)
	{
		m_SceneManager->Update(deltaTime);
	}
}

void Application::Render() const
{
	SDL_SetRenderDrawColorFloat(m_Renderer, CLEAR_COLOUR[0], CLEAR_COLOUR[1], CLEAR_COLOUR[2], CLEAR_COLOUR[3]);
	SDL_RenderClear(m_Renderer);

	if (m_SceneManager != nullptr)
	{
		m_SceneManager->Render(*m_Renderer);
	}

	SDL_RenderPresent(m_Renderer);
}

const bool Application::IsRunning() const
{
	return m_IsRunning;
}