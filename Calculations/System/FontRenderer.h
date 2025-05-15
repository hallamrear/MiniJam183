#pragma once

class TTF_TextEngine;
class TTF_Font;

class FontRenderer
{
private:
	TTF_TextEngine* m_TextEngine;
	TTF_Font* m_Font;

public:
	FontRenderer();
	~FontRenderer();

	bool Initialise(SDL_Renderer& renderer, const char* fontLocation);
	void Shutdown();

	TTF_TextEngine& GetTextEngine() const;
	TTF_Font& GetFont() const;
};

