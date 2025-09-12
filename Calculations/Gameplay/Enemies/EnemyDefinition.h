#pragma once
#include <System/File.h>
#include <Graphics/Animation.h>

class EnemyDefinition
{
protected:
public:
	std::string DefinitionName = "UNDEFINED";
	std::string SpriteLocation = "UNDEFINED";
	int MaxFrameCount;
	int AttackAnimationCount;
	std::vector<AnimationDetails> FrameDetails;
	int BaseDamage;
	int RollRange;
	int MaxHealth;

	EnemyDefinition();
	~EnemyDefinition();

	bool LoadDefinitionFromFile(const File::Filepath& filepath);
};