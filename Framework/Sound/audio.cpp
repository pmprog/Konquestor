
#include "audio.h"

Audio* Audio::CurrentAudio = nullptr;


Audio::Audio()
{
#ifdef WRITE_LOG
  printf( "Framework: Initialise Audio\n" );
#endif

	if( !al_install_audio() )
	{
		printf( "Audio_Initialise: Failed to install audio\n" );
		return;
	}
	if( !al_init_acodec_addon() )
	{
		printf( "Audio_Initialise: Failed to install codecs\n" );
		return;
	}

	// Allow playing samples
	al_reserve_samples( 10 );

	audioVoice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	if( audioVoice == 0 )
	{
		printf( "Audio_Initialise: Failed to create voice\n" );
		return;
	}
	audioMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
	if( audioMixer == 0 )
	{
		printf( "Audio_Initialise: Failed to create mixer\n" );
		al_destroy_voice( audioVoice );
		audioVoice = 0;
		return;
	}
	if( !al_attach_mixer_to_voice( audioMixer, audioVoice ) )
	{
		printf( "Audio_Initialise: Failed to attach mixer to voice\n" );
		al_destroy_voice( audioVoice );
		audioVoice = 0;
		al_destroy_mixer( audioMixer );
		audioMixer = 0;
		return;
	}
}

Audio::~Audio()
{
#ifdef WRITE_LOG
  printf( "Framework: Shutdown Audio\n" );
#endif
	if( audioVoice != 0 )
	{
		al_destroy_voice( audioVoice );
		audioVoice = 0;
	}
	if( audioMixer != 0 )
	{
		al_destroy_mixer( audioMixer );
		audioMixer = 0;
	}
	al_uninstall_audio();
}

ALLEGRO_EVENT_SOURCE* Audio::GetEventSource()
{
	return al_get_audio_event_source();
}

void Audio::PlayAudio( std::string Filename, bool Loop )
{
	if( audioVoice == 0 || audioMixer == 0 )
	{
		return;
	}

#ifdef WRITE_LOG
	printf( "Framework: Start audio file %s\n", Filename.c_str() );
#endif

}

void Audio::StopAudio()
{
	if( audioVoice == 0 || audioMixer == 0 )
	{
		return;
	}

#ifdef WRITE_LOG
  printf( "Framework: Stop audio\n" );
#endif
}

ALLEGRO_MIXER* Audio::GetMixer()
{
	return audioMixer;
}