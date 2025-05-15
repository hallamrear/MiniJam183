#include "pch.h"
#include "Texture.h"
#include <System/Services.h>
#include <System/FontRenderer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_surface.h>

bool Texture::LoadText(const char* str, TTF_Text*& texture)
{
    assert(texture == nullptr);

    FontRenderer& fr = Services::GetFontRenderer();

    texture = TTF_CreateText(&fr.GetTextEngine(), &fr.GetFont(), str, strlen(str));

    if (texture == nullptr)
    {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to create ttf_text for string '%s'", str);
        return false;
    }

    return true;
}

bool Texture::SetText(const char* str, TTF_Text*& texture)
{
    assert(texture != nullptr);
    return TTF_SetTextString(texture, str, strlen(str));
}

bool Texture::AppendText(const char* str, TTF_Text*& texture)
{
    assert(texture != nullptr);
    return TTF_AppendTextString(texture, str, strlen(str));
}

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

bool Texture::QueryText(TTF_Text* texture, int& width, int& height)
{
    return TTF_GetTextSize(texture, &width, &height);
}

bool Texture::Destroy(TTF_Text*& texture)
{
    if (texture == nullptr)
        return false;

    TTF_DestroyText(texture);
    texture = nullptr;
    return true;
}

bool Texture::Destroy(SDL_Texture*& texture)
{
    if (texture == nullptr)
        return false;

    SDL_DestroyTexture(texture);
    texture = nullptr;
    return true;
}
