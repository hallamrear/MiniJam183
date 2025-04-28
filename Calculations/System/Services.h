#pragma once

class Player;
class Input;

class Services
{
private:
	static SDL_Renderer* m_Renderer;
	static SDL_Window* m_Window;
	static Player* m_Player;
	static Input* m_InputManager;

public:
	static bool Initialise(SDL_Renderer* renderer, SDL_Window* window);
	static bool Shutdown();

	static void ProvidePlayer(Player* player);
	static void ProvideInputManager(Input* inputManager);

	static Input& GetInputManager();
	static SDL_Renderer& GetRenderer();
	static SDL_Window& GetWindow();
	static Player& GetPlayer();
};

