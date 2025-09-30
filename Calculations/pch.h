#pragma once

// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

//C++ Includes
#include <iostream>
#include <string>
#include <cassert>
#include <algorithm>
#include <random>

//rapidjson
#define RAPIDJSON_NOMEMBERITERATORCLASS
#include <rapidjson/rapidjson.h>

//SDL Includes
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

//System
constexpr const int		TARGET_FPS = 60;
constexpr const float	MS_PER_FRAME = 1.0f / TARGET_FPS;
constexpr const char*	PROJECT_NAME = "Mini Jam 183 ";
constexpr const char*	VERSION_STRING = "0.0";
constexpr const char*	WINDOW_TITLE = "Calculations";
constexpr const int		INITIAL_WINDOW_WIDTH = 1280;
constexpr const int		INITIAL_WINDOW_HEIGHT = 720;
constexpr const float	CLEAR_COLOUR[4] = { 0.54509803921568620f, 0.42745098039215684f, 0.61176470588235300f, 1.0f };
constexpr const float	TARGET_RESOLUTION_X = 1920;
constexpr const float	TARGET_RESOLUTION_Y = 1080;


//Game
constexpr const int		INITIAL_ENTITY_HEALTH = 10;
constexpr const int		INITIAL_PLAYER_HEALTH = 30;
constexpr const int		INITIAL_PLAYER_GOLD = 10;
constexpr const int		INITIAL_PLAYER_NUMBER_HAND_SIZE = 4;
constexpr const int		INITIAL_PLAYER_OPERAND_HAND_SIZE = 2;
constexpr const int		INITIAL_ENEMY_ROLL_RANGE = 2;
constexpr const int		INITIAL_ENEMY_BASE_DAMAGE = 5;

constexpr const char*	ENEMY_DEFINITION_DIRECTORY = "Content\\Data\\Definitions\\Enemies\\";
constexpr const char*	DEFAULT_CONTENT_DIRECTORY = "Content\\";

#endif //PCH_H