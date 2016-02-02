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
 *  File: SearchQuery.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 10, 2014
 *
 */

// System

// Shared
#include "site_defs.h"

// Module
#include "SearchQuery.h"
#include "Log.h"

SearchQuery::SearchQuery()
{
}

SearchQuery::~SearchQuery()
{
}

void SearchQuery::SetSearchQuery(const long& p_lValue)
{
  m_vSearchQuery.emplace_back(p_lValue);
}

void SearchQuery::ClearSearchQuery()
{
  m_vSearchQuery.clear();
}

const std::vector<long>& SearchQuery::GetSearchQuery() const
{
  return m_vSearchQuery;
}

bool SearchQuery::IsSearchQueryEmpty()
{
  return m_vSearchQuery.empty();
}

bool SearchQuery::SetEncryptedSearchQuery(const std::vector<std::string>& p_vEncrSearchQuery)
{
  if (p_vEncrSearchQuery.empty() == true)
  {
    elog(ERROR, "Ciphtertext is empty\n");
    return false;
  }

  // Append
  m_vEncrSearchQuery.insert(m_vEncrSearchQuery.end(), p_vEncrSearchQuery.begin(), p_vEncrSearchQuery.end());
  return true;
}

void SearchQuery::ClearEncryptedSearchQuery()
{
  m_vEncrSearchQuery.clear();
}

const std::vector<std::string>& SearchQuery::GetEncryptedSearchQuery() const
{
  return m_vEncrSearchQuery;
}

bool SearchQuery::IsEncryptedSearchQueryEmpty()
{
  return m_vEncrSearchQuery.empty();
}


