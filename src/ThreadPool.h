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
 *  File: ThreadPool.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 3, 2015
 *
 */

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

// System
#include <pthread.h>

// Shared

// Module
#include "Mutex.h"

class Thread;

class ThreadPool
{
public:
		ThreadPool();
		virtual ~ThreadPool();

		bool Init(Thread *p_pThreadType = NULL);

		int GetSize();
		void SetSize(int p_iSize);

		int GetNumFree();

		bool CheckIn(Thread &p_oThrd);
		Thread *CheckOut();

		bool Kill();

private:
		bool m_bRun_;
		int m_iSize_;
		std::list<Thread *> m_oThreads_;
		std::list<Thread *> m_oFree_;
		Mutex m_oMutex_;
};

#endif
