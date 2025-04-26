#pragma once

class Services
{
private:
	static SDL_Renderer* m_Renderer;
	static SDL_Window* m_Window;

public:
	static bool Initialise(SDL_Renderer* renderer, SDL_Window* window);
	static bool Shutdown();

	static SDL_Renderer& GetRenderer();
	static SDL_Window& GetWindow();
};

