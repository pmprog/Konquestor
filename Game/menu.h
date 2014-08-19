
#pragma once

#include "includes.h"

class Menu : public Stage
{
	private:
		TTFFont* titleFont;
		TTFFont* menuFont;

		int selectedItem;

  public:
		Menu();
		~Menu();

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
