
#pragma once

#include "control.h"
#include "../Display/fontcache.h"

class TextButton : public Control
{

	private:
		std::string text;
		TTFFont* font;

	protected:
		virtual void OnRender();

	public:
		HorizontalAlignment TextHAlign;
		VerticalAlignment TextVAlign;

		TextButton(Control* Owner, std::string Text, TTFFont* Font);
		virtual ~TextButton();

		virtual void EventOccured(Event* e);
		virtual void Update();
		virtual void UnloadResources();

		std::string GetText();
		void SetText( std::string Text );
};

