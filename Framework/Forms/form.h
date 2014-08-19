
#pragma once

#include "control.h"

class Form : public Control
{

	protected:
		virtual void OnRender();

	public:
		Form();
		virtual ~Form();

		virtual void EventOccured( Event* e );
		virtual void Update();
		virtual void UnloadResources();
};

