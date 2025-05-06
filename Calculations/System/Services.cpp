#include "pch.h"
#include "Services.h"
#include <Gameplay/Player/Player.h>

SDL_Renderer* Services::m_Renderer = nullptr;
Input* Services::m_InputManager = nullptr;
SDL_Window* Services::m_Window = nullptr;
Player* Services::m_Player = nullptr;
WorldMap* Services::m_WorldMap = nullptr;

bool Services::Initialise(SDL_Renderer* renderer, SDL_Window* window)
{
	assert(renderer);
	assert(window);

	m_Renderer = renderer;
	m_Window = window;
	m_Player = nullptr;
	m_InputManager = nullptr;

	return true;
}

bool Services::Shutdown()
{	
	m_Renderer = nullptr;
	m_Window = nullptr;
	m_Player = nullptr;
	m_InputManager = nullptr;
	m_WorldMap = nullptr;

	return true;
}

void Services::ProvidePlayer(Player* player)
{
	assert(player);
	m_Player = player;
}

void Services::ProvideInputManager(Input* inputManager)
{
	assert(inputManager);
	m_InputManager = inputManager;
}

void Services::ProvideWorldMap(WorldMap* worldMap)
{
	assert(worldMap);
	m_WorldMap = worldMap;
}

Input& Services::GetInputManager()
{
	assert(m_InputManager != nullptr);
	return *m_InputManager;
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

WorldMap& Services::GetWorldMap()
{
	assert(m_WorldMap != nullptr);
	return *m_WorldMap;
}
