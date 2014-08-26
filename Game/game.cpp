
#include "game.h"
#include "../Framework/Primitives/strings.h"

Game::Game( int Planets, std::vector<Player*> Players )
{
	menuFont = FontCache::LoadFont( "resources/game.ttf", 48 );
	detailFont = FontCache::LoadFont( "resources/game.ttf", 24 );
	selectSin = new Angle(0);
	gridSelectX = 0;
	gridSelectY = 0;

	StopAI = false;

	playerList = Players;

	for( int y = 0; y < MAP_HEIGHT; y++ )
	{
		for( int x = 0; x < MAP_WIDTH; x++ )
		{
			galacticMap[ (y * MAP_WIDTH) + x ] = nullptr;
		}
	}

	for( int pi = 0; pi < Planets; pi++ )
	{
		int px = rand() % MAP_WIDTH;
		int py = rand() % MAP_HEIGHT;
		while( galacticMap[ (py * MAP_WIDTH) + px ] != nullptr )
		{
			px = rand() % MAP_WIDTH;
			py = rand() % MAP_HEIGHT;
		}
		galacticMap[ (py * MAP_WIDTH) + px ] = new Planet();
		planetList.push_back(galacticMap[ (py * MAP_WIDTH) + px ]);
	}

	bool hasremoteplayers = false;
	for( auto pli = playerList.begin(); pli != playerList.end(); pli++ )
	{
		Player* pl = (Player*)*pli;
		pl->CurrentGame = this;
		int pi = rand() % planetList.size();
		while( planetList.at( pi )->OwnedBy != nullptr )
		{
			pi = rand() % planetList.size();
		}
		planetList.at( pi )->OwnedBy = pl;
		planetList.at( pi )->DefenceStats = 0.7f;
		planetList.at( pi )->ProductionRate = 3;
		planetList.at( pi )->Ships = 10;

		if( pl->Interaction == PlayerType::LocalComputer )
		{
			ALLEGRO_THREAD* ait = al_create_thread( &this->AIThread, (void*)pl );
			AIThreads.push_back( ait );
		}

		if( pl->Interaction == PlayerType::RemoteComputer || pl->Interaction == PlayerType::RemoteHuman )
		{
			hasremoteplayers = true;
		}
	}

	localInputForm = new Form();
	localInputForm->Location.X = 20 + (MAP_GRIDSIZE * MAP_WIDTH);
	localInputForm->Size.X = DISPLAY->GetWidth() - 10 - localInputForm->Location.X;
	localInputForm->Location.Y = 10;
	localInputForm->Size.Y = DISPLAY->GetHeight() - 20;

	Label* l = new Label( localInputForm, "No Planet", menuFont );
	l->BackgroundColour.a = 0.5f;
	l->Location.X = 2;
	l->Location.Y = 2;
	l->Size.X = localInputForm->Size.X - 4;
	l->Size.Y = menuFont->GetFontHeight() + 4;
	l->TextHAlign = HorizontalAlignment::Centre;
	l->TextVAlign = VerticalAlignment::Centre;
	localPlanetTitle = l;

	Control* c = new Control( localInputForm );
	c->BackgroundColour.a = 0.5f;
	c->Location.X = 2;
	c->Location.Y = l->Location.Y + l->Size.Y + 2;
	c->Size.X = localInputForm->Size.X - 4;
	c->Size.Y = ((menuFont->GetFontHeight() + 4) * 4) + 12;

	int yPos = 2;

	l = new Label( c, "Ships:", menuFont );
	l->BackgroundColour.a = 0.0f;
	l->Location.X = 2;
	l->Location.Y = yPos;
	l->Size.X = (c->Size.X / 1.5f);
	l->Size.Y = menuFont->GetFontHeight() + 4;
	l->TextVAlign = VerticalAlignment::Centre;

	localPlanetShips = new Label( c, "N/A", menuFont );
	localPlanetShips->BackgroundColour.a = 0.0f;
	localPlanetShips->ForegroundColour = al_map_rgb( 255, 255, 0 );
	localPlanetShips->Location.X = (c->Size.X / 1.5f);
	localPlanetShips->Location.Y = yPos;
	localPlanetShips->Size.X = c->Size.X - localPlanetShips->Location.X - 2;
	localPlanetShips->Size.Y = menuFont->GetFontHeight() + 4;
	localPlanetShips->TextVAlign = VerticalAlignment::Centre;
	localPlanetShips->TextHAlign = HorizontalAlignment::Centre;

	yPos += l->Size.Y + 2;

	l = new Label( c, "Production:", menuFont );
	l->BackgroundColour.a = 0.0f;
	l->Location.X = 2;
	l->Location.Y = yPos;
	l->Size.X = (c->Size.X / 1.5f);
	l->Size.Y = menuFont->GetFontHeight() + 4;
	l->TextVAlign = VerticalAlignment::Centre;

	localPlanetProduction = new Label( c, "N/A", menuFont );
	localPlanetProduction->BackgroundColour.a = 0.0f;
	localPlanetProduction->ForegroundColour = al_map_rgb( 255, 255, 0 );
	localPlanetProduction->Location.X = (c->Size.X / 1.5f);
	localPlanetProduction->Location.Y = yPos;
	localPlanetProduction->Size.X = c->Size.X - localPlanetProduction->Location.X - 2;
	localPlanetProduction->Size.Y = menuFont->GetFontHeight() + 4;
	localPlanetProduction->TextVAlign = VerticalAlignment::Centre;
	localPlanetProduction->TextHAlign = HorizontalAlignment::Centre;

	yPos += l->Size.Y + 2;

	l = new Label( c, "Defence:", menuFont );
	l->BackgroundColour.a = 0.0f;
	l->Location.X = 2;
	l->Location.Y = yPos;
	l->Size.X = (c->Size.X / 1.5f);
	l->Size.Y = menuFont->GetFontHeight() + 4;
	l->TextVAlign = VerticalAlignment::Centre;

	localPlanetDefence = new Label( c, "N/A", menuFont );
	localPlanetDefence->BackgroundColour.a = 0.0f;
	localPlanetDefence->ForegroundColour = al_map_rgb( 255, 255, 0 );
	localPlanetDefence->Location.X = (c->Size.X / 1.5f);
	localPlanetDefence->Location.Y = yPos;
	localPlanetDefence->Size.X = c->Size.X - localPlanetDefence->Location.X - 2;
	localPlanetDefence->Size.Y = menuFont->GetFontHeight() + 4;
	localPlanetDefence->TextVAlign = VerticalAlignment::Centre;
	localPlanetDefence->TextHAlign = HorizontalAlignment::Centre;

	yPos += l->Size.Y + 2;

	localPlanetLaunch = new TextButton( c, "Launch", menuFont );
	localPlanetLaunch->Name = "BTN_LAUNCH";
	localPlanetLaunch->BackgroundColour.a = 0.0f;
	localPlanetLaunch->Location.X = 2;
	localPlanetLaunch->Location.Y = yPos;
	localPlanetLaunch->Size.X = c->Size.X - 4;
	localPlanetLaunch->Size.Y = menuFont->GetFontHeight() + 4;

	localPlanetEndTurn = new TextButton( localInputForm, "End Turn", menuFont );
	localPlanetEndTurn->Name = "BTN_ENDTURN";
	localPlanetEndTurn->BackgroundColour.a = 0.0f;
	localPlanetEndTurn->Size.X = localInputForm->Size.X - 4;
	localPlanetEndTurn->Size.Y = menuFont->GetFontHeight() + 4;
	localPlanetEndTurn->Location.X = 2;
	localPlanetEndTurn->Location.Y = localInputForm->Size.Y - localPlanetEndTurn->Size.Y;

	localPlanetInFlightList = new ListBox( localInputForm );
	localPlanetInFlightList->Location.X = 2;
	localPlanetInFlightList->Location.Y = c->Location.Y + c->Size.Y + 2;
	localPlanetInFlightList->Size.X = c->Size.X;
	localPlanetInFlightList->Size.Y = localPlanetEndTurn->Location.Y - localPlanetInFlightList->Location.Y - 4;


	waitInputForm = new Form();
	waitInputForm->Location.X = localInputForm->Location.X;
	waitInputForm->Location.Y = localInputForm->Location.Y;
	waitInputForm->Size.X = localInputForm->Size.X;
	waitInputForm->Size.Y = localInputForm->Size.Y;

	waitInputLabel = new Label( waitInputForm, "Please Wait", menuFont );
	waitInputLabel->Location.X = 2;
	waitInputLabel->Location.Y = 2;
	waitInputLabel->Size.X = waitInputForm->Size.X - 4;
	waitInputLabel->Size.Y = waitInputForm->Size.Y - 4;
	waitInputLabel->TextHAlign = HorizontalAlignment::Centre;
	waitInputLabel->TextVAlign = VerticalAlignment::Centre;

	TurnNumber = 1;
	if( hasremoteplayers )
	{
		activeInputForm = waitInputForm;

		// TODO: Sync on netgame
	} else {
		currentPlayer = rand() % playerList.size();
		turnChangesAtPlayer = currentPlayer;
		NextPlayer();
	}

	for( auto ti = AIThreads.begin(); ti != AIThreads.end(); ti++ )
	{
		ALLEGRO_THREAD* ait = (ALLEGRO_THREAD*)*ti;
		al_start_thread( ait );
	}
}

Game::~Game()
{

	while( AIThreads.size() > 0 )
	{
		ALLEGRO_THREAD* t = AIThreads.back();
		AIThreads.pop_back();
		al_destroy_thread( t );
	}

	FontCache::UnloadFont( menuFont );
	FontCache::UnloadFont( detailFont );
	delete selectSin;
}

void Game::Begin()
{
	AUDIO->StopMusic();
	AUDIO->PlayMusic( "resources/ingame.ogg", true );
}

void Game::Pause()
{
}

void Game::Resume()
{
}

void Game::Finish()
{
	AUDIO->StopMusic();
	StopAI = true;
}

void Game::EventOccurred(Event *e)
{
	activeInputForm->EventOccured( e );

	if( e->Type == EVENT_KEY_PRESS )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}

		switch( e->Data.Keyboard.KeyCode )
		{
			case ALLEGRO_KEY_LEFT:
				if( gridSelectX > 0 )
				{
					gridSelectX--;
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				if( gridSelectX < MAP_WIDTH - 1 )
				{
					gridSelectX++;
				}
				break;
			case ALLEGRO_KEY_UP:
				if( gridSelectY > 0 )
				{
					gridSelectY--;
				}
				break;
			case ALLEGRO_KEY_DOWN:
				if( gridSelectY < MAP_HEIGHT - 1 )
				{
					gridSelectY++;
				}
				break;
		}
	}

	if( e->Type == EVENT_MOUSE_DOWN )
	{
		if( e->Data.Mouse.X >= 10 && e->Data.Mouse.X < 10 + (MAP_WIDTH * MAP_GRIDSIZE) && e->Data.Mouse.Y >= 10 && e->Data.Mouse.Y < 10 + (MAP_HEIGHT * MAP_GRIDSIZE) )
		{
			gridSelectX = ( e->Data.Mouse.X - 10 ) / MAP_GRIDSIZE;
			gridSelectY = ( e->Data.Mouse.Y - 10 ) / MAP_GRIDSIZE;
		}
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.EventFlag == FormEventType::ButtonClick )
	{
		if( e->Data.Forms.RaisedBy == localPlanetEndTurn )
		{
			NextPlayer();
		}
	}

}

void Game::Update()
{
	selectSin->Add( 360.0 / (float)FRAMEWORK->GetFramesPerSecond() );
	activeInputForm->Update();
}

void Game::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	int pidx = 0;
	for( int y = 0; y < MAP_HEIGHT; y++ )
	{
		for( int x = 0; x < MAP_WIDTH; x++ )
		{
			Planet* p = galacticMap[ (y * MAP_WIDTH) + x ];
			if( p != nullptr )
			{
				if( p->OwnedBy != nullptr )
				{
					al_draw_filled_circle( 40 + (x * MAP_GRIDSIZE), 40 + (y * MAP_GRIDSIZE), 22 + (int)(selectSin->Sine() * 3.0f), p->OwnedBy->Colour );
				}

				al_draw_filled_circle( 40 + (x * MAP_GRIDSIZE), 40 + (y * MAP_GRIDSIZE), 20, p->PlanetColour );
				al_draw_filled_circle( 40 + (x * MAP_GRIDSIZE), 40 + (y * MAP_GRIDSIZE), 20, al_map_rgba( 0, 0, 0, 128 ) );
				al_draw_filled_circle( 38 + (x * MAP_GRIDSIZE), 38 + (y * MAP_GRIDSIZE), 16, p->PlanetColour );

				menuFont->DrawString( 16 + (x * MAP_GRIDSIZE), 10 + (y * MAP_GRIDSIZE), Strings::FromNumber( pidx ), FontHAlign::LEFT, al_map_rgb( 0, 0, 0 ) );
				menuFont->DrawString( 14 + (x * MAP_GRIDSIZE),  8 + (y * MAP_GRIDSIZE), Strings::FromNumber( pidx ), FontHAlign::LEFT, al_map_rgb( 255, 255, 255 ) );
				pidx++;
			}

			al_draw_line( 10 + (x * MAP_GRIDSIZE), 10 + (y * MAP_GRIDSIZE), 10 + ((x + 1) * MAP_GRIDSIZE), 10 + (y * MAP_GRIDSIZE), al_map_rgb( 255, 255, 255 ), 1 );
			al_draw_line( 10 + (x * MAP_GRIDSIZE), 10 + (y * MAP_GRIDSIZE), 10 + (x * MAP_GRIDSIZE), 10 + ((y + 1) * MAP_GRIDSIZE), al_map_rgb( 255, 255, 255 ), 1 );

		}
	}

	al_draw_line( 10, 10 + (MAP_HEIGHT * MAP_GRIDSIZE), 10 + (MAP_WIDTH * MAP_GRIDSIZE), 10 + (MAP_HEIGHT * MAP_GRIDSIZE), al_map_rgb( 255, 255, 255 ), 1 );
	al_draw_line( 10 + (MAP_WIDTH * MAP_GRIDSIZE), 10, 10 + (MAP_WIDTH * MAP_GRIDSIZE), 10 + (MAP_HEIGHT * MAP_GRIDSIZE), al_map_rgb( 255, 255, 255 ), 1 );

	al_draw_rectangle( 10 + (gridSelectX * MAP_GRIDSIZE), 10 + (gridSelectY * MAP_GRIDSIZE), 10 + ((gridSelectX+1) * MAP_GRIDSIZE), 10 + ((gridSelectY+1) * MAP_GRIDSIZE), al_map_rgba_f( 1.0f, 1.0f, 0.0f, ((selectSin->Cosine() + 1.0f) / 4.0f) + 0.5f ), 3 );

	std::string curplayer = playerList.at(currentPlayer)->Name;
	curplayer.append( "'s Turn");
	menuFont->DrawString( 10, DISPLAY->GetHeight() - menuFont->GetFontHeight() - 6, curplayer, FontHAlign::LEFT, al_map_rgb( 255, 255, 255 ) );

	activeInputForm->Render();
}

bool Game::IsTransition()
{
	return false;
}

void Game::NextPlayer()
{
	currentPlayer = (currentPlayer + 1) % playerList.size();

	if( turnChangesAtPlayer == currentPlayer )
	{
		TurnNumber++;
	}

	Player* p = playerList.at(currentPlayer);
	switch( p->Interaction )
	{
		case PlayerType::LocalHuman:
			activeInputForm = localInputForm;
			break;
		case PlayerType::LocalComputer:
			waitInputLabel->SetText("Waiting for AI");
			activeInputForm = waitInputForm;
			break;
		case PlayerType::RemoteHuman:
		case PlayerType::RemoteComputer:
			waitInputLabel->SetText("Waiting for Network");
			activeInputForm = waitInputForm;
			break;
	}
	activeInputForm->BackgroundColour = p->Colour;
}

Player* Game::GetCurrentPlayer()
{
	return playerList.at(currentPlayer);
}

void* Game::AIThread(ALLEGRO_THREAD* Thread, void* PlayerPtr)
{
	Player* me = (Player*)PlayerPtr;

	while( !me->CurrentGame->StopAI )
	{
		if( me->CurrentGame->GetCurrentPlayer() == me )
		{
			for( int i = 1; i < 9900000; i++)
			{
			}
			// TODO: Process AI
			me->CurrentGame->NextPlayer();
		}
	}
	return nullptr;
}
