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
 *  File: EventHandler.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 10, 2015
 *
 */

// System

// Shared
#include "site_defs.h"

// Module
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include "EventHandler.h"
#include "ReadContentEvent.h"
#include "SimilarityMeasureEvent.h"
#include "SimilarityMeasureResults.h"
#include "DocumentSimilarity.h"
#include "Log.h"
#include "FileManager.h"
#include "Dictionary.h"
#include "DocumentIndex.h"
#include "TFIndex.h"

EventHandler::EventHandler()
{
}

EventHandler::~EventHandler()
{
}

bool EventHandler::HandleEvent(boost::shared_ptr<Event> p_oEvent)
{
  bool m_bRet = false;

  task_event_type_e eTaskType = p_oEvent->GetTaskType();

  switch(eTaskType)
  {
    case READ_CONTENT_EVENT:
      HandleReadContentEvent(p_oEvent);
      m_bRet = true;
      break;
    case SIMILARITY_MEASURE_EVENT:
      HandleSimilarityMeasureEvent(p_oEvent);
      m_bRet = true;
      break;
    default:
      elog(ERROR, "Unknown event type!\n");
      break;
  }

  return m_bRet;
}

bool EventHandler::HandleReadContentEvent(boost::shared_ptr<Event> p_oEvent)
{
  boost::shared_ptr<ReadContentEvent> pReadContentEvent = boost::static_pointer_cast<ReadContentEvent>(p_oEvent);

  elog(DEBUG, "Handling '%d' words from '%s'.\n",
       pReadContentEvent->GetContentWordList().size(),
       pReadContentEvent->GetDocumentFilename().string().c_str());

  // Get filename and list of words for a document
  boost::filesystem::path oDocumentFilename = pReadContentEvent->GetDocumentFilename();
  const std::vector<std::string>& vDocumentWordList = pReadContentEvent->GetContentWordList();

  // Add to the dictionary
  if (pReadContentEvent->GetDictionary()->AddWords(vDocumentWordList) == false)
  {
    elog(CRITICAL, "Unable to add words from document '%s'.\n", oDocumentFilename.string().c_str());
    return false;
  }

  // Create document index
  boost::shared_ptr<DocumentIndex> pDocumentIndex = boost::make_shared<DocumentIndex>(oDocumentFilename);

  // Add word list to the document index
  pDocumentIndex->AddWords(vDocumentWordList);

  // Store index pointer in vector
  pReadContentEvent->GetTFIndex()->AddTFIndex(pDocumentIndex);

  return true;
}

bool EventHandler::HandleSimilarityMeasureEvent(boost::shared_ptr<Event> p_oEvent)
{
  boost::shared_ptr<SimilarityMeasureEvent> pSimilarityMeasureEvent =
      boost::static_pointer_cast<SimilarityMeasureEvent>(p_oEvent);

  std::string sFilename = pSimilarityMeasureEvent->GetDocumentFilename();
  std::string sOutputDirectory = pSimilarityMeasureEvent->GetSimilarityScoresDirectory();
  std::vector<std::string> sEncrSimilarityScores = pSimilarityMeasureEvent->GetEncrSimilarityScores();

  elog(DEBUG, "Similarity measurement for document '%s'.\n", sFilename.c_str());

  // Create new document similarity
  boost::shared_ptr<DocumentSimilarity> pDocumentSimilarity = boost::make_shared<DocumentSimilarity>(sFilename);

  boost::shared_ptr<FileManager> oFileManager = pSimilarityMeasureEvent->GetFileManager();

  // Read the content of provided file
  if (oFileManager->WriteEncryptedSimilarityScores(sOutputDirectory,
                                                   sFilename,
                                                   sEncrSimilarityScores) == false)
  {
    elog(ERROR, "Unable to write encrypted similarity scores of '%s' file.\n", sFilename.c_str());
  }

  // Push back document name to the similarity results
  pSimilarityMeasureEvent->GetSimilarityMeasureResults()->AddResult(pDocumentSimilarity);


  return true;
}



