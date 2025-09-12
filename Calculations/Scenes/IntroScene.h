#pragma once
#include "Scene.h"

struct IMG_Animation;

class IntroScene :
    public Scene
{
private:
    SDL_Renderer& m_Renderer;
    SDL_Window& m_Window;
    IMG_Animation* m_IntroGif;
    SDL_FRect m_DrawRect;
    int m_CurrentFrame;
    float m_CurrentFrameTimeElapsed;
    SDL_Texture* m_CurrentFrameTexture;
    bool m_HasFinished;

public:
    IntroScene(SceneManager& manager);
    ~IntroScene();

    void OnEnter() override;
    void OnExit() override;
    void Update(const float& deltaTime) override;
    void Render(SDL_Renderer& renderer) const override;
};

