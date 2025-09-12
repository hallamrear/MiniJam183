#pragma once
#include <unordered_map>
#include <System/File.h>

class Texture
{
private:
	static std::unordered_map<std::string, SDL_Texture*> m_TextureMap;

public:
	static bool LoadPNG(const char* filename, SDL_Texture*& texture);
	static bool QueryTexture(SDL_Texture* texture, float& width, float& height);
	static bool QueryTexture(SDL_Texture* texture, int& width, int& height);
	static bool Destroy(SDL_Texture*& texture);

	static void PreloadTextures(std::vector<File::Filepath>& filepaths);
	static void CleanupLoadedTextures();
};
