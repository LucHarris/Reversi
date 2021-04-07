#pragma once
#include <SFML/Audio.hpp>

#include <vector>
#include <array>
#include <memory>
#include "Fader.h"

// opens .wav files from a folder and fades between music track
class MusicEngine
{
	// music streams
	std::vector<std::unique_ptr<sf::Music>> mMusic;
	// values for fading between music tracks
	Fader mFader;

	// range 0 to 100.0f
	float mMasterVolume = 00.0f;
	// for fading between streams
	size_t mActiveMusic = 1;
	// always one behind active music track
	size_t mPreviousMusic = 0;
	// cross fade between tracks using mFader
	void SetVolume();
public:
	MusicEngine();
	// loads music from folder
	void Init();
	// adjust volume for fading
	void Update(float dt) ;
	// sets next track to fade into
	void PlayNext(float fade = 1.0f);

	void SetMasterVolume(float vol);
};

