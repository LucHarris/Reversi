#pragma once
#include <chrono>

template <typename REP = int, typename PERIOD = std::micro>
class Timer
{

private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	//using second_t = std::chrono::duration<double, std::ratio<1>>;
	using period_t = std::chrono::duration<REP, PERIOD>;

	std::chrono::time_point<clock_t> mStart;

public:
	Timer() 
		: 
		mStart(clock_t::now())
	{
	}

	void Reset()
	{
		mStart = clock_t::now();
	}

	REP elapsed() const
	{
		std::chrono::time_point<clock_t> end(clock_t::now());
		return std::chrono::duration_cast<period_t>(end - mStart).count();
	}

};

