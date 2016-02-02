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
 *  File: DocumentIndex.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 6, 2014 
 *
 */


// System
#include <unordered_map>

// Shared

// Module
#include <boost/filesystem/path.hpp>
#include "DocumentIndex.h"
#include "Log.h"

DocumentIndex::DocumentIndex(const boost::filesystem::path& p_oDocumentFilename):
    m_oDocumentFilename(p_oDocumentFilename)
{}

DocumentIndex::~DocumentIndex()
{}

boost::filesystem::path DocumentIndex::GetFilename() const
{
  return m_oDocumentFilename;
}

void DocumentIndex::AddWords(const std::vector<std::string>& p_vDocumentWordList)
{
  // Add each word and increment the appearance counter
  for (const std::string& sWord : p_vDocumentWordList)
  {
    m_umWordsWithCounters[sWord] ++;
  }
}

bool DocumentIndex::CheckWordAppearance(const std::string& p_sWord) const
{
  bool bRet = false;

  // Find a given word
  auto search_result = m_umWordsWithCounters.find(p_sWord);

  // Check resulted search
  if(search_result != m_umWordsWithCounters.end())
  {
    bRet = true;
  }

  return bRet;
}

long DocumentIndex::GetWordAppearance(const std::string& p_sWord) const
{
  // Check existence of the word
  if (CheckWordAppearance(p_sWord) == false)
  {
    elog(ERROR, "Word '%s'' does not exist in the document '%s'\n", p_sWord.c_str(), m_oDocumentFilename.string().c_str());
    return 0;
  }

  return m_umWordsWithCounters.at(p_sWord);
}

const std::unordered_map<std::string, long>& DocumentIndex::GetDocumentWordList() const
{
  return m_umWordsWithCounters;
}

bool DocumentIndex::SetTF(const std::string& p_sWord, const long& p_lValue)
{
  if (p_sWord.empty() == true)
  {
    elog(ERROR, "Keyword is empty. Unable to set TF value.\n");
    return false;
  }

  m_vTermWeights.emplace_back(p_sWord, p_lValue);

  return true;
}

bool DocumentIndex::GetTFWords(std::vector<std::string>& p_vTFWords) const
{
  if (!p_vTFWords.empty())
  {
    elog(ERROR, "TF vector is not empty\n");
    return false;
  }

  for (const TF& oTFItem : m_vTermWeights)
  {
    p_vTFWords.emplace_back(oTFItem.GetWord());
  }

  return true;
}

bool DocumentIndex::CheckWordExistence(const std::string& p_sWord) const
{
  for (const TF& oTFItem : m_vTermWeights)
  {
    if (oTFItem.GetWord().compare(p_sWord) == 0)
    {
      return true;
    }
  }

  return false;
}

long DocumentIndex::GetTFValueByWord(const std::string& p_sWord) const
{
  for (const TF& oTFItem : m_vTermWeights)
  {
    if (oTFItem.GetWord().compare(p_sWord) == 0)
    {
      return oTFItem.GetValue();
    }
  }

  // Return negative value, indicate error;
  return -1;
}

bool DocumentIndex::GetTfValues(std::vector<long>& p_vTFValues) const
{
  if (p_vTFValues.empty() == false)
  {
    elog(ERROR, "TF vector is not empty!\n");
    return false;
  }

  for (const TF& oTFItem : m_vTermWeights)
  {
    p_vTFValues.emplace_back(oTFItem.GetValue());
  }

  return true;
}

bool DocumentIndex::SetEncryptedTF(std::vector<std::string>& p_vEncrTermWeights)
{
  if (p_vEncrTermWeights.empty() == true)
  {
    elog(ERROR, "Encrypted TF vector is empty\n");
    return false;
  }

  m_vEncrTermWeights = p_vEncrTermWeights
      ;
  return true;
}

const std::vector<std::string>& DocumentIndex::GetEncryptedTF() const
{
  return m_vEncrTermWeights;
}



