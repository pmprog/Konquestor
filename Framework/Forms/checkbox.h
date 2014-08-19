
#pragma once

#include "control.h"

class CheckBox : public Control
{

	private:
		ALLEGRO_BITMAP* imagechecked;
		ALLEGRO_BITMAP* imageunchecked;

		void LoadResources();

	protected:
		virtual void OnRender();

	public:
		bool Checked;

		CheckBox( Control* Owner );
		virtual ~CheckBox();

		virtual void EventOccured( Event* e );
		virtual void Update();
		virtual void UnloadResources();
};
