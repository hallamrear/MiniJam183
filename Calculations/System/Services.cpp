#include "pch.h"
#include "Services.h"

SDL_Renderer* Services::m_Renderer = nullptr;
SDL_Window* Services::m_Window = nullptr;

bool Services::Initialise(SDL_Renderer* renderer, SDL_Window* window)
{
	assert(renderer);
	assert(window);

	m_Renderer = renderer;
	m_Window = window;

	return false;
}

bool Services::Shutdown()
{
	if (m_Renderer != nullptr)
	{
		m_Renderer = nullptr;
	}

	if (m_Window == nullptr)
	{
		m_Window = nullptr;
	}

	return true;
}

SDL_Renderer& Services::GetRenderer()
{
	assert(m_Renderer != nullptr);
	return *m_Renderer;
}

SDL_Window& Services::GetWindow()
{
	assert(m_Window != nullptr);
	return *m_Window;
}
