#pragma once

class Texture
{
public:
	static bool LoadPNG(const char* filename, SDL_Texture*& texture);
};
