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
 *  File: MutexHandler.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 4, 2015
 *
 */

// System

// Shared

// Module
#include "MutexHandler.h"
#include "Mutex.h"
#include "MutexRW.h"
#include "Log.h"

MutexHandler::MutexHandler(Mutex &p_oMutex) :
  m_oMutex_(p_oMutex)
{
  if (!m_oMutex_.Lock())
  {
	elog(CRITICAL, "Unable to lock mutex.\n");
  }
}

MutexHandler::MutexHandler(MutexRW &p_oMutex, bool p_bRead) :
  m_oMutex_(p_oMutex)
{
  if (p_bRead && !p_oMutex.ReadLock())
  {
	elog(CRITICAL, "Unable to read lock mutex.\n");
  }
  else if (!p_bRead && !p_oMutex.WriteLock())
  {
	elog(CRITICAL, "Unable to write lock mutex.\n");
  }
}

MutexHandler::~MutexHandler()
{
  if (!m_oMutex_.Unlock())
  {
	elog(CRITICAL, "Unable to unlock mutex.\n");
  }
}
