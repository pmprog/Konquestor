
#pragma once

#include "includes.h"
#include "player.h"

class NewLocalGame : public Stage
{
	private:
		std::vector<Player*> players;

		TTFFont* titleFont;
		TTFFont* menuFont;

		Form* newGameForm;

  public:
		

		NewLocalGame();
		~NewLocalGame();

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
