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
 *  File: SimilarityMeasureEvent.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: July 8, 2015
 *
 */

#ifndef _SIMILARITY_MEASURE_EVENT_H
#define _SIMILARITY_MEASURE_EVENT_H

// System
#include <string>
#include <vector>

// Shared

// Module
#include <boost/shared_ptr.hpp>
#include "Event.h"
#include "SimilarityMeasureResults.h"
#include "FileManager.h"

class SimilarityMeasureEvent : public Event
{
public:
        SimilarityMeasureEvent(const std::string& p_sSSE2DocumentFilename,
                               const std::vector<std::string>& p_vEncrSimilarityScores,
                               boost::shared_ptr<SimilarityMeasureResults> p_oSimilarityMeasureResults,
                               std::string p_sSimilarityScoresDirectory,
                               boost::shared_ptr<FileManager> p_oFileManager);
        virtual ~SimilarityMeasureEvent();

        virtual bool Execute();

        std::string GetDocumentFilename() const ;
        std::vector<std::string> GetEncrSimilarityScores() const;
        boost::shared_ptr<SimilarityMeasureResults> GetSimilarityMeasureResults();
        std::string GetSimilarityScoresDirectory() const;
        boost::shared_ptr<FileManager> GetFileManager();

private:
        std::string m_sSSE2DocumentFilename;
        std::vector<std::string> m_vEncrSimilarityScores;
        boost::shared_ptr<SimilarityMeasureResults> m_oSimilarityMeasureResults;
        std::string m_sSimilarityScoresDirectory;
        boost::shared_ptr<FileManager> m_oFileManager;
};

#endif

