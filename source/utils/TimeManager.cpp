// ******************************************************************************
//
// Filename:	TimeManager.cpp
// Project:		Utils
// Author:		Steven Ball
//
// Purpose:
//	 A Manager class to handle all time related functionality. Can be used
//	 to get elapsed time, current tick count and also for a change in time
//	 on a frame by frame basis, to allow for time based animations.
//
// Revision History:
//   Initial Revision - 15/07/08
//
// Copyright (c) 2005-2006, Steven Ball
//
// ******************************************************************************

#include "TimeManager.h"

#include <windows.h>
#include <stdio.h>
#include <Mmsystem.h>

#pragma comment (lib, "Winmm.lib")


// Initialize the singleton instance
TimeManager *TimeManager::c_instance = 0;

TimeManager* TimeManager::GetInstance()
{
	if(c_instance == 0)
		c_instance = new TimeManager;

	return c_instance;
}

void TimeManager::Destroy()
{
	if(c_instance)
	{
		for(unsigned int i = 0; i < m_vpCountdownTimers.size(); i++)
		{
			delete m_vpCountdownTimers[i];
			m_vpCountdownTimers[i] = 0;
		}
		m_vpCountdownTimers.clear();

		delete c_instance;
	}
}

TimeManager::TimeManager()
{
	m_startTickCount = GetTickCount();
	m_ticks = 0;

	QueryPerformanceCounter(&m_FPS_PreviousTicks);
	QueryPerformanceFrequency(&m_FPS_TicksPerSecond);

	m_FPS = 0.0f;
}

bool TimeManager::HasCountdownTimers() const
{
	return(m_vpCountdownTimers.size() > 0);
}

void TimeManager::AddCountdownTimer(CountdownTimer* countdownTimer)
{
	m_vpCountdownTimers.push_back(countdownTimer);
}

void TimeManager::RemoveCountdownTimer(CountdownTimer* countdownTimer)
{
	m_vpCountdownTimers.erase(std::remove(m_vpCountdownTimers.begin(), m_vpCountdownTimers.end(), countdownTimer), m_vpCountdownTimers.end());
}

void TimeManager::RemoveCountdownTimers()
{
	m_vpCountdownTimers.clear();
}

int TimeManager::GetTicks() const
{
	return m_ticks;
}

int TimeManager::GetMilliseconds() const
{
	return m_ticks / 10;
}

int TimeManager::GetSeconds() const
{
	return GetTicks() / 1000;
}

int TimeManager::GetMinutes() const
{
	return GetSeconds() / 60;
}

int TimeManager::GetHours() const
{
	return GetMinutes() / 60;
}

double TimeManager::GetDeltaTime() const
{
	return m_deltaTime;
}

float TimeManager::GetFPS() const
{
	return m_FPS;
}

void TimeManager::UpdateTimer()
{
	// Update the tick counter
	m_ticks = GetTickCount() - m_startTickCount;


	// Update the delta time
	double timeNow = (double)timeGetTime() / 1000.0;
	static double timeOld = timeNow - (1.0/50.0);

	double delta = timeNow - timeOld;
	timeOld = timeNow;

	m_deltaTime = delta;


	// Update the FPS
	QueryPerformanceCounter(&m_FPS_CurrentTicks);
	m_FPS = 1.0f / ((float)(m_FPS_CurrentTicks.QuadPart - m_FPS_PreviousTicks.QuadPart) / (float)m_FPS_TicksPerSecond.QuadPart);
	m_FPS_PreviousTicks = m_FPS_CurrentTicks;


	// Also update all our countdown timers
	CountdownTimerList::iterator iterator;
	for(iterator = m_vpCountdownTimers.begin(); iterator != m_vpCountdownTimers.end(); ++iterator)
	{
		(*iterator)->UpdateCountdown((float)m_deltaTime);
	}
}