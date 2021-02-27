#include "DeltaTimer.h"

template<typename T>
void DeltaTimer<T>::Restart(T t)
{
	time = t;
}

template<typename T>
void DeltaTimer<T>::Update(T dt)
{
	time -= dt;
}

template<typename T>
bool DeltaTimer<T>::HasElapsed()
{
	return time <= 0;
}

