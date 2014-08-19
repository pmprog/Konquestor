
#include "newlocal.h"
#include "../Framework/Primitives/strings.h"

NewLocalGame::NewLocalGame()
{
	titleFont = FontCache::LoadFont( "resources/game.ttf", 128 );
	menuFont = FontCache::LoadFont( "resources/game.ttf", 48 );
	DISPLAY->MouseVisible( true );
	selectedItem = 0;

	newGameForm = new Form();
	newGameForm->Location.X = 10;
	newGameForm->Size.X = 780;
	newGameForm->Location.Y = 100;
	newGameForm->Size.Y = 370;

	Label* l = new Label( newGameForm, "Name:", menuFont );
	l->Location.X = 4;
	l->Location.Y = 4;
	l->Size.X = menuFont->GetFontWidth("Name:");
	l->Size.Y = menuFont->GetFontHeight() + 6;
	l->TextVAlign = VerticalAlignment::Centre;

	TextEdit* te = new TextEdit( newGameForm, "", menuFont );
	te->Name = "EDIT_NAME";
	te->BackgroundColour = al_map_rgb( 220, 220, 220 );
	te->ForegroundColour = al_map_rgb( 0, 0, 0 );
	te->Location.X = 8 + l->Size.X;
	te->Location.Y = 4;
	te->Size.X = 500 - te->Location.X;
	te->Size.Y = menuFont->GetFontHeight() + 6;
	te->TextVAlign = VerticalAlignment::Centre;

	l = new Label( newGameForm, "Human:", menuFont );
	l->Location.X = 510;
	l->Location.Y = 4;
	l->Size.X = menuFont->GetFontWidth("Human:");
	l->Size.Y = menuFont->GetFontHeight() + 6;
	l->TextVAlign = VerticalAlignment::Centre;

	CheckBox* cb = new CheckBox( newGameForm );
	cb->Name = "CHECK_HUMAN";
	cb->Location.X = 510 + l->Size.X;
	cb->Location.Y = 4;
	cb->Size.X = l->Size.Y;
	cb->Size.Y = l->Size.Y;
	cb->Checked = true;

	TextButton* tb = new TextButton( newGameForm, "Add", menuFont );
	tb->Name = "BUTTON_ADD";
	tb->Location.X = cb->Location.X + cb->Size.X + 4;
	tb->Location.Y = 4;
	tb->Size.X = 776 - tb->Location.X;
	tb->Size.Y = l->Size.Y;
	tb->TextHAlign = HorizontalAlignment::Centre;
	tb->TextVAlign = VerticalAlignment::Centre;

	l = new Label( newGameForm, "Players:", menuFont );
	l->Location.X = 4;
	l->Size.X = 772;
	l->Location.Y = tb->Location.Y + tb->Size.Y + 4;
	l->Size.Y = menuFont->GetFontHeight() + 2;
	l->TextVAlign = VerticalAlignment::Centre;

	ListBox* lb = new ListBox( newGameForm );
	lb->Name = "LIST_PLAYERS";
	lb->BackgroundColour = al_map_rgb( 220, 220, 220 );
	lb->ForegroundColour = al_map_rgb( 0, 0, 0 );
	lb->Location.X = 4;
	lb->Size.X = 772;
	lb->Location.Y = l->Location.Y + l->Size.Y + 4;
	lb->Size.Y = 200;

	l = new Label( newGameForm, "Neutral Planets:", menuFont );
	l->Location.X = 4;
	l->Location.Y = lb->Location.Y + lb->Size.Y + 4;
	l->Size.X = menuFont->GetFontWidth("Neutral Planets:");
	l->Size.Y = menuFont->GetFontHeight() + 2;
	l->TextVAlign = VerticalAlignment::Centre;

	HScrollBar* hs = new HScrollBar( newGameForm );
	hs->Name = "SCROLL_PLANETS";
	hs->Location.X = l->Location.X + l->Size.X + 4;
	hs->Location.Y = l->Location.Y + 4;

	l = new Label( newGameForm, "4", menuFont );
	l->Name = "LABEL_PLANETS";
	l->Size.X = menuFont->GetFontWidth("000");
	l->Size.Y = menuFont->GetFontHeight() + 2;
	l->Location.X = 776 - l->Size.X;
	l->Location.Y = lb->Location.Y + lb->Size.Y + 4;
	l->TextVAlign = VerticalAlignment::Centre;
	l->TextHAlign = HorizontalAlignment::Centre;

	hs->Size.X = l->Location.X - hs->Location.X - 4;
	hs->Size.Y = l->Size.Y - 8;
	hs->Minimum = 2;
	hs->Maximum = 9;
	hs->Value = 4;
	hs->BackgroundColour = al_map_rgb( 0, 0, 0 );


	tb = new TextButton( newGameForm, "Back", menuFont );
	tb->Name = "BUTTON_BACK";
	tb->Size.X = menuFont->GetFontWidth("Back") + 24;
	tb->Size.Y = menuFont->GetFontHeight() + 6;
	tb->Location.X = 4;
	tb->Location.Y = 366 - tb->Size.Y;
	tb->TextHAlign = HorizontalAlignment::Centre;
	tb->TextVAlign = VerticalAlignment::Centre;

	tb = new TextButton( newGameForm, "Start", menuFont );
	tb->Name = "BUTTON_START";
	tb->Size.X = menuFont->GetFontWidth("Start") + 24;
	tb->Size.Y = menuFont->GetFontHeight() + 6;
	tb->Location.X = 776 - tb->Size.X;
	tb->Location.Y = 366 - tb->Size.Y;
	tb->TextHAlign = HorizontalAlignment::Centre;
	tb->TextVAlign = VerticalAlignment::Centre;
}

NewLocalGame::~NewLocalGame()
{
	FontCache::UnloadFont( titleFont );
	FontCache::UnloadFont( menuFont );
}

void NewLocalGame::Begin()
{
}

void NewLocalGame::Pause()
{
}

void NewLocalGame::Resume()
{
}

void NewLocalGame::Finish()
{
}

void NewLocalGame::EventOccurred(Event *e)
{
	newGameForm->EventOccured( e );

	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		switch( e->Data.Keyboard.KeyCode )
		{
			case ALLEGRO_KEY_UP:
				selectedItem = (selectedItem + 3) % 4;
				break;
			case ALLEGRO_KEY_DOWN:
				selectedItem = (++selectedItem) % 4;
				break;
			case ALLEGRO_KEY_ENTER:
			case ALLEGRO_KEY_PGDN:
				switch( selectedItem )
				{
					case 0:
						break;
					case 1:
						break;
					case 2:
						break;
					case 3:
						FRAMEWORK->ShutdownFramework();
						return;
				}
				break;
		}
	}

	if( e->Type == EVENT_MOUSE_DOWN )
	{
	}

	if( e->Type == EVENT_FORM_INTERACTION )
	{
		if( e->Data.Forms.RaisedBy->Name == "BUTTON_ADD" && e->Data.Forms.EventFlag == FormEventType::ButtonClick )
		{
			Label* l = new Label( newGameForm->FindControl("LIST_PLAYERS"), ((TextEdit*)newGameForm->FindControl("EDIT_NAME"))->GetText(), menuFont );
			l->Size.Y = menuFont->GetFontHeight() + 2;
			l->BackgroundColour = al_map_rgb( (rand() % 128) + 128, (rand() % 128) + 128, (rand() % 128) + 128 );
		}
		if( e->Data.Forms.RaisedBy->Name == "BUTTON_BACK" && e->Data.Forms.EventFlag == FormEventType::ButtonClick )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		if( e->Data.Forms.RaisedBy->Name == "SCROLL_PLANETS" && e->Data.Forms.EventFlag == FormEventType::ScrollBarChange )
		{
			((Label*)newGameForm->FindControl("LABEL_PLANETS"))->SetText( Strings::FromNumber( ((HScrollBar*)e->Data.Forms.RaisedBy)->Value ) );
		}
	}
}

void NewLocalGame::Update()
{
	newGameForm->Update();
}

void NewLocalGame::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
	titleFont->DrawString( DISPLAY->GetWidth() / 2, 6, "Konquestor", FontHAlign::CENTRE, al_map_rgb( 255, 255, 255 ) );

	newGameForm->Render();
}

bool NewLocalGame::IsTransition()
{
	return false;
}
