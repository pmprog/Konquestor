
#include "graphicbutton.h"

GraphicButton::GraphicButton( Control* Owner, std::string Image, std::string ImageDepressed ) : Control( Owner )
{
	CanFocus = true;
	image = nullptr;
	imagedepressed = nullptr;
	imagehover = nullptr;
	image_name = Image;
	imagedepressed_name = ImageDepressed;
	imagehover_name = "";
}

GraphicButton::GraphicButton( Control* Owner, std::string Image, std::string ImageDepressed, std::string ImageHover ) : Control( Owner )
{
	CanFocus = true;
	image = nullptr;
	imagedepressed = nullptr;
	imagehover = nullptr;
	image_name = Image;
	imagedepressed_name = ImageDepressed;
	imagehover_name = ImageHover;
}

GraphicButton::~GraphicButton()
{
}

void GraphicButton::EventOccured( Event* e )
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

void GraphicButton::OnRender()
{
	ALLEGRO_BITMAP* useimage;

	if( image == nullptr && image_name != "" )
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
	if( imagedepressed == nullptr && imagedepressed_name != "" )
	{
		imagedepressed = BitmapCache::LoadBitmap( imagedepressed_name );
		if( Size.X == 0 )
		{
			Size.X = al_get_bitmap_width( imagedepressed );
		}
		if( Size.Y == 0 )
		{
			Size.Y = al_get_bitmap_height( imagedepressed );
		}
	}
	if( imagehover == nullptr && imagehover_name != "" )
	{
		imagehover = BitmapCache::LoadBitmap( imagehover_name );
		if( Size.X == 0 )
		{
			Size.X = al_get_bitmap_width( imagehover );
		}
		if( Size.Y == 0 )
		{
			Size.Y = al_get_bitmap_height( imagehover );
		}
	}

	useimage = image;
	if( mouseDepressed )
	{
		useimage = imagedepressed;
	} else if( (mouseInside || IsFocused()) && imagehover != nullptr ) {
		useimage = imagehover;
	}

	if( useimage != nullptr )
	{
		int bmpw = al_get_bitmap_width( useimage );
		int bmph = al_get_bitmap_height( useimage );
		if( bmpw == Size.X && bmph == Size.Y )
		{
			al_draw_bitmap( useimage, 0, 0, 0 );
		} else {
			al_draw_scaled_bitmap( useimage, 0, 0, bmpw, bmph, 0, 0, this->Size.X, this->Size.Y, 0 );
		}
	}
}

void GraphicButton::Update()
{
	Control::Update();
}

void GraphicButton::UnloadResources()
{
	if( image != nullptr )
	{
		BitmapCache::UnloadBitmap( image );
		image = nullptr;
	}
	if( imagedepressed != nullptr )
	{
		BitmapCache::UnloadBitmap( imagedepressed );
		imagedepressed = nullptr;
	}
	if( imagehover != nullptr )
	{
		BitmapCache::UnloadBitmap( imagehover );
		imagehover = nullptr;
	}
	Control::UnloadResources();
}

ALLEGRO_BITMAP* GraphicButton::GetImage()
{
	return image;
}

void GraphicButton::SetImage( ALLEGRO_BITMAP* Image )
{
	image_name = "";
	image = Image;
}

ALLEGRO_BITMAP* GraphicButton::GetDepressedImage()
{
	return imagedepressed;
}

void GraphicButton::SetDepressedImage( ALLEGRO_BITMAP* Image )
{
	imagedepressed_name = "";
	imagedepressed = Image;
}

ALLEGRO_BITMAP* GraphicButton::GetHoverImage()
{
	return imagehover;
}

void GraphicButton::SetHoverImage( ALLEGRO_BITMAP* Image )
{
	imagehover_name = "";
	imagehover = Image;
}

