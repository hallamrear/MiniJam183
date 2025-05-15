#pragma once

class Texture
{
public:
	static bool LoadText(const char* str, SDL_Texture*& texture);
	static bool LoadPNG(const char* filename, SDL_Texture*& texture);
	static bool QueryTexture(SDL_Texture* texture, float& width, float& height);
	static bool QueryTexture(SDL_Texture* texture, int& width, int& height);
	static bool Destroy(SDL_Texture*& texture);
};
