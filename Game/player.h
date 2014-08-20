
#pragma once

#include "includes.h"

#ifndef Game
class Game;
#endif

#ifndef Planet
class Planet;
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

class PlayerTransit
{
	public:
		int Ships;
		Planet* Target;
		int ArrivesOnTurn;
};

class Player
{

	public:
		std::string Name;
		PlayerType::Type Interaction;
		ALLEGRO_COLOR Colour;
		Game* CurrentGame;
};