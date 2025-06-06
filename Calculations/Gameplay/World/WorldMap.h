#pragma once
#include <Gameplay/World/MapNode.h>

typedef std::vector<MapNode> Path;
constexpr const int c_MapWidth = 5;
constexpr const int c_MapLength = 10;

class WorldMap
{
private:
	MapNode m_Map[c_MapWidth][c_MapLength];
	MapNode m_Start;
	MapNode m_End;
	MapNode& m_CurrentNode;

public:
	WorldMap();
	~WorldMap();

	void ResetMap(); 
	void Print(const Path& path);
	void GenerateNewMap(const unsigned int& seed, const unsigned int& strength);
	const MapNode& GetMapNode(const Position& position) const;
	void SetCurrentNode(const MapNode& node);
	const MapNode& GetCurrentNode() const;

	void GetPossibleSelectionNodesFromCurrentPosition(std::vector<const MapNode*>& nodes) const;
	void GetPossibleSelectionNodes(std::vector<const MapNode*>& nodes, const std::pair<int, int>& currentPosition) const;
};

