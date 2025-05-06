#pragma once

class WorldMap;
class Player;
class Input;

class Services
{
private:
	static SDL_Renderer* m_Renderer;
	static SDL_Window* m_Window;
	static Player* m_Player;
	static Input* m_InputManager;
	static WorldMap* m_WorldMap;

public:
	static bool Initialise(SDL_Renderer* renderer, SDL_Window* window);
	static bool Shutdown();

	static void ProvidePlayer(Player* player);
	static void ProvideInputManager(Input* inputManager);
	static void ProvideWorldMap(WorldMap* worldMap);

	static Input& GetInputManager();
	static SDL_Renderer& GetRenderer();
	static SDL_Window& GetWindow();
	static Player& GetPlayer();
	static WorldMap& GetWorldMap();
};

