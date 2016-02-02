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
 *	OTHER DEALINGS IN THE SOFTWARE.\
 *
 *
 *  File: SSE2SearchResults.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: May 27, 2015
 *
 */

// System
#include <set>
#include <string>

// Shared

// Module
#include "SSE2SearchResults.h"
#include "Log.h"

SSE2SearchResults::SSE2SearchResults()
{

}

SSE2SearchResults::~SSE2SearchResults()
{

}

bool SSE2SearchResults::AddResult(const std::string &p_sDocumentFilename)
{
  if (p_sDocumentFilename.empty() == true)
  {
    elog(ERROR, "SSE2 result is empty.\n");
    return false;
  }

  m_sSSE2Results.insert(p_sDocumentFilename);

  return true;
}

bool SSE2SearchResults::Empty()
{
  return m_sSSE2Results.empty();
}

void SSE2SearchResults::Clear()
{
  m_sSSE2Results.clear();
}

size_t SSE2SearchResults::Size()
{
  return m_sSSE2Results.size();
}

const std::set<std::string>& SSE2SearchResults::GetResults() const
{
  return m_sSSE2Results;
}

bool SSE2SearchResults::GetResultsInVector(std::vector<std::string>& p_vSSE2Results)
{
  if (m_sSSE2Results.empty() == true)
  {
    elog(ERROR, "SSE2 results are empty.\n");
    return false;
  }

  std::copy(m_sSSE2Results.begin(), m_sSSE2Results.end(), std::back_inserter(p_vSSE2Results));

  return true;
}
