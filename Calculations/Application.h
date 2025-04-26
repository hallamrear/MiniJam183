#pragma once

class Application
{
private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window; 
	SDL_WindowFlags m_WindowFlags;
	bool m_IsRunning;
	bool m_IsFullscreen;

	bool InitSDL();
	void ShutdownSDL();


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

