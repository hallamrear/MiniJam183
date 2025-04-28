#pragma once

class SceneManager;
class Player;
class Input;

class Application
{
private:
	Input* m_InputManager;
	Player* m_Player;
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window; 
	SDL_WindowFlags m_WindowFlags;
	bool m_IsRunning;
	bool m_IsFullscreen;

	bool InitSDL();
	void ShutdownSDL();

	SceneManager* m_SceneManager;

public:
	Application();
	~Application();

	bool Initialise();
	void Shutdown();

	void ProcessEvents(const float& deltaTime);
	void Update(const float& deltaTime);
	void Render() const;

	const bool IsRunning() const;
};

