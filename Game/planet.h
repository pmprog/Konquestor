
#pragma once

#include "includes.h"
#include "player.h"

class Planet
{

	public:
		ALLEGRO_COLOR PlanetColour;
		Player* OwnedBy;

		int Ships;
		int ProductionRate;
		float DefenceStats;

		Planet()
		{
			PlanetColour = al_map_rgb( (rand() % 128) + 64, (rand() % 128) + 64, (rand() % 128) + 64 );
			OwnedBy = nullptr;
			Ships = (rand() % 10) + 5;
			ProductionRate = (rand() % 6) + 2;
			DefenceStats = ((rand() % 4) / 10.0f) + 0.4f;
		};

};