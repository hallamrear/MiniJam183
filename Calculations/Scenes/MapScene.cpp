#include "pch.h"
#include "MapScene.h"
#include <System/Services.h>
#include <System/SceneManager.h>
#include <Graphics/Texture.h>
#include <System/Collision.h>
#include <System/Input.h>

MapScene::MapScene(SceneManager& manager) : Scene(manager),
	m_WorldMap(Services::GetWorldMap()),
	c_EncounterImageWidth(64.0f), c_EncounterImageHeight(64.0f),
	c_MapScrollSpeed(10.0f),
	c_MapEdgePadding(16.0f),
	c_ButtonPressCooldown(0.5f)
{
	m_IsButtonRectsDirty = true;
	m_MapScrollOffset = 0.0f;
	m_SelectedNodeIndex = -1;
	m_CanSelectButton = true;
	m_ButtonPressCooldownTimer = -1.0f;

	m_PossibleMapMovements = std::vector<const MapNode*>();

	for (size_t i = 0; i < 50; i++)
	{
		m_NodeButtons[i] = SDL_FRect{-1.0f, -1.0f, 0.0f, 0.0f};
	}

	m_StartNodeDrawRect = m_NodeButtons[0];
	m_EndNodeDrawRect = m_NodeButtons[0];

	m_EncounterAtlas = nullptr;
	Texture::LoadPNG("Content/Map/EncounterAtlas.png", m_EncounterAtlas);
	m_CrossTexture = nullptr;
	Texture::LoadPNG("Content/Map/Cross.png", m_CrossTexture);
	m_PlayerIconTexture = nullptr;
	Texture::LoadPNG("Content/Map/PlayerIcon.png", m_PlayerIconTexture);
	m_StartNodeTexture = nullptr;
	Texture::LoadPNG("Content/Map/Start.png", m_StartNodeTexture);
	m_EndNodeTexture = nullptr;
	Texture::LoadPNG("Content/Map/End.png", m_EndNodeTexture);
	m_BackgroundTexture = nullptr;
	Texture::LoadPNG("Content/Map/Background.png", m_BackgroundTexture);
}

MapScene::~MapScene()
{
	m_PossibleMapMovements.clear();
	m_SelectedNodeIndex = -1;
	for (size_t i = 0; i < 50; i++)
	{
		m_NodeButtons[i] = SDL_FRect{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	Texture::Destroy(m_EncounterAtlas);
	Texture::Destroy(m_CrossTexture);
	Texture::Destroy(m_StartNodeTexture);
	Texture::Destroy(m_EndNodeTexture);
	Texture::Destroy(m_BackgroundTexture);
}

void MapScene::OnEnter()
{
	int window_h = 0;
	SDL_GetWindowSize(&Services::GetWindow(), nullptr, &window_h);
	m_MapScrollOffset = window_h;

	m_CanSelectButton = false;
	m_ButtonPressCooldownTimer = c_ButtonPressCooldown;
	m_IsButtonRectsDirty = true;
	RecalculateButtonRects();
}

void MapScene::HandleEvent(const SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
	{
		m_IsButtonRectsDirty = true;
	}
	break;

	case SDL_EVENT_MOUSE_WHEEL:
	{
		int x = e.wheel.x;
		int y = e.wheel.y;

		if (e.wheel.direction == SDL_MouseWheelDirection::SDL_MOUSEWHEEL_FLIPPED)
		{
			x *= -1;
			y *= -1;
		}

		m_MapScrollOffset += y * c_MapScrollSpeed;
		m_IsButtonRectsDirty = true;
	}
	break;

	case SDL_EVENT_KEY_UP:
	{
		switch (e.key.key)
		{
		case SDLK_P:
		{
			m_WorldMap.GenerateNewMap(time(NULL), 0);
			m_IsButtonRectsDirty = true;
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
	if (m_IsButtonRectsDirty == false)
		return;

	int window_w = 0;
	int window_h = 0;
	SDL_GetWindowSize(&Services::GetWindow(), &window_w, &window_h);

	const float padding = SDL_max(c_EncounterImageWidth, c_EncounterImageHeight);
	const float totalWidth = c_MapWidth * c_EncounterImageWidth + (c_MapWidth * padding);
	const float totalHeight = c_MapLength * c_EncounterImageHeight + (c_MapLength * padding);
	float offset_x = (window_w / 2) - ((totalWidth / 2));
	float offset_y = (window_h / 2) - ((totalHeight / 3) * 2);

	int pos_x = m_WorldMap.GetCurrentNode().GetPosition().first;
	int pos_y = m_WorldMap.GetCurrentNode().GetPosition().second;
	offset_y -= ((pos_y * c_EncounterImageHeight) + (pos_y * padding)) - m_MapScrollOffset;

	int index = 0;
	for (size_t y = 0; y < c_MapLength; y++)
	{
		for (size_t x = 0; x < c_MapWidth; x++)
		{
			const MapNode& node = m_WorldMap.GetMapNode({ x, y });
			index = (y * c_MapWidth) + x;

			m_NodeButtons[index].x = offset_x + (x * c_EncounterImageWidth) + (x * padding);
			m_NodeButtons[index].y = offset_y + (y * c_EncounterImageHeight) + (y * padding);
			m_NodeButtons[index].w = c_EncounterImageWidth;
			m_NodeButtons[index].h = c_EncounterImageHeight;
		}
	}

	m_StartNodeDrawRect.w = 0.0f; m_StartNodeDrawRect.h = 0.0f;
	if (m_StartNodeTexture != nullptr)
	{
		m_StartNodeDrawRect.w = 256.0f;
		m_StartNodeDrawRect.h = 128.0f;
	}

	m_EndNodeDrawRect.w = 0.0f; m_EndNodeDrawRect.h = 0.0f;
	if (m_EndNodeTexture != nullptr)
	{
		m_EndNodeDrawRect.w = 256.0f;
		m_EndNodeDrawRect.h = 128.0f;
	}

	offset_y = m_MapScrollOffset;
	m_StartNodeDrawRect.x = (window_w / 2) - (m_StartNodeDrawRect.w / 2) - (c_EncounterImageWidth / 2);
	m_StartNodeDrawRect.y = m_NodeButtons[0].y - padding - m_StartNodeDrawRect.h;

	m_EndNodeDrawRect.x = (window_w / 2) - (m_EndNodeDrawRect.w / 2) - (c_EncounterImageWidth / 2);
	m_EndNodeDrawRect.y = m_NodeButtons[((c_MapLength - 1) * c_MapWidth)].y + m_EndNodeDrawRect.h + padding;

	m_IsButtonRectsDirty = false;
}

void MapScene::Update(const float& deltaTime)
{
	if (m_IsButtonRectsDirty)
	{
		RecalculateButtonRects();
	}

	constexpr const size_t mapSize = c_MapLength * c_MapWidth;
	m_SelectedNodeIndex = -1;

	if (m_CanSelectButton == false)
	{
		m_ButtonPressCooldownTimer -= deltaTime;

		if (m_ButtonPressCooldownTimer <= 0.0f)
		{
			m_CanSelectButton = true;
		}
	}

	m_WorldMap.GetPossibleSelectionNodesFromCurrentPosition(m_PossibleMapMovements);
	for (size_t m = 0; m < m_PossibleMapMovements.size(); m++)
	{
		int x = m_PossibleMapMovements[m]->GetPosition().first;
		int y = m_PossibleMapMovements[m]->GetPosition().second;

		int buttonIndex = (y * c_MapWidth) + x;

		SDL_FRect& buttonRect = m_NodeButtons[buttonIndex];

		if (y == c_MapLength)
		{
			buttonRect = m_EndNodeDrawRect;
		}

		if (Collision::PointInRect(m_InputManager.GetMouseX(), m_InputManager.GetMouseY(), buttonRect) &&
			m_InputManager.GetMouseButtonDown(Input::MOUSE_BUTTON::LEFT_BUTTON) &&
			m_CanSelectButton == true)
		{
			m_SelectedNodeIndex = buttonIndex;
			m_CanSelectButton = false;
			m_ButtonPressCooldownTimer = 0.5f;

			const MapNode& node = m_WorldMap.GetMapNode({ x, y });
			m_WorldMap.SetCurrentNode(node);

			switch (node.GetType())
			{
			case MapNode::ENCOUNTER_START: {} break;
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
}

void MapScene::Render(SDL_Renderer& renderer) const
{
	SDL_SetRenderDrawColorFloat(&renderer, 1.0f, 1.0f, 1.0f, 1.0f);
	SDL_RenderDebugText(&renderer, 10, 10, "SCENE_IDENTIFIER::MAP_SCENE");

	const int& currentXPosition = m_WorldMap.GetCurrentNode().GetPosition().first;
	const int& currentYPosition = m_WorldMap.GetCurrentNode().GetPosition().second;

	SDL_RenderTextureTiled(&renderer, m_BackgroundTexture, nullptr, 1.0f, nullptr);

	if (m_StartNodeTexture != nullptr)
	{
		SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
		SDL_RenderRect(&renderer, &m_StartNodeDrawRect);
		SDL_RenderTexture(&renderer, m_StartNodeTexture, nullptr, &m_StartNodeDrawRect);
	}

	if (m_EndNodeTexture != nullptr)
	{
		SDL_SetRenderDrawColor(&renderer, 255, 255, 255, 255);
		SDL_RenderRect(&renderer, &m_EndNodeDrawRect);
		SDL_RenderTexture(&renderer, m_EndNodeTexture, nullptr, &m_EndNodeDrawRect);
	}

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
			SDL_FRect drawRect = m_NodeButtons[index];

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
			if (currentYPosition < 0)
			{
				for (size_t i = 0; i < m_PossibleMapMovements.size(); i++)
				{
					int possible_x = m_PossibleMapMovements[i]->GetPosition().first;
					int possible_y = m_PossibleMapMovements[i]->GetPosition().second;

					if (possible_y == currentYPosition + 1)
					{
						int possibleMovementIndex = (possible_y * c_MapWidth) + possible_x;
						const SDL_FRect& endPointRect = m_NodeButtons[possibleMovementIndex];
						SDL_FPoint startPoint = { m_StartNodeDrawRect.x + (m_StartNodeDrawRect.w / 2), m_StartNodeDrawRect.y + (m_StartNodeDrawRect.h / 2) };
						SDL_FPoint endPoint = { endPointRect.x + (endPointRect.w / 2), endPointRect.y + (endPointRect.h / 2) };
						SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
						SDL_RenderLine(&renderer, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
					}
				}
			}

			SDL_RenderTexture(&renderer, m_EncounterAtlas, &srcRect, &drawRect);

			if (y < currentYPosition)
			{
				SDL_RenderTexture(&renderer, m_CrossTexture, nullptr, &drawRect);
			}
			else
			{
				if (y == currentYPosition)
				{
					if (x == currentXPosition)
					{
						srcRect.x = c_EncounterImageWidth;
						SDL_RenderTexture(&renderer, m_EncounterAtlas, &srcRect, &drawRect);
						SDL_RenderTexture(&renderer, m_PlayerIconTexture, nullptr, &drawRect);

						for (size_t i = 0; i < m_PossibleMapMovements.size(); i++)
						{
							int possible_x = m_PossibleMapMovements[i]->GetPosition().first;
							int possible_y = m_PossibleMapMovements[i]->GetPosition().second;

							if (possible_y == c_MapLength)
							{
								SDL_FPoint startPoint = { drawRect.x + (drawRect.w / 2), drawRect.y + (drawRect.h / 2) };
								SDL_FPoint endPoint = { m_EndNodeDrawRect.x + (m_EndNodeDrawRect.w / 2), m_EndNodeDrawRect.y + (m_EndNodeDrawRect.h / 2) };
								SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
								SDL_RenderLine(&renderer, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
							}
							else if (possible_y == currentYPosition + 1)
							{
								int possibleMovementIndex = (possible_y * c_MapWidth) + possible_x;
								const SDL_FRect& endPointRect = m_NodeButtons[possibleMovementIndex];
								SDL_FPoint startPoint = { drawRect.x + (drawRect.w / 2), drawRect.y + (drawRect.h / 2) };
								SDL_FPoint endPoint = { endPointRect.x + (endPointRect.w / 2), endPointRect.y + (endPointRect.h / 2) };
								SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
								SDL_RenderLine(&renderer, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
							}
						}
					}
					else
					{
						SDL_RenderTexture(&renderer, m_CrossTexture, nullptr, &drawRect);
					}
				}
				else
				{
					
				}
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
