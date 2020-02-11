
#include "Timer.h"
#include <SDL_timer.h>

Timer::Timer()
{
	__int64 countsPerSec = SDL_GetPerformanceFrequency();
	m_SecondsPerCount = 1.0 / (double)countsPerSec;
}

float Timer::TotalTime()const
{
	if (m_Stopped)
	{
		return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
	else
	{
		return (float)(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
}

float Timer::DeltaTime()const
{
	return (float)m_DeltaTime;
}

void Timer::Reset()
{
	__int64 currTime = SDL_GetPerformanceCounter();

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_Stopped = false;
}

void Timer::Start()
{
	__int64 startTime = SDL_GetPerformanceCounter();

	if (m_Stopped)
	{
		m_PausedTime += (startTime - m_StopTime);

		m_PrevTime = startTime;
		m_StopTime = 0;
		m_Stopped = false;
	}
}

void Timer::Stop()
{
	if (!m_Stopped)
	{
		__int64 currTime = SDL_GetPerformanceCounter();

		m_StopTime = currTime;
		m_Stopped = true;
	}
}

void Timer::Tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.0;
		return;
	}

	__int64 currTime = SDL_GetPerformanceCounter();
	m_CurrTime = currTime;

	// Time difference between this frame and the previous.
	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;

	// Prepare for next frame.
	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0;
	}
}