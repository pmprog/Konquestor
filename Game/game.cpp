
#include "game.h"
#include "../Framework/Primitives/strings.h"

Game::Game( int Planets, std::vector<Player*> Players )
{
	menuFont = FontCache::LoadFont( "resources/game.ttf", 48 );
	detailFont = FontCache::LoadFont( "resources/game.ttf", 36 );
	selectSin = new Angle(0);

	gridSourceX = -1;
	gridSourceY = -1;
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
		galacticMap[ (py * MAP_WIDTH) + px ]->GenerateName( pi );
		galacticMap[ (py * MAP_WIDTH) + px ]->GridX = px;
		galacticMap[ (py * MAP_WIDTH) + px ]->GridY = py;
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
		planetList.at( pi )->ProductionRate = 5;
		planetList.at( pi )->Ships = 13;

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

	localInfoButton = new GraphicButton( localInputForm, "resources/button_info.png", "resources/button_info_sel.png", "resources/button_info_hover.png" );
	localInfoButton->Location.X = 0;
	localInfoButton->Location.Y = 0;
	localTransfersButton = new GraphicButton( localInputForm, "resources/button_xfer.png", "resources/button_xfer_sel.png", "resources/button_xfer_hover.png" );
	localTransfersButton->Location.X = localInfoButton->Size.X;
	localTransfersButton->Location.Y = 0;

	localPlanetEndTurn = new TextButton( localInputForm, "End Turn", menuFont );
	localPlanetEndTurn->Name = "BTN_ENDTURN";
	localPlanetEndTurn->BackgroundColour.a = 0.0f;
	localPlanetEndTurn->Size.X = localInputForm->Size.X - 4;
	localPlanetEndTurn->Size.Y = menuFont->GetFontHeight() + 4;
	localPlanetEndTurn->Location.X = 2;
	localPlanetEndTurn->Location.Y = localInputForm->Size.Y - localPlanetEndTurn->Size.Y;

	localInfoTab = new Control( localInputForm );
	localInfoTab->BackgroundColour = al_map_rgba( 0, 0, 0, 128 );
	localInfoTab->Location.X = 2;
	localInfoTab->Location.Y = localTransfersButton->Size.Y + 2;
	localInfoTab->Size.X = localInputForm->Size.X - 4;
	localInfoTab->Size.Y = localPlanetEndTurn->Location.Y - 2 - localInfoTab->Location.Y;

	localTransferTab = new Control( localInputForm );
	localTransferTab->BackgroundColour = al_map_rgba( 0, 0, 0, 128 );
	localTransferTab->Location.X = 2000;
	localTransferTab->Location.Y = localTransfersButton->Size.Y + 2;
	localTransferTab->Size.X = localInputForm->Size.X - 4;
	localTransferTab->Size.Y = localPlanetEndTurn->Location.Y - 2 - localTransferTab->Location.Y;


	////////////////////////////////// localInfoTab

	localPlanetInfoSource = new PlanetControl( localInfoTab , detailFont, false );
	localPlanetInfoSource->BackgroundColour.a = 0.0f;
	localPlanetInfoSource->Location.X = 2;
	localPlanetInfoSource->Location.Y = 2;
	localPlanetInfoSource->Size.X = localInfoTab->Size.X - 4;

	localPlanetLaunch = new TextButton( localInfoTab, "Launch", menuFont );
	localPlanetLaunch->Location.X = 2000;
	localPlanetLaunch->Location.Y = localPlanetInfoSource->Size.Y + 4;
	localPlanetLaunch->Size.X = localInfoTab->Size.X - 4;
	localPlanetLaunch->Size.Y = menuFont->GetFontHeight() + 6;

	localPlanetInfoTarget = new PlanetControl( localInfoTab , detailFont, true );
	localPlanetInfoTarget->BackgroundColour.a = 0.0f;
	localPlanetInfoTarget->Location.X = 2000;
	localPlanetInfoTarget->Location.Y = localPlanetInfoSource->Size.Y + 4;
	localPlanetInfoTarget->Size.X = localInfoTab->Size.X - 4;

	int preexpansiony = localPlanetInfoTarget->Size.Y;
	localPlanetInfoTarget->Size.Y = localInfoTab->Size.Y - 2 - localPlanetInfoTarget->Location.Y;


	Label* l = new Label( localPlanetInfoTarget, "Ships to Send", detailFont );
	l->BackgroundColour.a = 0.0f;
	l->Location.X = 2;
	l->Location.Y = preexpansiony;
	l->Size.X = localPlanetInfoTarget->Size.X / 1.5f;
	l->Size.Y = detailFont->GetFontHeight() + 4;
	l->TextVAlign = VerticalAlignment::Centre;

	localPlanetTargetSliderValue = new Label( localPlanetInfoTarget, "0", detailFont );
	localPlanetTargetSliderValue->BackgroundColour.a = 0.0f;
	localPlanetTargetSliderValue->ForegroundColour = al_map_rgb( 255, 255, 0 );
	localPlanetTargetSliderValue->Location.X = localPlanetInfoTarget->Size.X / 1.5f;
	localPlanetTargetSliderValue->Location.Y = preexpansiony;
	localPlanetTargetSliderValue->Size.X = localPlanetInfoTarget->Size.X - localPlanetTargetSliderValue->Location.X - 2;
	localPlanetTargetSliderValue->Size.Y = detailFont->GetFontHeight() + 4;
	localPlanetTargetSliderValue->TextVAlign = VerticalAlignment::Centre;
	localPlanetTargetSliderValue->TextHAlign = HorizontalAlignment::Centre;

	localPlanetTargetSlider = new HScrollBar( localPlanetInfoTarget );
	localPlanetTargetSlider->BackgroundColour = al_map_rgb(0, 0, 0);
	localPlanetTargetSlider->Minimum = 0;
	localPlanetTargetSlider->Maximum = 0;
	localPlanetTargetSlider->Value = 0;
	localPlanetTargetSlider->Location.X = 2;
	localPlanetTargetSlider->Location.Y = preexpansiony + detailFont->GetFontHeight() + 6;
	localPlanetTargetSlider->Size.X = localPlanetInfoTarget->Size.X - 4;
	localPlanetTargetSlider->Size.Y = 16;

	localPlanetCancelLaunch = new TextButton( localPlanetInfoTarget, "Cancel", detailFont );
	localPlanetCancelLaunch->Location.X = 2;
	localPlanetCancelLaunch->Location.Y = localPlanetInfoTarget->Size.Y - 6 - detailFont->GetFontHeight();
	localPlanetCancelLaunch->Size.X = (localPlanetInfoTarget->Size.X - 6) / 2;
	localPlanetCancelLaunch->Size.Y = detailFont->GetFontHeight() + 4;

	localPlanetSendShips = new TextButton( localPlanetInfoTarget, "Send", detailFont );
	localPlanetSendShips->Location.X = 4 + localPlanetCancelLaunch->Size.X;
	localPlanetSendShips->Location.Y = localPlanetInfoTarget->Size.Y - 6 - detailFont->GetFontHeight();
	localPlanetSendShips->Size.X = (localPlanetInfoTarget->Size.X - 6) / 2;
	localPlanetSendShips->Size.Y = detailFont->GetFontHeight() + 4;

	////////////////////////////////// localTransferTab

	localPlanetInFlightList = new ListBox( localTransferTab );
	localPlanetInFlightList->Location.X = 2;
	localPlanetInFlightList->Location.Y = 2;
	localPlanetInFlightList->Size.X = localTransferTab->Size.X - 4;
	localPlanetInFlightList->Size.Y = localTransferTab->Size.Y - 4;


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
			if( gridSourceX >= 0 && gridSourceY >= 0 )
			{
				gridSelectX = gridSourceX;
				gridSelectY = gridSourceY;
				gridSourceX = -1;
				gridSourceY = -1;
				DisplaySelectedPlanetInfo();
			} else {
				delete FRAMEWORK->ProgramStages->Pop();
				return;
			}
		}

		switch( e->Data.Keyboard.KeyCode )
		{
			case ALLEGRO_KEY_LEFT:
				if( gridSelectX > 0 )
				{
					gridSelectX--;
					DisplaySelectedPlanetInfo();
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				if( gridSelectX < MAP_WIDTH - 1 )
				{
					gridSelectX++;
					DisplaySelectedPlanetInfo();
				}
				break;
			case ALLEGRO_KEY_UP:
				if( gridSelectY > 0 )
				{
					gridSelectY--;
					DisplaySelectedPlanetInfo();
				}
				break;
			case ALLEGRO_KEY_DOWN:
				if( gridSelectY < MAP_HEIGHT - 1 )
				{
					gridSelectY++;
					DisplaySelectedPlanetInfo();
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
			DisplaySelectedPlanetInfo();
		}
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.EventFlag == FormEventType::ScrollBarChange )
	{
		if( e->Data.Forms.RaisedBy == localPlanetTargetSlider )
		{
			localPlanetTargetSliderValue->SetText(Strings::FromNumber(localPlanetTargetSlider->Value));
		}
	}

	if( e->Type == EVENT_FORM_INTERACTION && e->Data.Forms.EventFlag == FormEventType::ButtonClick )
	{
		if( e->Data.Forms.RaisedBy == localPlanetEndTurn )
		{
			NextPlayer();
		}
		if( e->Data.Forms.RaisedBy == localInfoButton )
		{
			localInfoTab->Location.X = 2;
			localTransferTab->Location.X = 2000;
		}
		if( e->Data.Forms.RaisedBy == localTransfersButton )
		{
			localInfoTab->Location.X = 2000;
			localTransferTab->Location.X = 2;
		}
		if( e->Data.Forms.RaisedBy == localPlanetLaunch )
		{
			gridSourceX = gridSelectX;
			gridSourceY = gridSelectY;
			DisplaySelectedPlanetInfo();
		}
		if( e->Data.Forms.RaisedBy == localPlanetCancelLaunch )
		{
			gridSelectX = gridSourceX;
			gridSelectY = gridSourceY;
			gridSourceX = -1;
			gridSourceY = -1;
			DisplaySelectedPlanetInfo();
		}

		if( e->Data.Forms.RaisedBy == localPlanetSendShips )
		{
			// TODO: Launch Ships
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

	if( gridSourceX >= 0 && gridSourceY >= 0 )
	{
		al_draw_rectangle( 10 + (gridSourceX * MAP_GRIDSIZE), 10 + (gridSourceY * MAP_GRIDSIZE), 10 + ((gridSourceX+1) * MAP_GRIDSIZE), 10 + ((gridSourceY+1) * MAP_GRIDSIZE), al_map_rgba_f( 0.0f, 0.4f, 1.0f, ((selectSin->Sine() + 1.0f) / 4.0f) + 0.5f ), 3 );
	}
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
	gridSourceX = -1;
	gridSourceY = -1;
	DisplaySelectedPlanetInfo();

	currentPlayer = (currentPlayer + 1) % playerList.size();

	if( turnChangesAtPlayer == currentPlayer )
	{
		TurnNumber++;
	}

	Player* p = playerList.at(currentPlayer);

	for( auto pi = planetList.begin(); pi != planetList.end(); pi++ )
	{
		Planet* pl = (Planet*)*pi;
		if( pl->OwnedBy == p )
		{
			pl->Ships += pl->ProductionRate;
		}
	}

	switch( p->Interaction )
	{
		case PlayerType::LocalHuman:
			activeInputForm = localInputForm;
			localPlanetInfoSource->SetPlayer( p );
			localPlanetInfoTarget->SetPlayer( p );
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
			for( int i = 1; i < 99000000; i++)
			{
			}
			// TODO: Process AI
			me->CurrentGame->NextPlayer();
		}
	}
	return nullptr;
}

void Game::DisplaySelectedPlanetInfo()
{
	Planet* p = galacticMap[ (gridSelectY * MAP_WIDTH) + gridSelectX ];

	if( gridSourceX < 0 )
	{
		localPlanetInfoSource->SetPlanet( p );
		if( p != nullptr )
		{
			localPlanetTargetSlider->Maximum = p->Ships;
			localPlanetTargetSlider->Value = p->Ships;
			localPlanetTargetSliderValue->SetText(Strings::FromNumber(p->Ships));
		} else {
			localPlanetTargetSlider->Maximum = 0;
			localPlanetTargetSlider->Value = 0;
			localPlanetTargetSliderValue->SetText("0");
		}
	} else {
		localPlanetInfoTarget->SetPlanet( p );
		localPlanetInfoTarget->SetTarget( galacticMap[ (gridSourceY * MAP_WIDTH) + gridSourceX ] );
	}

	if( p == nullptr )
	{
		localPlanetLaunch->Location.X = 2000;
		localPlanetInfoTarget->Location.X = 2000;
	} else if( gridSourceX >= 0 ) {
		localPlanetLaunch->Location.X = 2000;
		localPlanetInfoTarget->Location.X = 2;
	} else {
		localPlanetInfoTarget->Location.X = 2000;
		if( p->OwnedBy != GetCurrentPlayer() )
		{
			localPlanetLaunch->Location.X = 2000;
		} else {
			localPlanetLaunch->Location.X = 2;
		}
	}

}
