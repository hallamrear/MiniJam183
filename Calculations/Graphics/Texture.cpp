#include "pch.h"
#include "Texture.h"
#include <System/Services.h>

bool Texture::LoadPNG(const char* filename, SDL_Texture*& texture)
{
    assert(texture == nullptr);

    texture = IMG_LoadTexture(&Services::GetRenderer(), filename);

    if (texture == nullptr)
    {
        SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to load file: %s", filename);
        return false;
    }
    
    return true;
}
