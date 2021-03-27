#pragma once
#include <SFML/Audio.hpp>

#include <vector>
#include <array>
#include <memory>
#include "Fader.h"

class MusicEngine
{

	std::vector<std::unique_ptr<sf::Music>> mMusic;
	Fader mFader;

	// in seconds
	float mMasterVolume = 100.0f;
	// for fading between streams
	size_t mActiveMusic = 1;
	size_t mPreviousMusic = 0;
public:
	// loads music from folder
	void Init();
	// adjust volume for fading
	void Update(float dt) ;
	// sets next track to fade into
	void PlayNext(float fade = 1.0f);
};

