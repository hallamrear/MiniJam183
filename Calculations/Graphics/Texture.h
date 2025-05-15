#pragma once

struct TTF_Text;

class Texture
{
public:
	static bool LoadText(const char* str, TTF_Text*& texture);
	static bool SetText(const char* str, TTF_Text*& texture);
	static bool AppendText(const char* str, TTF_Text*& texture);
	static bool QueryText(TTF_Text* texture, int& width, int& height);
	static bool Destroy(TTF_Text*& texture);

	static bool LoadPNG(const char* filename, SDL_Texture*& texture);
	static bool QueryTexture(SDL_Texture* texture, float& width, float& height);
	static bool QueryTexture(SDL_Texture* texture, int& width, int& height);
	static bool Destroy(SDL_Texture*& texture);
};
