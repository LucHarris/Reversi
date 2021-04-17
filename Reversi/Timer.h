#pragma once
#include <chrono>
class Timer
{

private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	//using second_t = std::chrono::duration<double, std::ratio<1>>;
	using millisec_t = std::chrono::duration<double, std::milli>;

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

	double elapsed() const
	{
		std::chrono::time_point<clock_t> end(clock_t::now());
		return std::chrono::duration_cast<millisec_t>(end - mStart).count();
	}

};

