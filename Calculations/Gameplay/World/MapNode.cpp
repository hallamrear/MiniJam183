#include "pch.h"
#include "MapNode.h"

MapNode::MapNode()
{
	m_Type = ENCOUNTER_TYPE::ENCOUNTER_UNKNOWN;
	m_Position.first = 0;
	m_Position.second = 0;
}

MapNode::MapNode(const ENCOUNTER_TYPE& encounterType, const Position& position)
{
	m_Type = encounterType;
	m_Position = position;
}

MapNode::~MapNode()
{
	Reset();
}

void MapNode::Reset()
{
	m_Type = ENCOUNTER_TYPE::ENCOUNTER_UNKNOWN;
	m_Position.first = 0;
	m_Position.second = 0;
}

const MapNode::ENCOUNTER_TYPE& MapNode::GetType() const
{
	return m_Type;
}

const Position& MapNode::GetPosition() const
{
	return m_Position;
}

bool MapNode::operator==(const MapNode& rhs) const
{
	return (this->m_Position.first == rhs.GetPosition().first) && (this->m_Position.second == rhs.GetPosition().second);
}
