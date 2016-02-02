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
 *	OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 *  File: SSE2SearchQuery.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 14, 2014
 *
 */

// System

// Shared

// Module
#include "SSE2SearchQuery.h"
#include "Log.h"

SSE2SearchQuery::SSE2SearchQuery()
{

}

SSE2SearchQuery::~SSE2SearchQuery()
{

}

bool SSE2SearchQuery::AddWord(const std::string& p_sWord)
{
  if (p_sWord.empty() == true)
  {
    elog(ERROR, "Search query input is empty.\n");
    return false;
  }

  m_vSearchQuery.emplace_back(p_sWord);

  return true;
}

bool SSE2SearchQuery::IsSSE2SearchQueryEmpty()
{
  return m_vSearchQuery.empty();
}

const std::vector<std::string>& SSE2SearchQuery::GetSearchQuery() const
{
  return m_vSearchQuery;
}

void SSE2SearchQuery::ClearSSE2SearchQuery()
{
  m_vSearchQuery.clear();
}

bool SSE2SearchQuery::AddEncryptedWord(const std::string &p_sEncrWord)
{
  if (p_sEncrWord.empty() == true)
  {
    elog(ERROR, "Encrypted search query input is empty.\n");
    return false;
  }

  m_vEncrSearchQuery.emplace_back(p_sEncrWord);

  return true;
}

bool SSE2SearchQuery::IsSSE2EncryptedSearchQueryEmpty()
{
  return m_vEncrSearchQuery.empty();
}

const std::vector<std::string>& SSE2SearchQuery::GetEncryptedSearchQuery() const
{
  return m_vEncrSearchQuery;
}

void SSE2SearchQuery::ClearSSE2EncryptedSearchQuery()
{
  m_vEncrSearchQuery.clear();
}


