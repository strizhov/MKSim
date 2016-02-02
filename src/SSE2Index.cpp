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
 *  File: SSE2Index.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 13, 2014
 *
 */

// System
#include <iostream>
#include <set>

// Shared

// Module
#include "SSE2Index.h"
#include "Log.h"

SSE2Index::SSE2Index()
{

}

SSE2Index::~SSE2Index()
{

}

bool SSE2Index::AddEntry(const std::string& p_sWord, const std::string& p_sDocumentFilename)
{
  if (p_sWord.empty() == true)
  {
    elog(WARNING, "Word is empty.\n");
    return false;
  }

  if (p_sDocumentFilename.empty() == true)
  {
    elog(WARNING, "Document filename is empty.\n");
    return false;
  }

  m_mIndex.emplace(p_sWord, p_sDocumentFilename);

  return true;
}

void SSE2Index::Clear()
{
  m_mIndex.clear();
}

bool SSE2Index::IsExist(const std::string& p_sWord)
{
  if (p_sWord.empty() == true)
  {
    elog(WARNING, "Given word is empty.\n");
    return false;
  }

  // Check if word exist in the lookup table
  if (m_mIndex.count(p_sWord) > 0)
    return true;

  // Not found
  return false;
}

bool SSE2Index::GetEntry(const std::string& p_sWord, std::string& p_sDocumentFilename)
{
  if (p_sWord.empty() == true)
  {
    elog(WARNING, "Given word is empty.\n");
    return false;
  }

  p_sDocumentFilename = m_mIndex[p_sWord];

  return true;
}

bool SSE2Index::IsEmpty()
{
  return m_mIndex.empty();
}

void SSE2Index::PrintSSE2Index()
{
  for (auto& x: m_mIndex) {
    std::cout << x.first << ": " << x.second << std::endl;
  }
}

const std::map<std::string, std::string>& SSE2Index::GetIndex() const
{
  return m_mIndex;
}

bool SSE2Index::SetIndex(const std::map<std::string, std::string> &p_mIndex)
{
  if (p_mIndex.empty() == true)
  {
    elog(ERROR, "Unable to set new SSE2 index, its empty.\n");
    return false;
  }

  // Clear current index
  Clear();

  // Move elements from given index
  std::move(p_mIndex.begin(), p_mIndex.end(), std::inserter(m_mIndex, m_mIndex.begin()));

  return true;
}

size_t SSE2Index::GetDocumentCollectionSize()
{
  if (IsEmpty() == true)
  {
    elog(ERROR, "SSE2 is empty.\n");
    return 0;
  }

  // Make set of document filenames
  std::set<std::string> sDocumentFilenames;
  for (auto& value : m_mIndex)
  {
    sDocumentFilenames.emplace(value.second);
  }
/*
  for (std::map<std::string, std::string::iterator it=m_mIndex.begin(); it!=m_mIndex.end(); ++it)
    sDocumentFilenames.emplace(it->second);
*/

  return sDocumentFilenames.size();
}
