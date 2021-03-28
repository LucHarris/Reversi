#include "DeltaTimer.h"


void DeltaTimer::Restart(float t)
{
	time = t;
}


void DeltaTimer::Update(float dt)
{
	time -= dt;
}

bool DeltaTimer::HasElapsed()
{
	return time <= 0.0f;
}

