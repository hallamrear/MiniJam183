#pragma once

class Player;

class Services
{
private:
	static SDL_Renderer* m_Renderer;
	static SDL_Window* m_Window;
	static Player* m_Player;

public:
	static bool Initialise(SDL_Renderer* renderer, SDL_Window* window, Player* player);
	static bool Shutdown();

	static SDL_Renderer& GetRenderer();
	static SDL_Window& GetWindow();
	static Player& GetPlayer();
};

