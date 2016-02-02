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
 *  File: Queue.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 10, 2015
 *
 */

#ifndef _QUEUE_H
#define _QUEUE_H

// System
#include <list>

// Shared

// Module
#include <boost/shared_ptr.hpp>
#include "Thread.h"
#include "Mutex.h"

class Task;

class Queue : public Thread
{
public:
		Queue();
		virtual ~Queue();

		bool Init(int p_iMax);

		virtual bool Run();
		virtual Queue *dup();

		bool Clear();
		bool Enqueue(boost::shared_ptr<Task> p_oTask);
		boost::shared_ptr<Task> Dequeue();

protected:
		Mutex &GetMutex();

private:
		int m_iMax;
		int m_iCurrent;
		std::list<boost::shared_ptr<Task>> m_oQueue;
		Mutex m_oMutex;


};

#endif
