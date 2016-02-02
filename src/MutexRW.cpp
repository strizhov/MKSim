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
 *  File: MutexRW.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 4, 2015
 *
 */

// System
#include <string.h>
#include <errno.h>
#include <pthread.h>

// Shared

// Module
#include "MutexRW.h"
#include "Log.h"

MutexRW::MutexRW()
{
  int iErr = pthread_rwlock_init(&m_tRwMutex_, NULL);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to init rw lock: %s\n", strerror(iErr));
  }
}

MutexRW::~MutexRW()
{
  pthread_rwlock_destroy(&m_tRwMutex_);
}

bool MutexRW::ReadLock()
{
  bool bRet = false;

  elog(DEBUG, "Creating READ lock now...\n");
  int iErr = pthread_rwlock_rdlock(&m_tRwMutex_);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to read lock rw mutex: %s\n", strerror(iErr));
  }
  else
  {
	elog(DEBUG, "Obtained READ lock.\n");
	bRet = true;
  }

  return bRet;
}

bool MutexRW::WriteLock()
{
  bool bRet = false;

  elog(DEBUG, "Creating WRITE lock now...\n");
  int iErr = pthread_rwlock_wrlock(&m_tRwMutex_);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to write lock rw mutex: %s\n", strerror(iErr));
  }
  else
  {
	elog(DEBUG, "Obtained WRITE lock.\n");
	bRet = true;
  }

  return bRet;
}

bool MutexRW::Unlock()
{
  bool bRet = false;

  int iErr = pthread_rwlock_unlock(&m_tRwMutex_);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to unlock rw mutex: %s\n", strerror(iErr));
  }
  else
  {
	bRet = true;
  }

  return bRet;
}
