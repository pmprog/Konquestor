
#include "fadeout.h"
#include "../framework.h"

TransitionFadeOut::TransitionFadeOut( Stage* Target, ALLEGRO_COLOR Source, int Frames )
{
	targetStage = Target;
	transitionFrom = Source;
	transitionFrames = Frames;
	currentFrame = 0;
	targetRender = 0;
}

TransitionFadeOut::~TransitionFadeOut()
{
	if( targetRender != 0 )
	{
		al_destroy_bitmap( targetRender );
	}
}

void TransitionFadeOut::Begin()
{
	targetRender = al_create_bitmap( DISPLAY->GetWidth(), DISPLAY->GetHeight() );
	DISPLAY->SetTarget( targetRender );
	FRAMEWORK->ProgramStages->Previous()->Render();
	DISPLAY->ClearTarget();
}

void TransitionFadeOut::Pause()
{
}

void TransitionFadeOut::Resume()
{
}

void TransitionFadeOut::Finish()
{
}

void TransitionFadeOut::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_DOWN || e->Type == EVENT_MOUSE_DOWN )
	{
		//currentFrame--;
		FinishTransition();
	}
}

void TransitionFadeOut::Update()
{
	currentFrame++;
	if( currentFrame >= transitionFrames )
	{
		FinishTransition();
	}
}

void TransitionFadeOut::Render()
{
	//targetStage->Render();
	al_draw_bitmap( targetRender, 0, 0, 0 );
	transitionFrom.a = (float)currentFrame / (float)transitionFrames;
	if( transitionFrom.a > 1.0f )
	{
		transitionFrom.a = 1.0f;
	}
	al_draw_filled_rectangle( 0, 0, DISPLAY->GetWidth(), DISPLAY->GetHeight(), transitionFrom );
}

void TransitionFadeOut::FinishTransition()
{
	Stage* t = targetStage;
	delete Framework::System->ProgramStages->Pop();
	Framework::System->ProgramStages->Push( t );
}

bool TransitionFadeOut::IsTransition()
{
	return true;
}
