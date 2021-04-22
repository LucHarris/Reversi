#include "MusicEngine.h"
#include <filesystem>
#include <cassert>

void MusicEngine::Init()
{
	const std::string dir("./Data/Audio/Music/");
	const std::string ext(".ogg");

	// searches music dir 
	for (auto& p : std::filesystem::directory_iterator(dir))
	{
		// opens and adds wav format stream to container with default values
		if (!ext.compare(p.path().extension().generic_string()))
		{
			std::unique_ptr<sf::Music> m(new sf::Music);

			std::string s = p.path().generic_string();
			if (m->openFromFile(s))
			{
				m->setLoop(true);
				m->setVolume(0.0f);
				mMusic.push_back(std::move(m));
			}
			else
			{
				// hasn't loaded
				assert(false);
			}
		}
	}

	const size_t size = mMusic.size();

	assert(size > 1);

	srand(time(NULL));
	mActiveMusic = 1 + (rand() % (mMusic.size() - 1)); // 1 to (mMusic.size() - 1)

	assert(mActiveMusic > 0);

	mPreviousMusic = mActiveMusic - 1;
}


void MusicEngine::Update(float dt)
{
	mFader.Update(dt);

	if (mFader.isFading())
	{
		SetVolume();
	} 
	else if (mFader.EndState())
	{

		for (auto& m : mMusic)
		{
			m->pause();
		}

		mMusic.at(mActiveMusic)->play();
	}
}

void MusicEngine::PlayNext(float fade)
{

	size_t s = mMusic.size();
	if (++mActiveMusic >= s)
	{
		mActiveMusic = 0;
	}

	if (++mPreviousMusic >= s)
	{
		mPreviousMusic = 0;
	}

	mFader.Reset(fade);
	
	mMusic.at(mActiveMusic)->play();

}

void MusicEngine::SetMasterVolume(float vol)
{
	mMasterVolume = vol;
	// update volume
	SetVolume();
}

void MusicEngine::SetVolume()
{
	mMusic.at(mActiveMusic)->setVolume(mFader.GetFade() * mMasterVolume);
	mMusic.at(mPreviousMusic)->setVolume(mFader.GetInverseFade() * mMasterVolume);
}

MusicEngine::MusicEngine()
{
}


