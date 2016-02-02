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
 *  File: ThreadPool.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 3, 2015
 *
 */

// System
#include <pthread.h>

// Shared
#include "site_defs.h"

// Module
#include "ThreadPool.h"
#include "Thread.h"
#include "MutexHandler.h"

#include "Log.h"

ThreadPool::ThreadPool() :
	m_bRun_(false),
	m_iSize_(10)
{

}

ThreadPool::~ThreadPool()
{
  Kill();
}

bool ThreadPool::Init(Thread *p_pThreadType)
{
  bool bRet = false;

  if (m_bRun_)
  {
	elog(CRITICAL, "Pool is already running.\n");
  }
  else
  {
	Thread oDefaultThrd(this);
	m_bRun_ = true;
	bRet = true;
	for (int i = 0; i < GetSize(); i++)
	{
	  Thread *pThr = NULL;
	  pThr = (NULL == p_pThreadType) ?  oDefaultThrd.dup() : p_pThreadType->dup();

	  if (!pThr->Start())
	  {
		elog(CRITICAL, "Unable to start thread, killing...\n");
		pThr->Kill();
		delete pThr;
		bRet = false;
		m_bRun_ = false;
		break;
	  }
	  else
	  {
		m_oThreads_.push_back(pThr);
		m_oFree_.push_back(pThr);
	  }
	}
  }

  return bRet;

}

int ThreadPool::GetSize()
{
  return m_iSize_;
}

void ThreadPool::SetSize(int p_iSize)
{
  m_iSize_ = p_iSize;
}

int ThreadPool::GetNumFree()
{
  return (int) m_oFree_.size();
}

bool ThreadPool::CheckIn(Thread &p_oThrd)
{
  bool bRet = false;

  //
  // BEGIN CRITICAL SECTION
  //
  {
	MutexHandler oHandler(m_oMutex_);

	m_oFree_.push_back(&p_oThrd);
	m_oMutex_.Signal();
	bRet = true;
  }
  //
  // END CRITICAL SECTION
  //

  return bRet;

}

Thread *ThreadPool::CheckOut()
{
  Thread *pRet = NULL;

  while (NULL == pRet && m_bRun_)
  //
  // BEGIN CRITICAL SECTION
  //
  {
	MutexHandler oHandler(m_oMutex_);

	if (!m_oFree_.empty())
	{
	  pRet = m_oFree_.front();
	  m_oFree_.pop_front();
	}
	else
	{
	  m_oMutex_.Wait(1000);
	}
  }
  //
  // END CRITICAL SECTION
  //

  return pRet;

}

bool ThreadPool::Kill()
{
  m_bRun_ = false;

  std::list<Thread *>::const_iterator tIter;
  for (tIter = m_oThreads_.begin();
	   m_oThreads_.end() != tIter;
	   tIter++)
  {
	(*tIter)->Kill();
  }

  for (tIter = m_oThreads_.begin();
	   m_oThreads_.end() != tIter;
	   tIter++)
  {
	(*tIter)->Join();
	delete (*tIter);
  }

  m_oThreads_.clear();
  m_oFree_.clear();

  return !m_bRun_;

}


