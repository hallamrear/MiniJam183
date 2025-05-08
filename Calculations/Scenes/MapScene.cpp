#include "pch.h"
#include "MapScene.h"
#include <System/Services.h>
#include <System/SceneManager.h>
#include <Graphics/Texture.h>
#include <System/Collision.h>
#include <System/Input.h>

MapScene::MapScene(SceneManager& manager) : Scene(manager),
	m_WorldMap(Services::GetWorldMap()),
	c_EncounterImageWidth(64.0f), c_EncounterImageHeight(64.0f)
{
	m_SelectedNodeIndex = -1;
	m_CanSelectButton = true;
	m_ButtonPressCooldown = -1.0f;

	for (size_t i = 0; i < 50; i++)
	{
		m_NodeButtons[i] = SDL_FRect{-1.0f, -1.0f, 0.0f, 0.0f};
	}

	m_EncounterAtlas = nullptr;
	Texture::LoadPNG("Content/Map/EncounterAtlas.png", m_EncounterAtlas);
	m_CrossTexture = nullptr;
	Texture::LoadPNG("Content/Map/Cross.png", m_CrossTexture);
}

MapScene::~MapScene()
{
	m_SelectedNodeIndex = -1;
	for (size_t i = 0; i < 50; i++)
	{
		m_NodeButtons[i] = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	if (m_EncounterAtlas != nullptr)
	{
		SDL_DestroyTexture(m_EncounterAtlas);
		m_EncounterAtlas = nullptr;
	}

	if (m_CrossTexture != nullptr)
	{
		SDL_DestroyTexture(m_CrossTexture);
		m_CrossTexture = nullptr;
	}
}

void MapScene::OnEnter()
{
	m_CanSelectButton = false;
	m_ButtonPressCooldown = 2.0f;
	RecalculateButtonRects();
}

void MapScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
	{
		RecalculateButtonRects();
	}
	break;

	case SDL_EVENT_KEY_UP:
	{
		switch (e.key.key)
		{
		case SDLK_P:
		{
			m_WorldMap.GenerateNewMap(time(NULL), 0);
			RecalculateButtonRects();
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
	m_SelectedNodeIndex = -1;
}

void MapScene::RecalculateButtonRects()
{
	int window_w = 0;
	int window_h = 0;
	SDL_GetWindowSize(&Services::GetWindow(), &window_w, &window_h);

	const float padding = SDL_max(c_EncounterImageWidth, c_EncounterImageHeight);
	const float totalWidth = c_MapWidth * c_EncounterImageWidth + (c_MapWidth * padding);
	const float totalHeight = c_MapLength * c_EncounterImageHeight + (c_MapLength * padding);
	float offset_x = (window_w / 2) - (totalWidth / 2) - (c_EncounterImageWidth / 2);
	float offset_y = (window_h / 2) - (totalHeight / 2);

	int x = m_WorldMap.GetCurrentNode().GetPosition().first;
	int y = m_WorldMap.GetCurrentNode().GetPosition().second;
	//offset_x -= ((x * c_EncounterImageWidth) + (x * padding));
	offset_y -= ((y * c_EncounterImageHeight) + (y * padding));

	int index = 0;
	for (size_t y = 0; y < c_MapLength; y++)
	{
		for (size_t x = 0; x < c_MapWidth; x++)
		{
			const MapNode& node = m_WorldMap.GetMapNode({ x, y });
			index = (y * c_MapWidth) + x;

			if (node.GetType() == MapNode::ENCOUNTER_TYPE::ENCOUNTER_UNKNOWN)
			{
				m_NodeButtons[index].x = -1.0f;
				m_NodeButtons[index].y = -1.0f;
				m_NodeButtons[index].w = -1.0f;
				m_NodeButtons[index].h = -1.0f;
				continue;
			}

			m_NodeButtons[index].x = offset_x + (x * c_EncounterImageWidth) + (x * padding);
			m_NodeButtons[index].y = offset_y + (y * c_EncounterImageHeight) + (y * padding);
			m_NodeButtons[index].w = c_EncounterImageWidth;
			m_NodeButtons[index].h = c_EncounterImageHeight;
		}
	}
}

void MapScene::Update(const float& deltaTime)
{
	constexpr const size_t mapSize = c_MapLength * c_MapWidth;
	m_SelectedNodeIndex = -1;

	if (m_CanSelectButton == false)
	{
		m_ButtonPressCooldown -= deltaTime;

		if (m_ButtonPressCooldown <= 0.0f)
		{
			m_CanSelectButton = true;
		}
	}

	for (size_t i = 0; i < mapSize; i++)
	{
		if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), m_NodeButtons[i])
			&& m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON)
			&& m_CanSelectButton)
		{
			m_SelectedNodeIndex = i;
			m_CanSelectButton = false;
			m_ButtonPressCooldown = 0.5f;

			const int x = m_SelectedNodeIndex % c_MapWidth;
			const int y = m_SelectedNodeIndex / c_MapWidth;

			const MapNode& node = m_WorldMap.GetMapNode({ x, y });
			m_WorldMap.SetCurrentNode(node);

			switch (node.GetType())
			{
			case MapNode::ENCOUNTER_START: {  } break;
			case MapNode::ENCOUNTER_UNKNOWN: break;

			case MapNode::ENCOUNTER_ENEMY:
			case MapNode::ENCOUNTER_ELITE:
			case MapNode::ENCOUNTER_BOSS:
			{ 
				m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_BATTLE); 
			} 
			break;

			case MapNode::ENCOUNTER_SHOP:
			{ 
				m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_SHOP);
			} 
			break;

			case MapNode::ENCOUNTER_REST: 
			{ 
				m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_REST);
			} 
			break;

			case MapNode::ENCOUNTER_EVENT:
			{
				m_SceneManager.ChangeScene(SCENE_IDENTIFIER::SCENE_RANDOM_EVENT);
			} 
			break;

			default:
				break;
			}

			
			break;
		}
	}

	//If a node has been selected,
	if (m_SelectedNodeIndex != -1)
	{
		const int x = m_SelectedNodeIndex % c_MapWidth;
		const int y = m_SelectedNodeIndex / c_MapWidth;
		m_WorldMap.SetCurrentNode(m_WorldMap.GetMapNode({ x, y }));
		RecalculateButtonRects();
	}
}

void MapScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderDebugText(&renderer, 10, 10, "SCENE_IDENTIFIER::MAP_SCENE");

	const int& currentYPosition = m_WorldMap.GetCurrentNode().GetPosition().second;

	for (int y = 0; y < c_MapLength; y++)
	{
		for (int x = 0; x < c_MapWidth; x++)
		{
			const MapNode& node = m_WorldMap.GetMapNode({ x, y }); 
			if (node.GetType() == MapNode::ENCOUNTER_TYPE::ENCOUNTER_UNKNOWN)
			{
				continue;
			}

			const size_t index = (y * c_MapWidth) + x;
			const SDL_FRect& drawRect = m_NodeButtons[index];

			if (drawRect.x == -1.0f || drawRect.y == -1.0f)
			{
				continue;
			}

			SDL_SetRenderDrawColor(&renderer, 255, 0, 0, 255);

			if (index == m_SelectedNodeIndex)
			{
				SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
			}

			SDL_FRect srcRect{};
			srcRect.w = c_EncounterImageWidth;
			srcRect.h = c_EncounterImageHeight;
			srcRect.x = c_EncounterImageWidth * (int)(node.GetType());
			srcRect.y = 0.0f;

			SDL_RenderTexture(&renderer, m_EncounterAtlas, &srcRect, &drawRect);

			if (y < currentYPosition)
			{
				SDL_RenderTexture(&renderer, m_CrossTexture, nullptr, &drawRect);
			}

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
				default: break;
			}

			SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
			SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);
			SDL_RenderDebugText(&renderer, drawRect.x + ((drawRect.w / 2) - 4), drawRect.y + ((drawRect.h / 2) - 4), str.c_str());
		}
	}
}
