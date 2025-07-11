#include "pch.h"
#include "Text.h"
#include <System/Services.h>
#include <System/FontRenderer.h>
#include <SDL3_ttf/SDL_ttf.h>

bool Text::LoadText(const char* str, TTF_Text*& texture)
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

bool Text::SetText(const char* str, TTF_Text*& texture)
{
    assert(texture != nullptr);
    return TTF_SetTextString(texture, str, strlen(str));
}

bool Text::AppendText(const char* str, TTF_Text*& texture)
{
    assert(texture != nullptr);
    return TTF_AppendTextString(texture, str, strlen(str));
}

bool Text::QueryText(TTF_Text* texture, int& width, int& height)
{
    return TTF_GetTextSize(texture, &width, &height);
}

bool Text::Destroy(TTF_Text*& texture)
{
    if (texture == nullptr)
        return false;

    TTF_DestroyText(texture);
    texture = nullptr;
    return true;
}