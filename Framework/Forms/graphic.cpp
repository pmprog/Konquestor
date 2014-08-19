#include "graphic.h"
#include "../Display/bitmapcache.h"

Graphic::Graphic( Control* Owner, std::string Image ) : Control( Owner )
{
	image_name = Image;
	image = nullptr;
}

Graphic::~Graphic()
{
}

void Graphic::EventOccured( Event* e )
{
	Control::EventOccured( e );
}

void Graphic::OnRender()
{
	if( image == nullptr && image_name.length() > 0 )
	{
		image = BitmapCache::LoadBitmap( image_name );
		if( Size.X == 0 )
		{
			Size.X = al_get_bitmap_width( image );
		}
		if( Size.Y == 0 )
		{
			Size.Y = al_get_bitmap_height( image );
		}
	}

	if( image != nullptr )
	{
		int bmpw = al_get_bitmap_width( image );
		int bmph = al_get_bitmap_height( image );
		if( bmpw == Size.X && bmph == Size.Y )
		{
			al_draw_bitmap( image, 0, 0, 0 );
		} else {
			al_draw_scaled_bitmap( image, 0, 0, bmpw, bmph, 0, 0, this->Size.X, this->Size.Y, 0 );
		}
	}
}

void Graphic::Update()
{
	Control::Update();
}

void Graphic::UnloadResources()
{
	if( image != nullptr )
	{
		BitmapCache::UnloadBitmap( image );
		image = nullptr;
	}
	Control::UnloadResources();
}

ALLEGRO_BITMAP* Graphic::GetImage()
{
	return image;
}

void Graphic::SetImage( ALLEGRO_BITMAP* Image )
{
	image = Image;
	image_name = "";
}

