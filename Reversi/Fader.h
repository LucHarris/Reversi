#pragma once
class Fader
{

	float mTimer = 0.0f;
	bool mFading = false;
	bool mEndState = false;
	float mRate = 1.0f;
public:
	void Reset(float r = 1.0f)
	{
		mTimer = 0.0f;
		mFading = true;
		mRate = r;
	}

	void Update(float dt)
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

	// event to set
	bool EndState() const
	{
		return mEndState;
	}

	bool isFading() const
	{
		return mFading;
	}

	float GetFade() const
	{
		return mTimer;
	}

	float GetInverseFade() const
	{
		return 1.0f - mTimer;
	}
};

