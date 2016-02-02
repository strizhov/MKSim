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
 *  File: Thread.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 3, 2015
 *
 */


// System
#include <string.h>
#include <errno.h>
#include <pthread.h>

// Shared
#include "site_defs.h"

// Module
#include "Thread.h"
#include "ThreadPool.h"
#include "Task.h"
#include "Log.h"


void *_run(void *p_pParam)
{
  if (NULL == p_pParam)
  {
	elog(CRITICAL, "Callback got NULL paramerer.\n");
  }
  else
  {
	((Thread *) p_pParam)->Run();
	elog(CRITICAL, "Thread is run.\n");
  }

  return NULL;
}

Thread::Thread(ThreadPool *p_pPool) :
  m_bRun_(false),
  m_pTask_(NULL),
  m_pPool_(p_pPool)
{

}

Thread::~Thread()
{

}

bool Thread::Start()
{
  bool bRet = false;

  int iErr = 0;
  if (m_bRun_)
  {
	elog(CRITICAL, "Thread already running.\n");
  }
  else
  {
	m_bRun_ = true;
	if (0 != (iErr = pthread_create(&m_tID_, NULL, _run, this)))
	{
	  elog(CRITICAL, "Unable to start thread: %s\n", strerror(iErr));
	}
	else
	{
	  bRet = true;
	}
  }

  return bRet;

}

bool Thread::Run()
{
  bool bRet = false;

  while (m_bRun_)
  {
	bRet = true;

	boost::shared_ptr<Task> pTask = GetTask();
	if (NULL == pTask)
	{
	  sleep(1);
	}
	else
	{
	  if (!pTask->Execute())
	  {
		elog(CRITICAL, "Unable to execute task.\n");
	  }

	  SetTask(NULL);

	  if (NULL != m_pPool_ && !m_pPool_->CheckIn(*this))
	  {
		elog(CRITICAL, "Unable to check this thread in\n");
	  }
	}
  }

  return bRet;

}

bool Thread::Kill()
{
  m_bRun_ = false;

  return !m_bRun_;
}

bool Thread::Join()
{
  int iErr = pthread_join(m_tID_, NULL);
  if (0 != iErr)
  {
	elog(CRITICAL, "Unable to join thread: %s\n", strerror(iErr));
  }

  return (0 == iErr);

}

bool Thread::GetRun()
{
  return m_bRun_;
}

boost::shared_ptr<Task> Thread::GetTask()
{
  return m_pTask_;
}

void Thread::SetTask(boost::shared_ptr<Task> p_pTask)
{
  m_pTask_ = p_pTask;
}

ThreadPool *Thread::GetPool()
{
  return m_pPool_;
}

void Thread::SetPool(ThreadPool *p_pPool)
{
  m_pPool_ = p_pPool;
}

Thread *Thread::dup()
{
  return new Thread(m_pPool_);
}
