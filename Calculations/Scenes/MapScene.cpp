#include "pch.h"
#include "MapScene.h"
#include <System/Services.h>
#include <Graphics/Texture.h>
#include <Gameplay/World/WorldMap.h>

float t_w = 0.0f;
float t_h = 0.0f;

MapScene::MapScene(SceneManager& manager) : Scene(manager), m_WorldMap(Services::GetWorldMap())
{
	texture = nullptr;
	Texture::LoadPNG("Content/Map/Temporary.png", texture);
	Texture::QueryTexture(texture, t_w, t_h);
}

MapScene::~MapScene()
{
	if(texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}	
}

void MapScene::OnEnter()
{
}

void MapScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_KEY_UP:
	{
		switch (e.key.key)
		{
		case SDLK_P:
		{
			m_WorldMap.GenerateNewMap(time(NULL), 0);
		}
		break;

		default:
			break;
		}
	}
	break;

	default:
		break;
	}
}

void MapScene::OnExit()
{

}

void MapScene::Update(const float& deltaTime)
{

}

void MapScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderDebugText(&renderer, 10, 10, "MAP SCENE");

	SDL_FRect r;
	r.w = t_w;
	r.h = t_h;

	for (size_t y = 0; y < c_MapLength; y++)
	{
		for (size_t x = 0; x < c_MapWidth; x++)
		{
			r.x = 300.0f + (r.w * x);
			r.y = 300.0f + (r.h * y);

			const MapNode& node = m_WorldMap.GetMapNode({ x, y });
			if (node.GetType() != MapNode::ENCOUNTER_UNKNOWN)
			{
				SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
			}

			SDL_RenderFillRect(&renderer, &r);

			std::string str = "\0";

			switch (node.GetType())
			{
				case MapNode::ENCOUNTER_START:	 str = "b"; break;
				case MapNode::ENCOUNTER_ENEMY:	 str = "E"; break;
				case MapNode::ENCOUNTER_SHOP:	 str = "S"; break;
				case MapNode::ENCOUNTER_REST:	 str = "R"; break;
				case MapNode::ENCOUNTER_EVENT:	 str = "V"; break;
				case MapNode::ENCOUNTER_ELITE:	 str = "L"; break;
				case MapNode::ENCOUNTER_BOSS:	 str = "e"; break;
				case MapNode::ENCOUNTER_UNKNOWN: str = "+"; break;

				default:
					break;
			}

			SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
			SDL_RenderDebugText(&renderer, r.x + ((r.w / 2) - 4), r.y + ((r.h / 2) - 4), str.c_str());
		}
	}

	r.w *= c_MapWidth;
	r.x = 300.0f;
	r.y = 300.0f - (2.0f * r.h);
	SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(&renderer, &r);
	SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(&renderer, r.x + ((r.w / 2) - 4), r.y + ((r.h / 2) - 4), "Boss");

	r.y = 300.0f + (r.h * c_MapLength) + (r.h);
	SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(&renderer, &r);
	SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(&renderer, r.x + ((r.w / 2) - 4), r.y + ((r.h / 2) - 4), "Start");
}
