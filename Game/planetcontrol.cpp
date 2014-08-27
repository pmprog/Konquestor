
#include "planetcontrol.h"

PlanetControl::PlanetControl( Control* Owner, TTFFont* Font, bool ShowTurns ) : Control( Owner ), viewingPlanet( nullptr ), targetPlanet( nullptr ), viewingAs( nullptr ), lastSize(-1, -1)
{
	localPlanetTitle = new Label( this, "No Planet", Font );
	localPlanetTitle->BackgroundColour.a = 0.5f;
	localPlanetTitle->Location.X = 2;
	localPlanetTitle->Location.Y = 2;
	localPlanetTitle->Size.Y = Font->GetFontHeight() + 4;
	localPlanetTitle->TextHAlign = HorizontalAlignment::Centre;
	localPlanetTitle->TextVAlign = VerticalAlignment::Centre;

	int yPos = 4 + localPlanetTitle->Size.Y;

	localPlanetShipsLabel = new Label( this, "Ships:", Font );
	localPlanetShipsLabel->BackgroundColour.a = 0.0f;
	localPlanetShipsLabel->Location.X = 2;
	localPlanetShipsLabel->Location.Y = yPos;
	localPlanetShipsLabel->Size.Y = Font->GetFontHeight() + 4;
	localPlanetShipsLabel->TextVAlign = VerticalAlignment::Centre;

	localPlanetShips = new Label( this, "N/A", Font );
	localPlanetShips->BackgroundColour.a = 0.0f;
	localPlanetShips->ForegroundColour = al_map_rgb( 255, 255, 0 );
	localPlanetShips->Location.Y = yPos;
	localPlanetShips->Size.Y = Font->GetFontHeight() + 4;
	localPlanetShips->TextVAlign = VerticalAlignment::Centre;
	localPlanetShips->TextHAlign = HorizontalAlignment::Centre;

	yPos += localPlanetShipsLabel->Size.Y + 2;

	localPlanetProductionLabel = new Label( this, "Production:", Font );
	localPlanetProductionLabel->BackgroundColour.a = 0.0f;
	localPlanetProductionLabel->Location.X = 2;
	localPlanetProductionLabel->Location.Y = yPos;
	localPlanetProductionLabel->Size.Y = Font->GetFontHeight() + 4;
	localPlanetProductionLabel->TextVAlign = VerticalAlignment::Centre;

	localPlanetProduction = new Label( this, "N/A", Font );
	localPlanetProduction->BackgroundColour.a = 0.0f;
	localPlanetProduction->ForegroundColour = al_map_rgb( 255, 255, 0 );
	localPlanetProduction->Location.Y = yPos;
	localPlanetProduction->Size.Y = Font->GetFontHeight() + 4;
	localPlanetProduction->TextVAlign = VerticalAlignment::Centre;
	localPlanetProduction->TextHAlign = HorizontalAlignment::Centre;

	yPos += localPlanetProductionLabel->Size.Y + 2;

	localPlanetDefenceLabel = new Label( this, "Defence:", Font );
	localPlanetDefenceLabel->BackgroundColour.a = 0.0f;
	localPlanetDefenceLabel->Location.X = 2;
	localPlanetDefenceLabel->Location.Y = yPos;
	localPlanetDefenceLabel->Size.Y = Font->GetFontHeight() + 4;
	localPlanetDefenceLabel->TextVAlign = VerticalAlignment::Centre;

	localPlanetDefence = new Label( this, "N/A", Font );
	localPlanetDefence->BackgroundColour.a = 0.0f;
	localPlanetDefence->ForegroundColour = al_map_rgb( 255, 255, 0 );
	localPlanetDefence->Location.Y = yPos;
	localPlanetDefence->Size.Y = Font->GetFontHeight() + 4;
	localPlanetDefence->TextVAlign = VerticalAlignment::Centre;
	localPlanetDefence->TextHAlign = HorizontalAlignment::Centre;

	if( ShowTurns )
	{
		yPos += localPlanetDefence->Size.Y + 2;

		localPlanetTargetTurnsLabel = new Label( this, "Turns:", Font );
		localPlanetTargetTurnsLabel->BackgroundColour.a = 0.0f;
		localPlanetTargetTurnsLabel->Location.X = 2;
		localPlanetTargetTurnsLabel->Location.Y = yPos;
		localPlanetTargetTurnsLabel->Size.Y = Font->GetFontHeight() + 4;
		localPlanetTargetTurnsLabel->TextVAlign = VerticalAlignment::Centre;

		localPlanetTargetTurns = new Label( this, "N/A", Font );
		localPlanetTargetTurns->BackgroundColour.a = 0.0f;
		localPlanetTargetTurns->ForegroundColour = al_map_rgb( 255, 255, 0 );
		localPlanetTargetTurns->Location.Y = yPos;
		localPlanetTargetTurns->Size.Y = Font->GetFontHeight() + 4;
		localPlanetTargetTurns->TextVAlign = VerticalAlignment::Centre;
		localPlanetTargetTurns->TextHAlign = HorizontalAlignment::Centre;
	} else {
		localPlanetTargetTurnsLabel = nullptr;
		localPlanetTargetTurns = nullptr;
	}

	Size.Y = yPos + localPlanetDefenceLabel->Size.Y + 2;
	lastSize.Y = Size.Y;
}

PlanetControl::~PlanetControl()
{
	delete localPlanetTitle;
}

void PlanetControl::EventOccured( Event* e )
{
	Control::EventOccured( e );
}

void PlanetControl::OnRender()
{
}

void PlanetControl::Update()
{
	if( lastSize.X != Size.X || lastSize.Y != Size.Y )
	{
		localPlanetTitle->Size.X = Size.X - 4;
		localPlanetShipsLabel->Size.X = (Size.X / 1.5f);
		localPlanetShips->Location.X = (Size.X / 1.5f);
		localPlanetShips->Size.X = Size.X - localPlanetShips->Location.X - 2;
		localPlanetProductionLabel->Size.X = (Size.X / 1.5f);
		localPlanetProduction->Location.X = (Size.X / 1.5f);
		localPlanetProduction->Size.X = Size.X - localPlanetProduction->Location.X - 2;
		localPlanetDefenceLabel->Size.X = (Size.X / 1.5f);
		localPlanetDefence->Location.X = (Size.X / 1.5f);
		localPlanetDefence->Size.X = Size.X - localPlanetDefence->Location.X - 2;
		if( localPlanetTargetTurnsLabel != nullptr )
		{
			localPlanetTargetTurnsLabel->Size.X = (Size.X / 1.5f);
			localPlanetTargetTurns->Location.X = (Size.X / 1.5f);
			localPlanetTargetTurns->Size.X = Size.X - localPlanetDefence->Location.X - 2;
		}

		lastSize.X = Size.X;
		lastSize.Y = Size.Y;
	}

	Control::Update();
}

void PlanetControl::UnloadResources()
{
}

void PlanetControl::SetPlanet( Planet* View )
{
	viewingPlanet = View;
	UpdateInfo();
}

void PlanetControl::SetTarget( Planet* View )
{
	targetPlanet = View;
	UpdateInfo();
}

void PlanetControl::SetPlayer( Player* View )
{
	viewingAs = View;
	UpdateInfo();
}

void PlanetControl::UpdateInfo()
{
	Planet* p = viewingPlanet;
	if( p == nullptr )
	{
		localPlanetTitle->SetText("No Planet");
		localPlanetShips->SetText("N/A");
		localPlanetProduction->SetText("N/A");
		localPlanetDefence->SetText("N/A");

		if( localPlanetTargetTurns != nullptr )
		{
			localPlanetTargetTurns->SetText("N/A");
		}
	} else {
		localPlanetTitle->SetText(p->Name);
		if( p->OwnedBy != nullptr && p->OwnedBy == viewingAs )
		{
			localPlanetShips->SetText( Strings::FromNumber(p->Ships) );
			localPlanetProduction->SetText( Strings::FromNumber(p->ProductionRate) );
		} else {
			localPlanetShips->SetText("???");
			localPlanetProduction->SetText("???");
		}
		localPlanetDefence->SetText( Strings::FromNumber(p->DefenceStats) );

		if( localPlanetTargetTurns != nullptr && targetPlanet != nullptr )
		{
			Vector2 t( p->GridX, p->GridY );
			Vector2* td = new Vector2( targetPlanet->GridX, targetPlanet->GridY );
			localPlanetTargetTurns->SetText( Strings::FromNumber( Maths::Ceiling(t.DistanceTo( td )) ) );
			delete td;
		}
	}

}
