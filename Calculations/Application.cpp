#include "pch.h"
#include "Application.h"
#include <Graphics/Texture.h>
#include <System/Services.h>

SDL_Texture* texture = nullptr;

Application::Application()
{
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

	Services::Initialise(m_Renderer, m_Window);

	Texture::LoadPNG("Content/cat.png", texture);

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
	ShutdownSDL();

	Services::Shutdown();
}

void Application::ShutdownSDL()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
	SDL_Quit();
}

float zoomlevel = 1.0f;

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
				SDL_SetRenderLogicalPresentation(m_Renderer, width * zoomlevel, height * zoomlevel, scalingType);
			}
			break;

			case SDL_EVENT_QUIT:
			{
				m_IsRunning = false;
			}
			break;

			case SDL_EVENT_MOUSE_WHEEL:
			{

			}
			break;

			case SDL_EVENT_MOUSE_MOTION:
			{

			}
			break;

			case SDL_EVENT_KEY_DOWN:
			{
				switch (e.key.key)
				{
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
	}
}

void Application::Update(const float& deltaTime)
{

}

void Application::Render() const
{
	SDL_SetRenderDrawColorFloat(m_Renderer, CLEAR_COLOUR[0], CLEAR_COLOUR[1], CLEAR_COLOUR[2], CLEAR_COLOUR[3]);
	SDL_RenderClear(m_Renderer);
	
	SDL_FRect rect{ 64, 64, INITIAL_WINDOW_WIDTH / 2 - 64, INITIAL_WINDOW_HEIGHT / 2 - 64 };
	SDL_SetRenderDrawColorFloat(m_Renderer, 1.0F, 0.0F, 0.0F, 1.0F);
	SDL_RenderRect(m_Renderer, &rect);

	SDL_FRect R{ INITIAL_WINDOW_WIDTH / 2 - 64, INITIAL_WINDOW_HEIGHT / 2 - 64, INITIAL_WINDOW_WIDTH / 2 - 64, INITIAL_WINDOW_HEIGHT / 2 - 64 };
	SDL_RenderTexture(m_Renderer, texture, nullptr, &R);

	SDL_RenderPresent(m_Renderer);
}

const bool Application::IsRunning() const
{
	return m_IsRunning;
}