#include "pch.h"
#include "Texture.h"
#include <System/Services.h>
#include <SDL3/SDL_surface.h>

std::unordered_map<std::string, SDL_Texture*> Texture::m_TextureMap = std::unordered_map<std::string, SDL_Texture*>();

bool Texture::LoadPNG(const char* filename, SDL_Texture*& texture)
{
    auto found = m_TextureMap.find(filename);

    if (found != m_TextureMap.end())
    {
        texture = found->second;
        return true;
    }

    assert(texture == nullptr);

    texture = IMG_LoadTexture(&Services::GetRenderer(), filename);

    if (texture == nullptr)
    {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to load file: %s", filename);
        return false;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    m_TextureMap.insert({ filename, texture });

    return true;
}

bool Texture::QueryTexture(SDL_Texture* texture, float& width, float& height)
{
    assert(texture != nullptr);
    return SDL_GetTextureSize(texture, &width, &height);
}

bool Texture::QueryTexture(SDL_Texture* texture, int& width, int& height)
{
    assert(texture != nullptr);
    float w = 0.0f;
    float h = 0.0f;
    bool result = SDL_GetTextureSize(texture, &w, &h);
    width = (int)w;
    height = (int)h;
    return result;
}

bool Texture::Destroy(SDL_Texture*& texture)
{
    if (texture == nullptr)
        return false;

    SDL_DestroyTexture(texture);
    texture = nullptr;
    return true;
}

void Texture::PreloadTextures(std::vector<File::Filepath>& filepaths)
{
    CleanupLoadedTextures();

    size_t count = filepaths.size();
    for (size_t i = 0; i < count; i++)
    {
        SDL_Texture* texture = nullptr;
        bool loaded = LoadPNG(filepaths[i].string().c_str(), texture);
        assert(loaded && (texture != nullptr));
    }
}

void Texture::CleanupLoadedTextures()
{
    for (auto& texture : m_TextureMap)
    {
        if (texture.second != nullptr)
        {
            bool destroyed = Texture::Destroy(texture.second);
            SDL_assert(destroyed);
            texture.second = nullptr;
        }
    }

    m_TextureMap.clear();
}
