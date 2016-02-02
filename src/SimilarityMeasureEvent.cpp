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
 *  File: SimilarityMeasureEvent.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: July 8, 2015
 *
 */

// System

// Shared
#include "site_defs.h"

// Module
#include "SimilarityMeasureEvent.h"
#include "SimilarityMeasureResults.h"

SimilarityMeasureEvent::SimilarityMeasureEvent(const std::string& p_sSSE2DocumentFilename,
                                               const std::vector<std::string>& p_vEncrSimilarityScores,
                                               boost::shared_ptr<SimilarityMeasureResults> p_oSimilarityMeasureResults,
                                               std::string p_sSimilarityScoresDirectory,
                                               boost::shared_ptr<FileManager> p_oFileManager) :
  Event(SIMILARITY_MEASURE_EVENT),
  m_sSSE2DocumentFilename(p_sSSE2DocumentFilename),
  m_vEncrSimilarityScores(p_vEncrSimilarityScores),
  m_oSimilarityMeasureResults(p_oSimilarityMeasureResults),
  m_sSimilarityScoresDirectory(p_sSimilarityScoresDirectory),
  m_oFileManager(p_oFileManager)
{

}

SimilarityMeasureEvent::~SimilarityMeasureEvent()
{

}

bool SimilarityMeasureEvent::Execute()
{
  return false;
}

std::string SimilarityMeasureEvent::GetDocumentFilename() const
{
  return m_sSSE2DocumentFilename;
}

std::vector<std::string> SimilarityMeasureEvent::GetEncrSimilarityScores() const
{
  return m_vEncrSimilarityScores;
}

boost::shared_ptr<SimilarityMeasureResults> SimilarityMeasureEvent::GetSimilarityMeasureResults()
{
  return m_oSimilarityMeasureResults;
}

boost::shared_ptr<FileManager> SimilarityMeasureEvent::GetFileManager()
{
  return m_oFileManager;
}

std::string SimilarityMeasureEvent::GetSimilarityScoresDirectory() const
{
  return m_sSimilarityScoresDirectory;
}
