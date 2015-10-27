// ******************************************************************************
//
// Filename:	TimeManager.h
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

#pragma once

#include "CountdownTimer.h"

#include <windows.h>  // Needed for LARGE_INTEGER

#include <vector>
#include <algorithm>

typedef std::vector<CountdownTimer*> CountdownTimerList;


class TimeManager
{
public:
	/* Public methods */
	static TimeManager* GetInstance();
	void Destroy();

	bool HasCountdownTimers() const;
	void AddCountdownTimer(CountdownTimer* countdownTimer);
	void RemoveCountdownTimer(CountdownTimer* countdownTimer);
	void RemoveCountdownTimers();

	int GetTicks() const;
	int GetMilliseconds() const;
	int GetSeconds() const;
	int GetMinutes() const;
	int GetHours() const;
	double GetDeltaTime() const;

	float GetFPS() const;

	void UpdateTimer();	// Update the timer

protected:
	/* Protected methods */
	TimeManager();
	TimeManager(const TimeManager&);
	TimeManager &operator=(const TimeManager&);

private:
	/* Private methods */

public:
	/* Public members */

protected:
	/* Protected members */

private:
	/* Private members */
	int m_startTickCount;		// The tick count from when the timer was started
	int m_ticks;				// The current elapsed tick count since timer was started
	double m_deltaTime;			// The current delta time, updated every time the timer is updated (Every frame!)

	LARGE_INTEGER m_FPS_TicksPerSecond;
	LARGE_INTEGER m_FPS_CurrentTicks;
	LARGE_INTEGER m_FPS_PreviousTicks;

	float m_FPS;

	// A dynamic array of our countdown timers
	CountdownTimerList m_vpCountdownTimers;

	// Singleton instance
	static TimeManager *c_instance;
};
