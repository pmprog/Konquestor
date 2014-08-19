
#include "textbutton.h"


TextButton::TextButton( Control* Owner, std::string Text, TTFFont* Font ) : Control( Owner ), text( Text ), font( Font ), TextHAlign( HorizontalAlignment::Centre ), TextVAlign( VerticalAlignment::Centre )
{
	CanFocus = true;
}

TextButton::~TextButton()
{
}

void TextButton::EventOccured( Event* e )
{
	Control::EventOccured( e );

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::MouseDown )
	{
		// buttonclick->PlaySound();
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::MouseClick )
	{
		Event* ce = new Event();
		ce->Type = e->Type;
		memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
		ce->Data.Forms.EventFlag = FormEventType::ButtonClick;
		FRAMEWORK->PushEvent( ce );
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.RaisedBy == this && e->Data.Forms.EventFlag == FormEventType::KeyDown )
	{
		if( e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_SPACE || e->Data.Forms.KeyInfo.KeyCode == ALLEGRO_KEY_ENTER )
		{
			Event* ce = new Event();
			ce->Type = e->Type;
			memcpy( (void*)&(ce->Data.Forms), (void*)&(e->Data.Forms), sizeof( FRAMEWORK_FORMS_EVENT ) );
			ce->Data.Forms.EventFlag = FormEventType::ButtonClick;
			FRAMEWORK->PushEvent( ce );
		}
	}
}

void TextButton::OnRender()
{
	al_draw_filled_rectangle( 3,  3, Size.X - 2, Size.Y - 2, al_map_rgb( 160, 160, 160 ) );

	al_draw_line( 2, 4, Size.X - 2, 3, al_map_rgb( 220, 220, 220 ), 1 );
	al_draw_line( 2,  Size.Y - 4, Size.X - 2, Size.Y - 4, al_map_rgb( 80, 80, 80 ), 1 );
	al_draw_line( 2, Size.Y - 3, Size.X - 2, Size.Y - 3, al_map_rgb( 64, 64, 64 ), 1 );
	al_draw_rectangle( 3, 3, Size.X - 2, Size.Y - 2, al_map_rgb( 48, 48, 48 ), 1 );

	if( selectedDraw )
	{
		al_draw_rectangle( 1, 1, Size.X - 1, Size.Y - 1, al_map_rgb( 255, 255, 255 ), 2 );
	}

	if( mouseDepressed && mouseInside )
	{
		al_draw_rectangle( 1, 1, Size.X - 1, Size.Y - 1, al_map_rgb( 255, 255, 255 ), 2 );
	}
	
	int xpos;
	int ypos;

	switch( TextHAlign )
	{
		case HorizontalAlignment::Left:
			xpos = 0;
			break;
		case HorizontalAlignment::Centre:
			xpos = (Size.X / 2) - (font->GetFontWidth( text ) / 2);
			break;
		case HorizontalAlignment::Right:
			xpos = Size.X - font->GetFontWidth( text );
			break;
	}

	switch( TextVAlign )
	{
		case VerticalAlignment::Top:
			ypos = 0;
			break;
		case VerticalAlignment::Centre:
			ypos = (Size.Y / 2) - (font->GetFontHeight() / 2);
			break;
		case VerticalAlignment::Bottom:
			ypos = Size.Y - font->GetFontHeight();
			break;
	}

	font->DrawString( xpos, ypos, text, FontHAlign::LEFT, ForegroundColour );
}

void TextButton::Update()
{
	if( IsFocused() )
	{
		selectedTimer = (selectedTimer + 1) % TEXTBUTTON_SELECT_TOGGLE_TIME;
		if( selectedTimer == 0 )
		{
			selectedDraw = !selectedDraw;
		}
	} else {
		selectedDraw = false;
	}
}

void TextButton::UnloadResources()
{
}

std::string TextButton::GetText()
{
	return text;
}

void TextButton::SetText( std::string Text )
{
	text = Text;
}
