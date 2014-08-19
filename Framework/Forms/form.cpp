#include "form.h"

Form::Form() : Control( nullptr )
{
}

Form::~Form()
{
}

void Form::EventOccured( Event* e )
{
	Control::EventOccured( e );
}

void Form::OnRender()
{
}

void Form::Update()
{
	Control::Update();
	ResolveLocation();
}

void Form::UnloadResources()
{
	Control::UnloadResources();
}
