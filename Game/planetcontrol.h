
#pragma once

#include "planet.h"
#include "player.h"
#include "../Framework/Forms/control.h"

class PlanetControl : public Control
{
	private:
		Planet* viewingPlanet;
		Player* viewingAs;
		Vector2 lastSize;

		Label* localPlanetTitle;
		Label* localPlanetShipsLabel;
		Label* localPlanetShips;
		Label* localPlanetProductionLabel;
		Label* localPlanetProduction;
		Label* localPlanetDefenceLabel;
		Label* localPlanetDefence;

		void UpdateInfo();

	protected:
		virtual void OnRender();

	public:
		PlanetControl( Control* Owner, TTFFont* Font );
		virtual ~PlanetControl();

		void SetPlanet( Planet* View );
		void SetPlayer( Player* View );

		virtual void EventOccured( Event* e );
		virtual void Update();
		virtual void UnloadResources();
};
