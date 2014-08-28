
#pragma once

#include "includes.h"
#include "player.h"
#include "planet.h"
#include "../Framework/Primitives/angle.h"
#include "planetcontrol.h"

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
		int turnChangesAtPlayer;

		Planet* galacticMap[ MAP_WIDTH * MAP_HEIGHT ];
		std::vector<Planet*> planetList;

		int gridSourceX;
		int gridSourceY;

		int gridSelectX;
		int gridSelectY;

		Angle* selectSin;

		std::vector<ALLEGRO_THREAD*> AIThreads;

		Form* localInputForm;
		GraphicButton* localInfoButton;
		GraphicButton* localTransfersButton;
		Control* localInfoTab;
		Control* localTransferTab;
		PlanetControl* localPlanetInfoSource;
		PlanetControl* localPlanetInfoTarget;
		Label* localPlanetTargetSliderValue;
		HScrollBar* localPlanetTargetSlider;
		ListBox* localPlanetInFlightList;
		TextButton* localPlanetLaunch;
		TextButton* localPlanetCancelLaunch;
		TextButton* localPlanetSendShips;
		TextButton* localPlanetEndTurn;

		Form* waitInputForm;
		Label* waitInputLabel;
		Form* activeInputForm;

		void DisplaySelectedPlanetInfo();

  public:
		int TurnNumber;
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
