
#pragma once

#include "includes.h"

struct PlayerType
{
	enum Type
	{
		LocalHuman,
		NetworkHuman,
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

};