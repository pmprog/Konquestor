
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

	Label* l = new Label( localInputForm, "Test", detailFont );
	l->Location.X = 0;
	l->Location.Y = 0;
	l->Size.X = 200;
	l->Size.Y = 40;
	
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

	if( hasremoteplayers )
	{
		activeInputForm = waitInputForm;

		// TODO: Sync on netgame
	} else {
		currentPlayer = rand() % playerList.size();
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
}

void Game::Pause()
{
}

void Game::Resume()
{
}

void Game::Finish()
{
	StopAI = true;
}

void Game::EventOccurred(Event *e)
{
	activeInputForm->EventOccured( e );

	if( e->Type == EVENT_KEY_DOWN )
	{
		if( e->Data.Keyboard.KeyCode == ALLEGRO_KEY_ESCAPE )
		{
			delete FRAMEWORK->ProgramStages->Pop();
			return;
		}
	}

}

void Game::Update()
{
	selectSin->Add( 360.0 / (float)FRAMEWORK->GetFramesPerSecond() );
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

	al_draw_rectangle( 10 + (gridSelectX * MAP_GRIDSIZE), 10 + (gridSelectY * MAP_GRIDSIZE), 10 + ((gridSelectX+1) * MAP_GRIDSIZE), 10 + ((gridSelectY+1) * MAP_GRIDSIZE), al_map_rgba_f( 1.0f, 1.0f, 0.0f, (selectSin->Cosine() + 1.0f) / 2.0f ), 3 );

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
			// TODO: Process AI
		}
	}
	return nullptr;
}
