
#include "game.h"

Game::Game( int Planets, std::vector<Player*> Players )
{
	playerList = Players;
	// TODO: Sync on netgame
	currentPlayer = rand() % playerList.size();

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

	for( auto pli = playerList.begin(); pli != playerList.end(); pli++ )
	{
		Player* pl = (Player*)*pli;
		int pi = rand() % planetList.size();
		while( planetList.at( pi )->OwnedBy != nullptr )
		{
			pi = rand() % planetList.size();
		}
		planetList.at( pi )->OwnedBy = pl;
		planetList.at( pi )->DefenceStats = 0.7f;
		planetList.at( pi )->ProductionRate = 3;
		planetList.at( pi )->Ships = 10;
	}

}

Game::~Game()
{
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
}

void Game::EventOccurred(Event *e)
{
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
}

void Game::Render()
{
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	for( int y = 0; y < MAP_HEIGHT; y++ )
	{
		for( int x = 0; x < MAP_WIDTH; x++ )
		{
			Planet* p = galacticMap[ (y * MAP_WIDTH) + x ];
			if( p != nullptr )
			{
				if( p->OwnedBy != nullptr )
				{
					al_draw_filled_circle( 40 + (x * 60), 40 + (y * 60), 24, p->OwnedBy->Colour );
				}
				al_draw_filled_circle( 40 + (x * 60), 40 + (y * 60), 20, p->PlanetColour );
			}

			al_draw_line( 10 + (x * 60), 10 + (y * 60), 10 + ((x + 1) * 60), 10 + (y * 60), al_map_rgb( 255, 255, 255 ), 1 );
			al_draw_line( 10 + (x * 60), 10 + (y * 60), 10 + (x * 60), 10 + ((y + 1) * 60), al_map_rgb( 255, 255, 255 ), 1 );

		}
	}

	al_draw_line( 10, 10 + (MAP_HEIGHT * 60), 10 + (MAP_WIDTH * 60), 10 + (MAP_HEIGHT * 60), al_map_rgb( 255, 255, 255 ), 1 );
	al_draw_line( 10 + (MAP_WIDTH * 60), 10, 10 + (MAP_WIDTH * 60), 10 + (MAP_HEIGHT * 60), al_map_rgb( 255, 255, 255 ), 1 );


}

bool Game::IsTransition()
{
	return false;
}
