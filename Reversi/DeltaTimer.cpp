#include "DeltaTimer.h"


void DeltaCountdownTimer::Restart(float t)
{
	time = t;
}


void DeltaCountdownTimer::Update(float dt)
{
	time -= dt;
}

bool DeltaCountdownTimer::HasElapsed()
{
	return time <= 0.0f;
}

