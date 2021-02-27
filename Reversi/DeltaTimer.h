#pragma once

// Counts down using delta time
template<typename T = float>
class DeltaTimer
{
	T time;
public:
	// Set seconds to count down from
	void Restart(T t);
	// decrements delta time from timer
	void Update(T dt);
	// returns true when the timer has reached 0
	bool HasElapsed();

};


