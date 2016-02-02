/*
 * 	Copyright (c) 2014 Colorado State University
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
 *	OTHER DEALINGS IN THE SOFTWARE.\
 *
 *
 *  File: StatsTracker.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 24, 2014
 *
 */

// System
#include <time.h>

// Shared

// Module
#include "StatsTracker.h"
#include "Log.h"

StatsTracker StatsTracker::instance_;

StatsTracker::StatsTracker()
  : method_start_time_(0),
    method_end_time_(0)
{

}

StatsTracker::~StatsTracker()
{

}

StatsTracker& StatsTracker::GetInstance()
{
  return instance_;
}

void StatsTracker::Flush()
{
  method_start_time_ = 0;
  method_end_time_ = 0;
}

void StatsTracker::RecordMethodStartTime()
{
  time_t tNow = time(NULL);
  method_start_time_ = static_cast<long int>(tNow);
}

void StatsTracker::RecordMethodEndTime()
{
  time_t tNow = time(NULL);
  method_end_time_ = static_cast<long int>(tNow);
}

void StatsTracker::GetResultPerformanceTime()
{
  if (method_start_time_ == 0)
  {
    elog(INFO, "Unable to get the start time of method.\n");
    return;
  }

  if (method_end_time_ == 0)
  {
    elog(INFO, "Unable to get the end time of method.\n");
    return;
  }

  elog(INFO, "Method execution time: %ld seconds.\n", (method_end_time_ - method_start_time_));
  Flush();
}






