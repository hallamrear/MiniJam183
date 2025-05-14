#include "pch.h"
#include "WorldMap.h"
#include <Gameplay/World/MapNode.h>

#include <Windows.h>
#include <ConsoleApi2.h>

WorldMap::WorldMap() : m_CurrentNode(m_Start)
{
	m_Start = MapNode(MapNode::ENCOUNTER_START, Position(floor(c_MapWidth / 2), -1));
	m_End = MapNode(MapNode::ENCOUNTER_BOSS, Position(floor(c_MapWidth / 2), c_MapLength));
	ResetMap();
}

WorldMap::~WorldMap()
{
	ResetMap();
}

void WorldMap::Print(const Path& path)
{
	std::cout << "______ b ______" << std::endl;

	for (size_t y = 0; y < c_MapLength; y++)
	{
		for (size_t x = 0; x < c_MapWidth; x++)
		{
			MapNode::ENCOUNTER_TYPE type = m_Map[x][y].GetType();

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  FOREGROUND_BLUE);

			for (size_t i = 0; i < path.size(); i++)
			{
				const Position& pathPos = path[i].GetPosition();
				int p_x = pathPos.first;
				int p_y = pathPos.second;

				if ((x == p_x) && (y == p_y))
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					break;
				}
			}

			switch (type)
			{
			case MapNode::ENCOUNTER_START:  std::cout << " b "; break;
			case MapNode::ENCOUNTER_ENEMY:	std::cout << " E "; break;
			case MapNode::ENCOUNTER_SHOP:	std::cout << " S "; break;
			case MapNode::ENCOUNTER_REST:	std::cout << " R "; break;
			case MapNode::ENCOUNTER_EVENT:	std::cout << " V "; break;
			case MapNode::ENCOUNTER_ELITE:	std::cout << " L "; break;
			case MapNode::ENCOUNTER_BOSS:   std::cout << " e "; break;

			case MapNode::ENCOUNTER_UNKNOWN:   std::cout << " + "; break;

			default:
				assert("Should not have hit here.");
				break;
			}

		}
		std::cout << std::endl;
	}

	std::cout << "______ e ______" << std::endl;
}

void WorldMap::ResetMap()
{
	m_Start.Reset();
	m_End.Reset();

	m_Start = MapNode(MapNode::ENCOUNTER_START, Position(floor(c_MapWidth / 2), -1));
	m_End = MapNode(MapNode::ENCOUNTER_BOSS, Position(floor(c_MapWidth / 2), c_MapLength));

	for (size_t y = 0; y < c_MapLength; y++)
	{
		for (size_t x = 0; x < c_MapWidth; x++)
		{
			m_Map[x][y].Reset();
		}
	}
}

void WorldMap::GenerateNewMap(const unsigned int& seed, const unsigned int& strength)
{
	srand(seed);

	ResetMap();

	SDL_Log("New Map Generation %ui", seed);

	int roomSelectionRange = (int)MapNode::ENCOUNTER_TYPE::ENCOUNTER_BOSS - (int)MapNode::ENCOUNTER_TYPE::ENCOUNTER_START;

	for (size_t y = 0; y < c_MapLength; y++)
	{
		for (size_t x = 0; x < c_MapWidth; x++)
		{
			int r = rand() % (roomSelectionRange - 1) + 2;
			m_Map[x][y] = MapNode(MapNode::ENCOUNTER_TYPE(r), {x, y});
		}
	}

	m_Map[m_Start.GetPosition().first][m_Start.GetPosition().second + 1] = MapNode(MapNode::ENCOUNTER_TYPE::ENCOUNTER_ENEMY, { m_Start.GetPosition().first, m_Start.GetPosition().second + 1 });

	std::vector<Path> paths = std::vector<Path>();
	Path positions = Path();
	Position currentStep = { 0, 0 };
	int possiblePaths = 3;
	for (size_t i = 0; i < possiblePaths; i++)
	{
		positions.clear();

		MapNode lastNode = m_Start;
		currentStep = m_Start.GetPosition();
		currentStep.second++;
		positions.push_back(m_Map[currentStep.first][currentStep.second]);
		lastNode = m_Map[currentStep.first][currentStep.second];
		currentStep.second++;

		while (currentStep.second < c_MapLength)
		{
			int dir = rand() % 3;
			switch (dir)
			{
				//Left
				case 0:
				{ 
					currentStep.first - 1 < 0 ? currentStep.first++ : currentStep.first--;
				}; 
				break;

				//Straight
				case 1: { }; break;

				//Right
				case 2: 
				{
					(currentStep.first + 1 > (c_MapWidth - 1)) ? currentStep.first-- : currentStep.first++;
				};
				break;

				default: break;	
			}
			
			positions.push_back(m_Map[currentStep.first][currentStep.second]);
			currentStep.second++;
			lastNode = m_Map[currentStep.first][currentStep.second];
		}

		Print(positions);
		paths.push_back(positions);
	}

	for (size_t y = 0; y < c_MapLength; y++)
	{
		for (size_t x = 0; x < c_MapWidth; x++)
		{
			m_Map[x][y].Reset();
		}
	}

	Path mergedPaths = Path();
	for (auto& itr : paths)
	{
		for (size_t i = 0; i < itr.size(); i++)
		{
			auto found = std::find(mergedPaths.begin(), mergedPaths.end(), itr[i]);
			if (found == mergedPaths.end())
			{
				mergedPaths.push_back(itr[i]);
				const Position& pos = itr[i].GetPosition();
				m_Map[pos.first][pos.second] = itr[i];
			}
		}
	}

	Print(mergedPaths);
}

const MapNode& WorldMap::GetMapNode(const Position& position) const
{
	return m_Map[position.first][position.second];
}

const MapNode& WorldMap::GetCurrentNode() const
{
	return m_CurrentNode;
}

void WorldMap::SetCurrentNode(const MapNode& node)
{
	m_CurrentNode = node;
}

void WorldMap::GetPossibleSelectionNodes(std::vector<const MapNode*>& nodes, const std::pair<int, int>& currentPosition) const
{
	nodes.clear();

	if ((currentPosition.second + 1) >= c_MapLength)
	{
		nodes.push_back(&m_End);
		return;
	}

	for (int i = 0; i < 3; i++)
	{
		auto& node = m_Map[currentPosition.first + i - 1][currentPosition.second + 1];

		if (node.GetType() != MapNode::ENCOUNTER_TYPE::ENCOUNTER_UNKNOWN &&
			node.GetType() != MapNode::ENCOUNTER_TYPE::ENCOUNTER_START)
		{
			nodes.push_back(&node);
		}
	}
}

void WorldMap::GetPossibleSelectionNodesFromCurrentPosition(std::vector<const MapNode*>& nodes) const
{
	GetPossibleSelectionNodes(nodes, m_CurrentNode.GetPosition());
}