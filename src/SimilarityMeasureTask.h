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
 *  File: SimilarityMeasureTask.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: July 1, 2015
 *
 */

#ifndef _SIMILARITY_MEASURE_TASK_H
#define _SIMILARITY_MEASURE_TASK_H

// System
#include <set>
#include <string>

// Shared

// Module
#include <boost/shared_ptr.hpp>
#include "Task.h"
#include "HEManager.h"
#include "FileManager.h"
#include "EventQueue.h"
#include "SimilarityMeasureResults.h"

class SimilarityMeasureTask : public Task
{
public:
        SimilarityMeasureTask(const std::string& p_sEncrIndexDirectory,
                              const std::string& p_sSSE2SearchResult,
                              const std::vector<std::string>& p_vEncrSearchQuery,
                              const std::string& p_sHEManagerContext,
                              const std::string& p_sHEManagerSeckey,
                              const std::string& p_sHEManagerPubkey,
                              boost::shared_ptr<FileManager> p_oFileManager,
                              boost::shared_ptr<EventQueue> p_oEventQueue,
                              boost::shared_ptr<SimilarityMeasureResults> p_oSimilarityMeasureResults);

        SimilarityMeasureTask(const std::string& p_sEncrIndexDirectory,
                              const std::vector<std::string>& p_vSSE2SearchResultBulk,
                              const std::vector<std::string>& p_vEncrSearchQuery,
                              const std::string& p_sHEManagerContext,
                              const std::string& p_sHEManagerSeckey,
                              const std::string& p_sHEManagerPubkey,
                              boost::shared_ptr<FileManager> p_oFileManager,
                              boost::shared_ptr<EventQueue> p_oEventQueue,
                              boost::shared_ptr<SimilarityMeasureResults> p_oSimilarityMeasureResults);

        virtual ~SimilarityMeasureTask();

        virtual bool Execute();
        virtual bool Done();
        virtual bool Process();
private:
        void InitHEManager();
        bool CalculateSimilarity(const std::string& p_sSSE2SearchResult);
        bool CalculateSimilarity(const std::vector<std::string>& p_vSSE2SearchResultBulk);

        bool m_bDone;
        std::string m_sEncrIndexDirectory;
        std::string m_sSSE2SearchResult;
        std::vector<std::string> m_vSSE2SearchResultBulk;
        std::vector<std::string> m_vEncrTFSearchQuery;


        std::string m_sHEManagerContext;
        std::string m_sHEManagerSeckey;
        std::string m_sHEManagerPubkey;
        HEManager m_oHEManager;

        boost::shared_ptr<FileManager> m_oFileManager;
        boost::shared_ptr<EventQueue> m_oEventQueue;
        boost::shared_ptr<SimilarityMeasureResults> m_oSimilarityMeasureResults;
};

#endif
