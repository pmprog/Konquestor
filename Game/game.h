
#pragma once

#include "includes.h"
#include "player.h"
#include "planet.h"

#define MAP_WIDTH		8
#define MAP_HEIGHT	7

class Game : public Stage
{
	private:
		TTFFont* menuFont;
		std::vector<Player*> playerList;
		int currentPlayer;

		Planet* galacticMap[ MAP_WIDTH * MAP_HEIGHT ];
		std::vector<Planet*> planetList;

  public:

		Game( int Planets, std::vector<Player*> Players );
		~Game();

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
