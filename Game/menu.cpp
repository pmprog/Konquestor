
#include "menu.h"
#include "newlocal.h"

Menu::Menu()
{
	titleFont = FontCache::LoadFont( "resources/game.ttf", 128 );
	menuFont = FontCache::LoadFont( "resources/game.ttf", 64 );
	DISPLAY->MouseVisible( true );
	selectedItem = 0;
}

Menu::~Menu()
{
	FontCache::UnloadFont( titleFont );
	FontCache::UnloadFont( menuFont );
}

void Menu::Begin()
{
	AUDIO->PlayMusic( "resources/title.ogg", true );
}

void Menu::Pause()
{
}

void Menu::Resume()
{
	AUDIO->PlayMusic( "resources/title.ogg", true );
}

void Menu::Finish()
{
	AUDIO->StopMusic();
}

void Menu::EventOccurred(Event *e)
{
	if( e->Type == EVENT_KEY_PRESS )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			FRAMEWORK->ShutdownFramework();
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
						FRAMEWORK->ProgramStages->Push( new NewLocalGame() );
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
}

void Menu::Update()
{
}

void Menu::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
	titleFont->DrawString( DISPLAY->GetWidth() / 2, 6, "Konquestor", FontHAlign::CENTRE, al_map_rgb( 255, 255, 255 ) );

	int yPos = DISPLAY->GetHeight() - (menuFont->GetFontHeight() * 4.9f);

	menuFont->DrawString( DISPLAY->GetWidth() / 2, yPos, "Local Game", FontHAlign::CENTRE, (selectedItem == 0 ? al_map_rgb( 255, 255, 255 ) : al_map_rgb( 80, 80, 80 )) );
	yPos += (menuFont->GetFontHeight() * 1.2f);
	//menuFont->DrawString( DISPLAY->GetWidth() / 2, yPos, "Network Game", FontHAlign::CENTRE, (selectedItem == 1 ? al_map_rgb( 255, 255, 255 ) : al_map_rgb( 80, 80, 80 )) );
	menuFont->DrawString( DISPLAY->GetWidth() / 2, yPos, "Network Game", FontHAlign::CENTRE, (selectedItem == 1 ? al_map_rgb( 80, 80, 80 ) : al_map_rgb( 48, 48, 48 )) );
	yPos += (menuFont->GetFontHeight() * 1.2f);
	menuFont->DrawString( DISPLAY->GetWidth() / 2, yPos, "Options", FontHAlign::CENTRE, (selectedItem == 2 ? al_map_rgb( 255, 255, 255 ) : al_map_rgb( 80, 80, 80 )) );
	yPos += (menuFont->GetFontHeight() * 1.2f);
	menuFont->DrawString( DISPLAY->GetWidth() / 2, yPos, "Exit", FontHAlign::CENTRE, (selectedItem == 3 ? al_map_rgb( 255, 255, 255 ) : al_map_rgb( 80, 80, 80 )) );
	yPos += (menuFont->GetFontHeight() * 1.2f);
}

bool Menu::IsTransition()
{
	return false;
}
