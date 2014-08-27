
#pragma once

#include "planet.h"
#include "player.h"
#include "../Framework/Forms/control.h"

class PlanetControl : public Control
{
	private:
		Planet* viewingPlanet;
		Planet* targetPlanet;
		Player* viewingAs;
		Vector2 lastSize;

		Label* localPlanetTitle;
		Label* localPlanetShipsLabel;
		Label* localPlanetShips;
		Label* localPlanetProductionLabel;
		Label* localPlanetProduction;
		Label* localPlanetDefenceLabel;
		Label* localPlanetDefence;

		Label* localPlanetTargetTurnsLabel;
		Label* localPlanetTargetTurns;

		void UpdateInfo();

	protected:
		virtual void OnRender();

	public:
		PlanetControl( Control* Owner, TTFFont* Font, bool ShowTurns );
		virtual ~PlanetControl();

		void SetPlanet( Planet* View );
		void SetTarget( Planet* View );
		void SetPlayer( Player* View );

		virtual void EventOccured( Event* e );
		virtual void Update();
		virtual void UnloadResources();
};
