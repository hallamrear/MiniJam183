#include "pch.h"
#include "Services.h"
#include <Gameplay/Player/Player.h>

SDL_Renderer* Services::m_Renderer = nullptr;
SDL_Window* Services::m_Window = nullptr;
Player* Services::m_Player = nullptr;

bool Services::Initialise(SDL_Renderer* renderer, SDL_Window* window, Player* player)
{
	assert(renderer);
	assert(window);
	assert(player);

	m_Renderer = renderer;
	m_Window = window;
	m_Player = player;

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

	if (m_Player == nullptr)
	{
		m_Player = nullptr;
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

Player& Services::GetPlayer()
{
	assert(m_Player != nullptr);
	return *m_Player;
}
