#pragma once

struct TTF_Text;

class Text
{
public:
	static bool LoadText(const char* str, TTF_Text*& texture);
	static bool SetText(const char* str, TTF_Text*& texture);
	static bool SetTextColour(const SDL_FColor& colour, TTF_Text*& texture);
	static bool AppendText(const char* str, TTF_Text*& texture);
	static bool QueryText(TTF_Text* texture, int& width, int& height);
	static bool Destroy(TTF_Text*& texture);
};

