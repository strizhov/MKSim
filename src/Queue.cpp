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
 *  File: Queue.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 10, 2015
 *
 */

// System

// Shared

// Module
#include "Queue.h"
#include "Task.h"
#include "Log.h"
#include "MutexHandler.h"

Queue::Queue() :
  m_iMax(0),
  m_iCurrent(0)
{

}
Queue::~Queue()
{
  Clear();
}

bool Queue::Init(int p_iMax)
{
  Clear();
  m_iMax = p_iMax;

  return true;
}

bool Queue::Run()
{
  bool bRet = false;

  while (GetRun())
  {
	bRet = true;

	boost::shared_ptr<Task> pTask = Dequeue();
	if (NULL == pTask)
	{
	  m_oMutex.Wait(5000);
	}
	else
	{
	  pTask->Execute();
	}
  }

  return bRet;
}

Queue *Queue::dup()
{
  return new Queue();
}

bool Queue::Clear()
{
   m_oQueue.clear();

   return true;

}

bool Queue::Enqueue(boost::shared_ptr<Task> p_oTask)
{
  bool bRet = false;

  while (!bRet)
  //
  // CRITICAL SECTION BEGIN
  //
  {
	MutexHandler oMH(m_oMutex);

	if (m_iCurrent >= m_iMax)
	{
	  elog(WARNING, "Queue full, waiting to empty...\n");
	  m_oMutex.Wait(5000);
	}
	else
	{
	  m_oQueue.push_back(p_oTask);
	  m_iCurrent++;
	  bRet = true;
	}

	m_oMutex.Signal();
  }
  //
  // CRITICAL SECTION END
  //

  return bRet;

}

boost::shared_ptr<Task> Queue::Dequeue()
{
  boost::shared_ptr<Task> pRet = NULL;

  if (m_iCurrent < 0)
  {
    elog(CRITICAL, "Queue has less than 0 entries in it: Error!\n");
  }
  else if (m_iCurrent !=  0)
  //
  // BEGIN CRITICAL SECTION
  //
  {
	MutexHandler oMH(m_oMutex);

	pRet = m_oQueue.front();
	m_oQueue.pop_front();
	m_iCurrent--;
	m_oMutex.Signal();
  }
  //
  // CRITICAL SECTION END
  //

  return pRet;
}

Mutex &Queue::GetMutex()
{
  return m_oMutex;
}


