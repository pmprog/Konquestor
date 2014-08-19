
#pragma once

#include "../includes.h"

#define AUDIO	Audio::CurrentAudio

class Audio
{

	private:
		ALLEGRO_MIXER* audioMixer;
		ALLEGRO_VOICE* audioVoice;

	public:
		static Audio* CurrentAudio;

    Audio();
    ~Audio();

		ALLEGRO_EVENT_SOURCE* GetEventSource();

    void PlayAudio( std::string Filename, bool Loop );
    void StopAudio();

		ALLEGRO_MIXER* GetMixer();
};

