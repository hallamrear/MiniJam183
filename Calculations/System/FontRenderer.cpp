#include "pch.h"
#include "FontRenderer.h"
#include <sdl3_ttf/SDL_ttf.h>

FontRenderer::FontRenderer()
{
    m_Font = nullptr;
    m_TextEngine = nullptr;
}

FontRenderer::~FontRenderer()
{
    Shutdown();
}

bool FontRenderer::Initialise(SDL_Renderer& renderer, const char* fontLocation)
{
    if (m_TextEngine != nullptr)
    {
        SDL_Log("Tried to initialise a font renderer that already exists.");
        return false;
    }

    TTF_Init();

    m_TextEngine = TTF_CreateRendererTextEngine(&renderer);

    if (m_TextEngine == nullptr)
    {
        SDL_Log("Failed to create text engine.");
        SDL_Log("Error: %s", SDL_GetError());
        return false;
    }

    m_Font = TTF_OpenFont(fontLocation, 32.0f);

    if (m_Font == nullptr)
    {
        SDL_Log("Failed to load default font '%s'", fontLocation);
        SDL_Log("Error: %s", SDL_GetError());
        return false;
    }

    TTF_SetFontWrapAlignment(m_Font, TTF_HORIZONTAL_ALIGN_CENTER);

    return true;
}

void FontRenderer::Shutdown()
{
    if (m_Font != nullptr)
    {
        TTF_CloseFont(m_Font);
        m_Font = nullptr;
    }

    if (m_TextEngine != nullptr)
    {
        TTF_DestroyRendererTextEngine(m_TextEngine);
        m_TextEngine = nullptr;
    }

    TTF_Quit();
}

TTF_TextEngine& FontRenderer::GetTextEngine() const
{
    return *m_TextEngine;
}

TTF_Font& FontRenderer::GetFont() const
{
    return *m_Font;
}
