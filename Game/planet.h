
#pragma once

#include "includes.h"
#include "player.h"
#include "../Framework/Primitives/strings.h"

class Planet
{

	public:
		std::string Name;
		ALLEGRO_COLOR PlanetColour;
		Player* OwnedBy;

		int Ships;
		int ProductionRate;
		float DefenceStats;

		int GridX;
		int GridY;

		Planet()
		{
			PlanetColour = al_map_rgb( (rand() % 128) + 64, (rand() % 128) + 64, (rand() % 128) + 64 );
			OwnedBy = nullptr;
			Ships = (rand() % 10) + 5;
			ProductionRate = (rand() % 6) + 2;
			DefenceStats = ((rand() % 4) / 10.0f) + 0.4f;
		};

		void GenerateName( int PlanetIndex )
		{
			Name.clear();

			switch( rand() % 6 )
			{
				case 0:
					Name.append("Ea");
					break;
				case 1:
					Name.append("Ko");
					break;
				case 2:
					Name.append("B'");
					break;
				case 3:
					Name.append("Sa");
					break;
				case 4:
					Name.append("No");
					break;
				case 5:
					Name.append("T'");
					break;
			}

			switch( rand() % 6 )
			{
				case 0:
					Name.append("r");
					break;
				case 1:
					Name.append("rh");
					break;
				case 2:
					Name.append("rup");
					break;
				case 3:
					Name.append("mow");
					break;
				case 4:
					Name.append("xxo");
					break;
				case 5:
					Name.append("le");
					break;
			}

			switch( rand() % 4 )
			{
				case 0:
					Name.append("th");
					break;
				case 1:
					Name.append("al");
					break;
				case 2:
					Name.append("'r");
					break;
				case 3:
					break;
			}
			Name.append(" [");
			Name.append(Strings::FromNumber(PlanetIndex));
			Name.append(" ]");
		};

		// Combat Calculations
		//  Attacking Ships: Min( Attacking, Max( 0, Attacking - (Defending * (1 + (Defence% / 3))) ) )
		//  Defending Ships: Min( Defending, Max( 0, (Defending * (1 + (Defence% / 3))) - Attacking ) )
};