#pragma once

typedef std::pair<int, int> Position;

class MapNode
{
public:
	enum ENCOUNTER_TYPE : int
	{
		ENCOUNTER_UNKNOWN = 0,
		ENCOUNTER_START,
		ENCOUNTER_ENEMY,
		ENCOUNTER_SHOP,
		ENCOUNTER_REST,
		ENCOUNTER_EVENT,
		ENCOUNTER_ELITE,
		ENCOUNTER_BOSS,
	};

private:
protected:
	ENCOUNTER_TYPE m_Type;
	Position m_Position;
	std::vector<const MapNode*> m_Nodes;

public:
	MapNode();
	MapNode(const ENCOUNTER_TYPE& encounterType, const Position& position);
	virtual ~MapNode();

	void Reset();
	void AddForwardNode(const MapNode& node);
	const std::vector<const MapNode*>& GetNodes() const;
	const ENCOUNTER_TYPE& GetType() const;
	const Position& GetPosition() const;
	void Render(SDL_Renderer& renderer) const;

	bool operator==(const MapNode& rhs) const;
};

