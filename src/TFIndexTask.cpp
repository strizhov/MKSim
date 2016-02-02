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
 *  File: TFIndexTask.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 25, 2015
 *
 */

// System

// Shared

// Module
#include "TFIndexTask.h"
#include "Log.h"

TFIndexTask::TFIndexTask()
{
}

TFIndexTask::TFIndexTask(const std::string& p_sOutputDirectory,
						 const std::string& p_sFilename,
						 const std::vector<long>& p_vTFValues) :
  m_sOutputDirectory(p_sOutputDirectory),
  m_sFilename(p_sFilename),
  m_vTFValues(p_vTFValues)
{
}

TFIndexTask::~TFIndexTask()
{
}

bool TFIndexTask::SetOutputDirectory(const std::string& p_sOutputDirectory)
{
  if (p_sOutputDirectory.size() == 0)
  {
	elog(ERROR, "Output directory is empty.\n");
	return false;
  }

  m_sOutputDirectory = p_sOutputDirectory;

  return true;
}

std::string TFIndexTask::GetOutputDirectory() const
{
  return m_sOutputDirectory;
}

bool TFIndexTask::SetFileName(const std::string &p_sFilename)
{
  if (p_sFilename.size() == 0)
  {
	elog(ERROR, "Filename is empty.\n");
	return false;
  }

  m_sFilename == p_sFilename;

  return true;
}

std::string TFIndexTask::GetFilename() const
{
  return m_sFilename;
}

bool TFIndexTask::SetTFValues(const std::vector<long> p_vTFValues)
{
  if (p_vTFValues.size() == 0)
  {
	elog(ERROR, "No TF values found.\n");
	return false;
  }

  m_vTFValues = p_vTFValues;

  return true;
}

std::vector<long> TFIndexTask::GetTFValues() const
{
  return m_vTFValues;
}
