
#pragma once

#include "includes.h"

#ifndef Game
class Game;
#endif

struct PlayerType
{
	enum Type
	{
		LocalHuman,
		RemoteHuman,
		LocalComputer,
		RemoteComputer
	};
};

class Player
{

	public:
		std::string Name;
		PlayerType::Type Interaction;
		ALLEGRO_COLOR Colour;
		Game* CurrentGame;
};