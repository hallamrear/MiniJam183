#include "pch.h"
#include "Texture.h"
#include <System/Services.h>
#include <SDL3/SDL_surface.h>

bool Texture::LoadPNG(const char* filename, SDL_Texture*& texture)
{
    assert(texture == nullptr);

    texture = IMG_LoadTexture(&Services::GetRenderer(), filename);

    if (texture == nullptr)
    {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to load file: %s", filename);
        return false;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

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
