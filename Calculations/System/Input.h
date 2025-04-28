#pragma once

class Input
{
private:
	int m_MouseX;
	int m_MouseY;
	bool m_MouseButtons[3];

public:
	enum MOUSE_BUTTON
	{
		LEFT_BUTTON = 0,
		MIDDLE_BUTTON = 1,
		RIGHT_BUTTON = 2
	};

	Input();
	~Input();

	void HandleEvent(const SDL_Event& e);

	bool GetMouseButtonDown(const MOUSE_BUTTON& button) const;
	const int& GetMouseX() const;
	const int& GetMouseY() const;
};

