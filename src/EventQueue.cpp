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
 *  File: EventQueue.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 10, 2015
 *
 */

// System

// Shared

// Module
#include "EventQueue.h"
#include "Event.h"
#include "EventHandler.h"
#include "MutexHandler.h"
#include "Task.h"
#include "Log.h"

EventQueue::EventQueue(boost::shared_ptr<EventHandler> p_oEventHandler) :
  m_oEventHandler(p_oEventHandler)
{

}

EventQueue::~EventQueue()
{

}

boost::shared_ptr<EventHandler> EventQueue::GetEventHandler()
{
  return m_oEventHandler;
}

bool EventQueue::Run()
{
  bool bRet = false;

  while (GetRun())
  {
	bRet = true;

	boost::shared_ptr<Task> pTask = Dequeue();
	boost::shared_ptr<Event> pEvent = boost::static_pointer_cast<Event>(pTask);
	if (NULL == pTask)
	//
	// CRITICAL SECTION BEGIN
	//
	{
	  MutexHandler oMH(GetMutex());
	  GetMutex().Wait(5000);
	}
	//
	// CRITICAL SECTION END
	//
	else if (NULL == pEvent)
	{
	  elog(ERROR, "Unable to cast task for some reason.\n");
	}
	else
	{
	  GetEventHandler()->HandleEvent(pEvent);
	}
  }

  return bRet;

}







