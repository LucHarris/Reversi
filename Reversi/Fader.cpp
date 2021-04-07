#include "Fader.h"
#include <algorithm>
#include "Constants.h"

void Fader::Reset(float r)
{
	mTimer = 0.0f;
	mFading = true;
	mRate = r;
}

void Fader::Update(float dt)
{
	if (mFading)
	{
		mTimer += dt / mRate;
		if (mTimer >= 1.0f)
		{
			mFading = false;
			mEndState = true; // next frame will be reset
		}
	}
	else
	{
		mEndState = false;
	}
}

float Fader::GetFade() const
{
	return std::clamp(mTimer, gc::VOL_FADE_MIN, gc::VOL_FADE_MAX);
}


float Fader::GetInverseFade() const
{
	return std::clamp(1.0f - mTimer, gc::VOL_FADE_MIN, gc::VOL_FADE_MAX);
}