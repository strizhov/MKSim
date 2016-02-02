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
 *  File: Thread.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 3, 2015
 *
 */

#ifndef _THREAD_H
#define _THREAD_H

// System
#include <pthread.h>

// Shared

// Module
#include <boost/shared_ptr.hpp>

class ThreadPool;
class Task;

class Thread
{
public:
		Thread(ThreadPool *p_pPool = NULL);
		virtual ~Thread();

		bool Start();
		virtual bool Run();
		bool Kill();
		bool Join();

		bool GetRun();

		boost::shared_ptr<Task> GetTask();
		void SetTask(boost::shared_ptr<Task> p_pTask);

		ThreadPool *GetPool();
		void SetPool(ThreadPool *p_pPool);

		virtual Thread *dup();

private:
		bool m_bRun_;
		boost::shared_ptr<Task> m_pTask_;
		pthread_t m_tID_;
		ThreadPool *m_pPool_;
};

#endif
