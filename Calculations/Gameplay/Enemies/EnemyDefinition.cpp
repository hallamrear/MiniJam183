#include "pch.h"
#include "EnemyDefinition.h"
#include <rapidjson/document.h>
#include <sstream>
#include <vector>

EnemyDefinition::EnemyDefinition()
{
	DefinitionName = "UNDEFINED";
	SpriteLocation = "UNDEFINED";
	AttackAnimationCount = -1;
	MaxFrameCount = -1;
	FrameDetails = std::vector<AnimationDetails>();
	MaxHealth = INITIAL_ENTITY_HEALTH;
	BaseDamage = INITIAL_ENEMY_BASE_DAMAGE;
	RollRange = INITIAL_ENEMY_ROLL_RANGE;
}

EnemyDefinition::~EnemyDefinition()
{
	FrameDetails.clear();
	DefinitionName = "";
	SpriteLocation = "";
	AttackAnimationCount = 0;
	BaseDamage = 0;
	RollRange = 0;
	MaxHealth = 0;
}

bool EnemyDefinition::LoadDefinitionFromFile(const File::Filepath& filepath)
{
	std::stringstream buffer;
	if (File::LoadFileIntoStringStream(filepath, buffer) == false)
	{
		SDL_Log("Error loading enemy data file.");
		return false;
	}

	rapidjson::Document jsonDoc;
	jsonDoc.Parse(buffer.str().c_str());

	if (jsonDoc.HasParseError())
	{
		SDL_Log("Error loading enemy data file.");
		rapidjson::ParseErrorCode e = jsonDoc.GetParseError();
		SDL_Log("Error parse code: %i", e);
		return false;
	}

	int definitionCount = jsonDoc.MemberCount();
	if (definitionCount > 0)
	{
		for (rapidjson::Value::ConstMemberIterator itr = jsonDoc.MemberBegin(); itr != jsonDoc.MemberEnd(); ++itr)
		{
			if (itr)
			{
				const char* identifier = itr->name.GetString();
				SDL_Log("Loading enemy definition: %s", identifier);

				DefinitionName = identifier;

				if (itr->value.HasMember("Parent"))
				{
					if (itr->value["Parent"].IsNull() == false)
					{
						//TODO : Implement definition parenting.
						//definition = FindDefinitionByName(itr->value.GetString());
					}
				}

				if (itr->value.HasMember("Sprite"))
				{
					if (itr->value["Sprite"].IsString())
					{
						 SpriteLocation = itr->value["Sprite"].GetString();
					}
				}

				if (itr->value.HasMember("MaxHealth"))
				{
					if (itr->value["MaxHealth"].IsInt())
					{
						MaxHealth = itr->value["MaxHealth"].GetInt();
					}
				}

				if (itr->value.HasMember("BaseDamage"))
				{
					if (itr->value["BaseDamage"].IsInt())
					{
						BaseDamage = itr->value["BaseDamage"].GetInt();
					}
				}

				if (itr->value.HasMember("RollRange"))
				{
					if (itr->value["RollRange"].IsInt())
					{
						RollRange = itr->value["RollRange"].GetInt();
					}
				}

				if (itr->value.HasMember("MaxFrameCount"))
				{
					if (itr->value["MaxFrameCount"].IsInt())
					{
						MaxFrameCount = itr->value["MaxFrameCount"].GetInt();
					}
				}

				if (itr->value.HasMember("AttackAnimationCount"))
				{
					if (itr->value["AttackAnimationCount"].IsInt())
					{
						AttackAnimationCount = itr->value["AttackAnimationCount"].GetInt();
					}
				}

				if (itr->value.HasMember("AnimationData"))
				{
					if (itr->value["AnimationData"].IsArray())
					{
						rapidjson::GenericArray frameDataArray = itr->value["AnimationData"].GetArray();

						for (auto& frameData : frameDataArray)
						{
							AnimationDetails data;

							if (frameData.HasMember("FrameCount"))
							{
								data.FrameCount = frameData["FrameCount"].GetInt();
							}

							if (frameData.HasMember("LengthInSecs"))
							{
								data.Duration = frameData["LengthInSecs"].GetFloat();
							}

							if (frameData.HasMember("IsLooping"))
							{
								data.IsLooping = frameData["IsLooping"].GetBool();
							}

							FrameDetails.push_back(data);
						}
					}
				}
			}
		}
	}

	return true;
}
