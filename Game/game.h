
#pragma once

#include "includes.h"
#include "player.h"
#include "planet.h"
#include "../Framework/Primitives/angle.h"

#define MAP_WIDTH		8
#define MAP_HEIGHT	7
#define MAP_GRIDSIZE	60

class Game : public Stage
{
	private:
		TTFFont* menuFont;
		TTFFont* detailFont;
		std::vector<Player*> playerList;
		int currentPlayer;

		Planet* galacticMap[ MAP_WIDTH * MAP_HEIGHT ];
		std::vector<Planet*> planetList;

		int gridSelectX;
		int gridSelectY;

		Angle* selectSin;

		std::vector<ALLEGRO_THREAD*> AIThreads;

		Form* localInputForm;
		Label* localPlanetTitle;
		Label* localPlanetShips;
		Label* localPlanetProduction;
		Label* localPlanetDefence;
		ListBox* localPlanetInFlightList;
		TextButton* localPlanetLaunch;
		TextButton* localPlanetEndTurn;

		Form* waitInputForm;
		Label* waitInputLabel;
		Form* activeInputForm;

  public:
		bool StopAI;

		Game( int Planets, std::vector<Player*> Players );
		~Game();

		void NextPlayer();
		Player* GetCurrentPlayer();

		static void* AIThread(ALLEGRO_THREAD* Thread, void* PlayerPtr);

    // Stage control
    virtual void Begin();
    virtual void Pause();
    virtual void Resume();
    virtual void Finish();
    virtual void EventOccurred(Event *e);
    virtual void Update();
    virtual void Render();
		virtual bool IsTransition();
};
