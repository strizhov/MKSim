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
 *  File: SearchEngine.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: May 27, 2014
 *
 */

// System
#include <iostream>
#include <iterator>
#include <vector>
#include <stdlib.h>
#include <time.h>

// Shared
#include "site_defs.h"

// Module
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>
#include "SearchEngine.h"
#include "Log.h"
#include "StatsTracker.h"
#include "SimilarityMeasureTask.h"
#include "DecryptSimilarityMeasureTask.h"

SearchEngine::SearchEngine(const std::string& p_sEncrIndexDirectory,
						   boost::shared_ptr<FileManager> p_oFileManager,
						   boost::shared_ptr<LTCManager> p_oLTCManager,
						   boost::shared_ptr<HEManager> p_oHEManager,
						   boost::shared_ptr<ThreadPool> p_oPool,
						   boost::shared_ptr<EventQueue> p_oEventQueue) :
  shutdown_(true),
  m_sEncrIndexDirectory(p_sEncrIndexDirectory),
  m_oFileManager(p_oFileManager),
  m_oLTCManager(p_oLTCManager),
  m_oHEManager(p_oHEManager),
  m_oPool(p_oPool),
  m_oEventQueue(p_oEventQueue)
{
}

SearchEngine::~SearchEngine()
{
}

bool SearchEngine::Init()
{
  m_oSSE2Index = boost::make_shared<SSE2Index>();
  m_oSSE2SearchQuery = boost::make_shared<SSE2SearchQuery>();
  m_oSSE2SearchResults = boost::make_shared<SSE2SearchResults>();
  m_oDictionary = boost::make_shared<Dictionary>();
  m_oTFSearchQuery = boost::make_shared<SearchQuery>();
  m_oSimilarityMeasureResults = boost::make_shared<SimilarityMeasureResults>();

  return true;
}

bool SearchEngine::SearchIndex(const std::string& p_sInput)
{
  elog(DEBUG, "Checking search input begins.\n");

  if (p_sInput.empty() == true)
  {
    elog(ERROR, "Search input is empty\n");
    return false;
  }

  elog(DEBUG, "Checking search input ends.\n");

  // Read SSE2 Index
  elog(DEBUG, "Reading SSE2 Index from begins.\n");
  if (ReadSSE2Index() == false)
  {
    elog(ERROR, "Unable to read SSE2 index.\n");
    return false;
  }
  elog(DEBUG, "Reading SSE2 Index from ends.\n");


  elog(DEBUG, "Construction of SSE2 search query begins.\n");

  if (ConstructSSE2SearchQuery(p_sInput) == false)
  {
    elog(ERROR, "Unable to construct SSE2 search query.\n");
    return false;
  }
  elog(DEBUG, "Construction of SSE2 search query ends.\n");

  elog(DEBUG, "Construction of encrypted SSE2 search query begins.\n");
  if (EncryptSSE2SearchQuery() == false)
  {
    elog(ERROR, "Unable to encrypt SSE2 search query.\n");
    return false;
  }
  elog(DEBUG, "Construction of encrypted SSE2 search query ends.\n");

  elog(DEBUG, "SSE2 search begins.\n");
  if (SearchSSE2Index() == false)
  {
    elog(ERROR, "Unable to search SSE2 index.\n");
    return false;
  }
  elog(DEBUG, "SSE2 search ends.\n");

  elog(DEBUG, "Reading keyword dictionary from the file begins.\n");
  if (ReadKeywordDictionary() == false)
  {
    elog(ERROR, "Unable to read keyword dictionary.\n");
    return false;
  }

  elog(DEBUG, "Construction of TF search query begins.\n");
  if (ConstructTFSearchQuery(p_sInput) == false)
  {
    elog(ERROR, "Unable to construct search query\n");
    return false;
  }
  elog(DEBUG, "Construction of TF search query ends.\n");

  elog(DEBUG, "Construction of encrypted TF search query begins.\n");
  if (EncryptTFSearchQuery() == false)
  {
    elog(ERROR, "Unable to encrypt TF search query\n");
    return false;
  }
  elog(DEBUG, "Construction of encrypted TF search query ends.\n");

  elog(DEBUG, "Measurement of document similarity begins.\n");
  if (MeasureEncryptedSimilarity() == false)
  {
    elog(WARNING, "Unable to measure document similarity\n");
    return false;
  }
  elog(DEBUG, "Measurement of document similarity ends.\n");

  elog(DEBUG, "Decryption of similarity coefficients begins.\n");
  if (DecryptSimilarityResults() == false)
  {
    elog(WARNING, "Unable to decrypt similarity coefficient results.\n");
    return false;
  }
  elog(DEBUG, "Decryption of similarity coefficients ends.\n");

  return true;
}

bool SearchEngine::ReadSSE2Index()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (m_sEncrIndexDirectory.empty() == true)
  {
    elog(ERROR, "Directory with encrypted index is empty.\n");
    return false;
  }

  std::map<std::string, std::string> mSSE2Values;
  if (m_oFileManager->ReadSSE2Index(m_sEncrIndexDirectory,
                                      SSE2_INDEX_FILENAME,
                                      mSSE2Values) == false)
  {
    elog(ERROR, "SSE2 index read failed.\n");
    return false;
  }

  // Create SSE2 index
  if (m_oSSE2Index->SetIndex(mSSE2Values) == false)
  {
    elog(ERROR, "Unable to set new SSE2 index.\n");
    return false;
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

bool SearchEngine::ConstructSSE2SearchQuery(const std::string& p_sWordInput)
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  // Check if search input is not empty
  if (p_sWordInput.empty() == true)
  {
    elog(ERROR, "Search input is empty.\n");
    return false;
  }

  // Check if SSE2 index is not empty
  if (m_oSSE2Index->IsEmpty() == true)
  {
    elog(ERROR, "No entries found in SSE2 index. Is it empty?.\n");
    return false;
  }

  // Clean up previous search query
  if (m_oSSE2SearchQuery->IsSSE2SearchQueryEmpty() == false)
    m_oSSE2SearchQuery->ClearSSE2SearchQuery();

  // Array of words from the input. May contain repeated words.
  std::vector<std::string> vSSE2WordQuery;

  // Use same chomp strategy for search input, return filled array of words
  if (ChompWordInput(p_sWordInput, vSSE2WordQuery) == false)
  {
    elog(ERROR, "Unable to chomp the input '%s'.\n", p_sWordInput.c_str());
    return false;
  }


  size_t szDocumentCollectionSize = m_oSSE2Index->GetDocumentCollectionSize();
  elog(DEBUG, "Document collection size is '%d'.\n", szDocumentCollectionSize);

  // Iterate on each word, contatenate document id with each word
  for (const std::string& sWord : vSSE2WordQuery)
  {
    for (size_t i = 0; i < szDocumentCollectionSize; i++)
    {
      std::string sSSE2QueryKey = sWord + boost::lexical_cast<std::string>(i);
      m_oSSE2SearchQuery->AddWord(sSSE2QueryKey);
    }
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

bool SearchEngine::EncryptSSE2SearchQuery()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  // Make sure previous search query is not empty
  if (m_oSSE2SearchQuery->IsSSE2SearchQueryEmpty() == true)
  {
    elog(ERROR, "SSE2 search query is empty.\n");
    return false;
  }

  // Cleanup previous encrypted search query
  m_oSSE2SearchQuery->ClearSSE2EncryptedSearchQuery();

  // Get the vector of plaintext search query
  const std::vector<std::string>& vSearchVector = m_oSSE2SearchQuery->GetSearchQuery();

  for (const std::string& sWord : vSearchVector)
  {
    // Encrypt it!
    std::string sEncrWord;
    if (m_oLTCManager->CalculateHMAC(sWord, sEncrWord) == false)
    {
      elog(ERROR, "Unable to encrypt search query word '%s'.\n", sWord.c_str());
      return false;
    }

    // Add encrypted word to SSE2 search query
    m_oSSE2SearchQuery->AddEncryptedWord(sEncrWord);
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

bool SearchEngine::SearchSSE2Index()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (m_oSSE2SearchQuery->IsSSE2EncryptedSearchQueryEmpty() == true)
  {
    elog(ERROR, "Encrypted SSE2 search query is empty.\n");
    return false;
  }

  if (m_oSSE2Index->IsEmpty() == true)
  {
    elog(ERROR, "SSE2 index is empty.\n");
    return false;
  }

  // Clean up previous searches
  if (m_oSSE2SearchResults->Empty() == false)
    m_oSSE2SearchResults->Clear();

  // Get the array of encrypted search query
  const std::vector<std::string>& vEncrSearchVector = m_oSSE2SearchQuery->GetEncryptedSearchQuery();

  for (const std::string& sEncrWord : vEncrSearchVector)
  {
    if (m_oSSE2Index->IsExist(sEncrWord) == true)
    {
      // Retrieve the result
      std::string sDocumentFilename;
      if (m_oSSE2Index->GetEntry(sEncrWord, sDocumentFilename) == false)
      {
        elog(ERROR, "Can not retrieve document filename for '%s' word.\n", sEncrWord.c_str());
        return false;
      }
      // Compose results from SSE2 search
      m_oSSE2SearchResults->AddResult(sDocumentFilename);
    }
  }

  // Check if there are some document found
  if (m_oSSE2SearchResults->Empty() == true)
  {
    elog(WARNING, "Unable to find any documents for given search query.\n");
    return false;
  }

  PrintSSE2Results();

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}


void SearchEngine::PrintSSE2Results()
{
  elog(DEBUG, "Number of matching documents from SSE2 index: '%d'.\n", m_oSSE2SearchResults->Size());
  for (const std::string& sDocumentFilename : m_oSSE2SearchResults->GetResults())
  {
   elog(DEBUG, "Matching document: '%s'.\n", sDocumentFilename.c_str());
  }
}

bool SearchEngine::ReadKeywordDictionary()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (m_sEncrIndexDirectory.empty() == true)
  {
    elog(ERROR, "Directory with encrypted index is empty.\n");
    return false;
  }

  std::set<std::string> stDictionary;
  if (m_oFileManager->ReadKeywordDirectory(m_sEncrIndexDirectory,
                                             DICTIONARY_FILENAME,
                                             stDictionary) == false)
  {
    elog(ERROR, "Filemanager failed to read keyword dictionary.\n");
    return false;
  }

  if (m_oDictionary->SetDictionary(stDictionary) == false)
  {
    elog(ERROR, "Unable to set the keyword dictionary.\n");
    return false;
  }
  elog(DEBUG, "Read Keyword Dictionary Size: '%zu'.\n", m_oDictionary->GetSize());

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}


bool SearchEngine::ConstructTFSearchQuery(const std::string& p_sWordInput)
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (p_sWordInput.empty() == true)
  {
    elog(ERROR, "Search keyword input is empty.\n");
    return false;
  }

  // Check if document dictionary exist
  if (m_oDictionary->IsEmpty() == true)
  {
    elog(ERROR, "Dictionary is empty.\n");
    return false;
  }

  // Array of words from the input. May contain repeated words.
  std::vector<std::string> vWordQuery;

  // Use same chomp strategy for search input, return filled array of words
  if (ChompWordInput(p_sWordInput, vWordQuery) == false)
  {
    elog(ERROR, "Unable to chomp the input '%s'.\n", p_sWordInput.c_str());
    return false;
  }

  // Make sure that search query is empty, avoid chaos.
  if (m_oTFSearchQuery->IsSearchQueryEmpty() == false)
    m_oTFSearchQuery->ClearSearchQuery();

  // Get the dictionary
  const std::set<std::string>& stDictionary = m_oDictionary->GetDictionary();

  long found = 0;
  for (const std::string& sDictionaryWord : stDictionary)
  {
    // Calculate how many times a word in query repeats
    for (const std::string& sWord : vWordQuery)
    {
      if (sDictionaryWord.compare(sWord) == 0)
        found++; // increase number of times the word was founded
    }

    // Words in query exist in our dictionary?
    if (found > 0)
    {
      // Calculate TF value based on number of times a word repeats in search input
      // and the total number of words in the dictionary
      //long lTFScore = static_cast<long>(m_oDictionary->GetSize()) / found;

      // Fill up the search query with TF value
      m_oTFSearchQuery->SetSearchQuery(found);

      elog(DEBUG, "Search word is '%s', tf value is '%ld'.\n", sDictionaryWord.c_str(), found);

      // Reset findings counter
      found = 0;
    }
    else
    {
      // Set tf value to 0 since the word in query was never found in the dictionary
      m_oTFSearchQuery->SetSearchQuery(0);
    }
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

void SearchEngine::PrintTFSearchQuery()
{
  const std::vector<long>& vSearchQuery = m_oTFSearchQuery->GetSearchQuery();

  for (const long& lValue : vSearchQuery)
  {
    elog(DEBUG, "Search query value is '%ld'.\n", lValue);
  }
}

bool SearchEngine::EncryptTFSearchQuery()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  // Check if plaintext search query is not empty
  if (m_oTFSearchQuery->IsSearchQueryEmpty() == true)
  {
    elog(ERROR, "Search query is empty.\n");
    return false;
  }

  // Check if encrypted search query is not empty
  if (m_oTFSearchQuery->IsEncryptedSearchQueryEmpty() == false)
    m_oTFSearchQuery->ClearEncryptedSearchQuery();

  // Get TF search query
  const std::vector<long>& vTFSearchQuery = m_oTFSearchQuery->GetSearchQuery();

  // Encrypted search query, get values by reference
  std::vector<std::string> vEncrTFSearchQuery;

  // Set encrypted search query values with ciphtertexts
  if (m_oHEManager->EncryptArray(vTFSearchQuery, vEncrTFSearchQuery) == false)
  {
    elog(ERROR, "Array encryption failed.\n");
    return false;
  }

  // Set encrypted query
  if (m_oTFSearchQuery->SetEncryptedSearchQuery(vEncrTFSearchQuery) == false)
  {
    elog(ERROR, "Unable to set encrypted search query.\n");
    return false;
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

bool SearchEngine::MeasureEncryptedSimilarity()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  // Check if SSE2 search results exist
  if (m_oSSE2SearchResults->Empty() == true)
  {
    elog(ERROR, "No SSE2 search results found.\n");
    return false;
  }

  // Check if encrypted search query is not empty
  if (m_oTFSearchQuery->IsEncryptedSearchQueryEmpty() == true)
  {
    elog(ERROR, "Encrypted search query is empty.\n");
    return false;
  }

  // Clear previous results
  if (m_oSimilarityMeasureResults->AreEmpty() == false)
    m_oSimilarityMeasureResults->Clear();

  std::vector<std::string> vSSE2Results;
  if (m_oSSE2SearchResults->GetResultsInVector(vSSE2Results) == false)
  {
    elog(ERROR, "Unable to get SSE2 results.\n");
    return false;
  }

  int iDocumentNumber = m_oSSE2SearchResults->Size();
  int iPoolSize = m_oPool->GetSize();

  int iDocumentOnEachThread = iDocumentNumber / iPoolSize;
  int iReminder = iDocumentNumber % iPoolSize;

  int i = 0;
  while (i < (iDocumentOnEachThread * iPoolSize))
  {
    std::vector<std::string> vBulk(vSSE2Results.begin() + i,
                                   vSSE2Results.begin() + i + iDocumentOnEachThread);

    boost::shared_ptr<SimilarityMeasureTask> pTask =
        boost::make_shared<SimilarityMeasureTask>(m_sEncrIndexDirectory,
                                                  vBulk,
                                                  m_oTFSearchQuery->GetEncryptedSearchQuery(),
                                                  m_oHEManager->GetInitContext(),
                                                  m_oHEManager->GetInitSecretKey(),
                                                  m_oHEManager->GetInitPublicKey(),
                                                  m_oFileManager,
                                                  m_oEventQueue,
                                                  m_oSimilarityMeasureResults);

    Thread *pThrd = m_oPool->CheckOut();
    pThrd->SetTask(pTask);

    i = i + iDocumentOnEachThread;
  }

  if (iReminder > 0)
  {
    int start = iDocumentOnEachThread * iPoolSize;
    int end = start + iReminder;

    for (i = start; i < end; i ++)
    {
      boost::shared_ptr<SimilarityMeasureTask> pTask =
          boost::make_shared<SimilarityMeasureTask>(m_sEncrIndexDirectory,
                                                    vSSE2Results[i],
                                                    m_oTFSearchQuery->GetEncryptedSearchQuery(),
                                                    m_oHEManager->GetInitContext(),
                                                    m_oHEManager->GetInitSecretKey(),
                                                    m_oHEManager->GetInitPublicKey(),
                                                    m_oFileManager,
                                                    m_oEventQueue,
                                                    m_oSimilarityMeasureResults);
      Thread *pThrd = m_oPool->CheckOut();
      pThrd->SetTask(pTask);
    }
  }

  while (m_oPool->GetNumFree() < m_oPool->GetSize())
  {
    sleep(2);
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}


bool SearchEngine::DecryptSimilarityResults()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (m_oSimilarityMeasureResults->AreEmpty() == true)
  {
    elog(ERROR, "Encrypted document similarity results are empty!\n");
    return false;
  }

  const std::vector<boost::shared_ptr<DocumentSimilarity>>& vDocumentSimilarityResults =
      m_oSimilarityMeasureResults->GetDocumentSimilarity();

  int iResultsNumber = m_oSimilarityMeasureResults->Size();
  int iPoolSize = m_oPool->GetSize();

  int iResultsOnEachThread = iResultsNumber / iPoolSize;
  int iReminder = iResultsNumber % iPoolSize;

  int i = 0;
  while (i < (iResultsOnEachThread * iPoolSize))
  {
    std::vector<boost::shared_ptr<DocumentSimilarity>> vBulk(vDocumentSimilarityResults.begin() + i,
                                                             vDocumentSimilarityResults.begin() + i + iResultsOnEachThread);

    boost::shared_ptr<DecryptSimilarityMeasureTask> pTask =
        boost::make_shared<DecryptSimilarityMeasureTask>(vBulk,
                                                         m_sEncrIndexDirectory,
                                                         m_oFileManager,
                                                         m_oHEManager->GetInitContext(),
                                                         m_oHEManager->GetInitSecretKey(),
                                                         m_oHEManager->GetInitPublicKey());

    Thread *pThrd = m_oPool->CheckOut();
    pThrd->SetTask(pTask);

    i = i + iResultsOnEachThread;
  }

  if (iReminder > 0)
  {
    int start = iResultsOnEachThread * iPoolSize;
    int end = start + iReminder;

    for (i = start; i < end; i++)
    {
      boost::shared_ptr<DecryptSimilarityMeasureTask> pTask =
          boost::make_shared<DecryptSimilarityMeasureTask>(vDocumentSimilarityResults[i],
                                                           m_sEncrIndexDirectory,
                                                           m_oFileManager,
                                                           m_oHEManager->GetInitContext(),
                                                           m_oHEManager->GetInitSecretKey(),
                                                           m_oHEManager->GetInitPublicKey());

      Thread *pThrd = m_oPool->CheckOut();
      pThrd->SetTask(pTask);
    }
  }

  while (m_oPool->GetNumFree() < m_oPool->GetSize())
  {
    sleep(2);
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

void SearchEngine::PrintResults()
{
  if (m_oSimilarityMeasureResults->AreEmpty() == true)
  {
    elog(DEBUG, "No similarity results found.\n");
    return;
  }

  const std::vector<boost::shared_ptr<DocumentSimilarity>>& vResults = m_oSimilarityMeasureResults->GetDocumentSimilarity();

/*
  // TODO: Sort results
  std::sort(vResults.begin(), vResults.end(), [](const boost::shared_ptr<DocumentSimilarity>& lhs, const boost::shared_ptr<DocumentSimilarity>& rhs )
  {
     return (lhs->GetSimilarityCoefficient(), rhs->GetSimilarityCoefficient());
  });
*/
  // Output results
  for (boost::shared_ptr<DocumentSimilarity> oDocumentSimilarity : vResults)
  {
    elog(DEBUG, "Document '%s' has following similarity coefficient '%ld'.\n",
         oDocumentSimilarity->GetFileName().c_str(),
         oDocumentSimilarity->GetSimilarityCoefficient());
  }
}

double SearchEngine::CalculatePlaintextSimilarity(const std::vector<double>& search_query,
                                                  const std::vector<double>& tf_index)
{
  double result = 0;
  double qry_val = 0;
  double ind_val = 0;

  for (size_t i = 0; i < tf_index.size(); i++)
  {
    qry_val = search_query.at(i);
    ind_val = tf_index.at(i);
    result = result + (qry_val * ind_val);
  }

  return result;
}


/*
bool SearchEngine::MeasurePlaintextDocumentSimilarity()
{
  // Check if search query is not empty
  if (search_query_.size() == 0)
  {
    elog(ERROR, "Search query vector is empty\n");
    return false;
  }

  // Make sure results are empty
  if (results_.empty() == false)
    results_.clear();

  std::vector<double> dQuery = m_oTFSearchQuery->GetSearchQuery();

  std::vector<double> tf_index;

  for (const DocumentIndex& di : document_indexes_)
  {
     tf_index.clear();

     // tf_index is updated by reference
     if (di.GetTfValues(tf_index) == false)
     {
       elog(ERROR, "Unable to execute document similarity measurement!\n");
       return false;
     }

    double similarity_coefficient = CalculatePlaintextSimilarity(search_query_, tf_index);

    elog(DEBUG, "Document '%s' has similarity score: %f.\n");
  }

  return true;
}
*/

bool SearchEngine::ChompWordInput(const std::string& p_sContent,
                                  std::vector<std::string>& p_vWordArrayContent)
{
  if (p_sContent.empty() == true)
  {
    elog(ERROR, "Document content is empty\n");
    return false;
  }

  std::string sChompContent = p_sContent;

  // Make content use lowercase
  boost::to_lower(sChompContent);

  // Remove delimiters
  std::string alldelims = character_literals + keyboard_delimiters;
  boost::split(p_vWordArrayContent, sChompContent, boost::is_any_of(alldelims), boost::token_compress_on);

  // Remove english letter from vector of unique words
  for (const auto& letter : english_alphabet)
    p_vWordArrayContent.erase(std::remove(p_vWordArrayContent.begin(), p_vWordArrayContent.end(), letter), p_vWordArrayContent.end());

  // Remove most commonly used words from the vector of unique words
  for (const auto& word : english_common_words)
    p_vWordArrayContent.erase(std::remove(p_vWordArrayContent.begin(), p_vWordArrayContent.end(), word), p_vWordArrayContent.end());

  // Find words to remove
  std::vector<std::string> remove_words;
  for (const std::string& list_word : p_vWordArrayContent)
  {
    if ((list_word.length() < CHOMP_WORD_MIN_LENGTH) || (list_word.length() > CHOMP_WORD_MAX_LENGTH))
      remove_words.emplace_back(list_word);
  }

  // Remove found words
  for (const std::string& word : remove_words)
    p_vWordArrayContent.erase(std::remove(p_vWordArrayContent.begin(), p_vWordArrayContent.end(), word), p_vWordArrayContent.end());

  return true;
}



