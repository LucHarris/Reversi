#pragma once
class Fader
{

	float mTimer = 0.0f;
	bool mFading = false;
	bool mEndState = false;
	float mRate = 1.0f;

public:
	void Reset(float r = 1.0f);


	void Update(float dt);

	// event to set
	bool EndState() const
	{
		return mEndState;
	}

	bool isFading() const
	{
		return mFading;
	}

	float GetFade() const;
	

	float GetInverseFade() const;
};

