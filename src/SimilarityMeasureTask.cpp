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
 *  File: SimilarityMeasureTask.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: July 1, 2015
 *
 */

// System
#include <vector>

// Shared
#include "site_defs.h"

// Module
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "SimilarityMeasureTask.h"
#include "Log.h"
#include "SimilarityMeasureEvent.h"

SimilarityMeasureTask::SimilarityMeasureTask(const std::string& p_sEncrIndexDirectory,
                                             const std::string& p_sSSE2SearchResult,
                                             const std::vector<std::string>& p_vEncrSearchQuery,
                                             const string &p_sHEManagerContext,
                                             const string &p_sHEManagerSeckey,
                                             const string &p_sHEManagerPubkey,
                                             boost::shared_ptr<FileManager> p_oFileManager,
                                             boost::shared_ptr<EventQueue> p_oEventQueue,
                                             boost::shared_ptr<SimilarityMeasureResults> p_oSimilarityMeasureResults) :
  m_bDone(false),
  m_sEncrIndexDirectory(p_sEncrIndexDirectory),
  m_sSSE2SearchResult(p_sSSE2SearchResult),
  m_vEncrTFSearchQuery(p_vEncrSearchQuery),
  m_sHEManagerContext(p_sHEManagerContext),
  m_sHEManagerSeckey(p_sHEManagerSeckey),
  m_sHEManagerPubkey(p_sHEManagerPubkey),
  m_oFileManager(p_oFileManager),
  m_oEventQueue(p_oEventQueue),
  m_oSimilarityMeasureResults(p_oSimilarityMeasureResults)
{

}

SimilarityMeasureTask::SimilarityMeasureTask(const std::string& p_sEncrIndexDirectory,
                                             const std::vector<std::string>& p_vSSE2SearchResultBulk,
                                             const std::vector<std::string>& p_vEncrSearchQuery,
                                             const string &p_sHEManagerContext,
                                             const string &p_sHEManagerSeckey,
                                             const string &p_sHEManagerPubkey,
                                             boost::shared_ptr<FileManager> p_oFileManager,
                                             boost::shared_ptr<EventQueue> p_oEventQueue,
                                             boost::shared_ptr<SimilarityMeasureResults> p_oSimilarityMeasureResults) :
  m_bDone(false),
  m_sEncrIndexDirectory(p_sEncrIndexDirectory),
  m_vSSE2SearchResultBulk(p_vSSE2SearchResultBulk),
  m_vEncrTFSearchQuery(p_vEncrSearchQuery),
  m_sHEManagerContext(p_sHEManagerContext),
  m_sHEManagerSeckey(p_sHEManagerSeckey),
  m_sHEManagerPubkey(p_sHEManagerPubkey),
  m_oFileManager(p_oFileManager),
  m_oEventQueue(p_oEventQueue),
  m_oSimilarityMeasureResults(p_oSimilarityMeasureResults)
{

}


SimilarityMeasureTask::~SimilarityMeasureTask()
{

}

bool SimilarityMeasureTask::Execute()
{
  InitHEManager();

  if (m_sSSE2SearchResult.size() != 0)
  {
    if (CalculateSimilarity(m_sSSE2SearchResult) == false)
    {
      elog(ERROR, "Unable to calculate similarity score.\n");
    }
  }
  else if (m_vSSE2SearchResultBulk.size() != 0)
  {
    if (CalculateSimilarity(m_vSSE2SearchResultBulk) == false)
    {
      elog(ERROR, "Unable to calculate similarity in bulk.\n");
    }
  }
  else
  {
    elog(CRITICAL, "Unable to run similarity measure task!\n");
  }

  m_bDone = true;

  return true;
}

bool SimilarityMeasureTask::Done()
{
  return m_bDone;
}

bool SimilarityMeasureTask::Process()
{
  bool bRet = false;
  if (Done())
  {
    bRet = true;
  }

  return bRet;
}

void SimilarityMeasureTask::InitHEManager()
{
  // Init first HE manager and generate random keys (we will update them)
  m_oHEManager.Init();

  // Update context and keys so we can decrypt data later
  m_oHEManager.SetInitContext(m_sHEManagerContext);
  m_oHEManager.SetInitSecretKey(m_sHEManagerSeckey);
  m_oHEManager.SetInitPublicKey(m_sHEManagerPubkey);
}

bool SimilarityMeasureTask::CalculateSimilarity(const std::vector<std::string>& p_vSSE2SearchResultBulk)
{
  for (const std::string& sSSE2SearchResult : p_vSSE2SearchResultBulk)
  {
    if (CalculateSimilarity(sSSE2SearchResult) == false)
    {
      elog(ERROR, "Unable to calculate similarity measure for '%s'.\n", sSSE2SearchResult.c_str());
    }
  }

  return true;
}

bool SimilarityMeasureTask::CalculateSimilarity(const std::string& p_sSSE2SearchResult)
{
  bool bRet = false;

  if (m_vEncrTFSearchQuery.empty() == true)
  {
    elog(ERROR, "Encrypted TF Search Query is empty.\n");
  }

  // Read Encrypted TF values for given document filename
  std::vector<std::string> sEncrTFValues;
  if (m_oFileManager->ReadEncryptedTFValues(m_sEncrIndexDirectory,
                                            p_sSSE2SearchResult,
                                            sEncrTFValues) == false)
  {
    elog(ERROR, "Unable to read encrypted TF values from document '%s'.\n", p_sSSE2SearchResult.c_str());
  }

  // Calculate similarity scores
  std::vector<std::string> sEncrSimilarityScores;
  if (m_oHEManager.CalculateSimilarity(sEncrTFValues,
                                       m_vEncrTFSearchQuery,
                                       sEncrSimilarityScores) == false)
  {
    elog(ERROR, "Similarity calculation failed.\n");
  }

  // Create event and enqueue it
  boost::shared_ptr<SimilarityMeasureEvent> pEvent =
      boost::make_shared<SimilarityMeasureEvent>(p_sSSE2SearchResult,
                                                 sEncrSimilarityScores,
                                                 m_oSimilarityMeasureResults,
                                                 m_sEncrIndexDirectory,
                                                 m_oFileManager);

  if (m_oEventQueue->Enqueue(pEvent) == false)
  {
    elog(CRITICAL, "Unable to enqueue similarity results for document '%s'.\n", p_sSSE2SearchResult.c_str());
  }
  else
  {
    m_bDone = true;
    bRet = true;
  }

  return bRet;
}








