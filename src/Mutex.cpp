/*
 * 	Copyright (c) 2015 Colorado State University
 *
 *	Permission is hereby granted, free of charge, to any person
 *	obtaining a copy of this software and associated documentation
 *	files (the "Software"), to deal in the Software without
 *	restriction, including without limitation the rights to use,
 *	copy, modify, merge, publish, distribute, sublicense, and/or
 *	sell copies of the Software, and to permit persons to whom
 *	the Software is furnished to do so, subject to the following
 *	conditions:
 *
 *	The above copyright notice and this permission notice shall be
 *	included in all copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *	OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 *  File: Mutex.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 4, 2015
 *
 */

// System
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

// Shared

// Module
#include "Mutex.h"
#include "Log.h"

Mutex::Mutex()
{
  int iErr = pthread_mutex_init(&m_tMutex_, NULL);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to init mutex: %s\n", strerror(iErr));
  }

  iErr = pthread_cond_init(&m_tCond_, NULL);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to init cond: %s\n", strerror(iErr));
  }
}

Mutex::~Mutex()
{
  pthread_mutex_destroy(&m_tMutex_);
  pthread_cond_destroy(&m_tCond_);
}

bool Mutex::Lock()
{
  bool bRet = false;

  int iErr = pthread_mutex_lock(&m_tMutex_);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to lock mutex: %s\n", strerror(iErr));
  }
  else
  {
	bRet = true;
  }

  return bRet;
}

bool Mutex::Unlock()
{
  bool bRet = false;

  int iErr = pthread_mutex_unlock(&m_tMutex_);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to unlock mutex: %s\n", strerror(iErr));
  }
  else
  {
	bRet = true;
  }

  return bRet;
}

bool Mutex::Wait(long p_lMillis)
{
  bool bRet = false;

  int iErr = 0;
  if (0 == p_lMillis)
  {
	iErr = pthread_cond_wait(&m_tCond_, &m_tMutex_);
  }
  else
  {
	struct timespec tSpec;
	tSpec.tv_sec = time(NULL) + ((int) (p_lMillis/1000));
	tSpec.tv_nsec = p_lMillis % 1000;

	iErr = pthread_cond_timedwait(&m_tCond_, &m_tMutex_, &tSpec);
  }

  if (0 != iErr && ETIMEDOUT != iErr)
  {
	elog(CRITICAL, "Unable to wait: %s\n", strerror(iErr));
  }
  else
  {
	bRet = true;
  }

  return bRet;
}

bool Mutex::Signal()
{
  bool bRet = false;

  int iErr = pthread_cond_signal(&m_tCond_);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to signal: %s\n", strerror(iErr));
  }
  else
  {
	bRet = true;
  }

  return bRet;
}

bool Mutex::SignalAll()
{
  bool bRet = false;

  int iErr = pthread_cond_broadcast(&m_tCond_);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to broadcast: %s\n", strerror(iErr));
  }
  else
  {
	bRet = true;
  }

  return bRet;
}
