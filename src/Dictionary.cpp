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
 *  File: Dictionary.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 10, 2014
 *
 */

// System
#include <algorithm>
#include <unordered_set>

// Shared

// Module
#include "Dictionary.h"
#include "Log.h"

Dictionary::Dictionary()
{
}

Dictionary::~Dictionary()
{

}

bool Dictionary::AddWords(const std::string& p_sKeyword)
{
  if (p_sKeyword.empty() == true)
  {
    elog(ERROR, "Keyword to add is empty.\n");
    return false;
  }

  // Append
  m_stKeywordDictionary.insert(p_sKeyword);

  return true;
}

bool Dictionary::AddWords(const std::vector<std::string>& p_vKeywordArray)
{
  if (p_vKeywordArray.empty() == true)
  {
    elog(WARNING, "List of words to add is empty\n");
    return false;
  }

  // Add at the end
  m_stKeywordDictionary.insert(p_vKeywordArray.begin(), p_vKeywordArray.end());

  return true;
}

bool Dictionary::SetDictionary(const std::set<std::string>& p_stKeywordDictionary)
{
  if (p_stKeywordDictionary.empty() == true)
  {
    elog(ERROR, "Keyword dictionary is empty. Unable to set the new one.\n");
    return false;
  }

  Clear();

  std::move(p_stKeywordDictionary.begin(), p_stKeywordDictionary.end(),
            std::inserter(m_stKeywordDictionary, m_stKeywordDictionary.begin()));

  return true;
}

bool Dictionary::IsEmpty()
{
  return m_stKeywordDictionary.empty();
}

void Dictionary::Clear()
{
  m_stKeywordDictionary.clear();
}

const std::set<std::string>& Dictionary::GetDictionary() const
{
  return m_stKeywordDictionary;
}

std::size_t Dictionary::GetSize()
{
  return m_stKeywordDictionary.size();
}


