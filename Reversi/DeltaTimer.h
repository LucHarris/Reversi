#pragma once

// Counts down using delta time

class DeltaTimer
{
	float time;
public:
	DeltaTimer() = default;
	// Set seconds to count down from
	void Restart(float t);
	// decrements delta time from timer
	void Update(float dt);
	// returns true when the timer has reached 0
	bool HasElapsed();

};


