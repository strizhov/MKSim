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
 *  File: SearchEngine.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: May 27, 2015
 *
 */

#ifndef _SEARCH_ENGINE_H
#define _SEARCH_ENGINE_H

// System
#include <string>
#include <vector>
#include <unordered_set>

// Shared

// Module
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include "DocumentIndex.h"
#include "DocumentSimilarity.h"
#include "FileManager.h"
#include "SearchQuery.h"
#include "Dictionary.h"
#include "SSE2Index.h"
#include "SSE2SearchQuery.h"
#include "LTCManager.h"
#include "HEManager.h"
#include "SSE2SearchResults.h"
#include "SimilarityMeasureResults.h"
#include "ThreadPool.h"
#include "EventHandler.h"
#include "EventQueue.h"

class SearchEngine : boost::noncopyable
{
public:
		SearchEngine(const std::string& p_sEncrIndexDirectory,
					 boost::shared_ptr<FileManager> p_oFileManager,
					 boost::shared_ptr<LTCManager> p_oLTCManager,
					 boost::shared_ptr<HEManager> p_oHEManager,
					 boost::shared_ptr<ThreadPool> p_oPool,
					 boost::shared_ptr<EventQueue> p_oEventQueue);
		virtual ~SearchEngine();

		virtual bool Init();
		bool SearchIndex(const std::string& p_sInput);
		void PrintResults();
                bool GenRandomSearchQuery(int p_iRandInt, std::string& p_sWords);

private:

		bool ChompWordInput(const std::string& p_sContent,
							std::vector<std::string>& p_vWordArrayContent);
		bool ReadSSE2Index();
		bool ConstructSSE2SearchQuery(const std::string& p_sWordInput);

		bool EncryptSSE2SearchQuery();
		bool SearchSSE2Index();
		bool ReadKeywordDictionary();

		bool ConstructTFSearchQuery(const std::string& p_sWordInput);
		bool EncryptTFSearchQuery();
		void PrintTFSearchQuery();

		bool MeasureEncryptedSimilarity();
		bool MeasurePlaintextDocumentSimilarity();
		double CalculatePlaintextSimilarity(const std::vector<double>& search_query,
											const std::vector<double>& tf_index);
		bool DecryptSimilarityResults();
		void PrintSSE2Results();

		bool shutdown_;

		std::string m_sEncrIndexDirectory;
		boost::shared_ptr<FileManager> m_oFileManager;
		boost::shared_ptr<LTCManager> m_oLTCManager;
		boost::shared_ptr<HEManager> m_oHEManager;
		boost::shared_ptr<ThreadPool> m_oPool;
		boost::shared_ptr<EventQueue> m_oEventQueue;

		boost::shared_ptr<SSE2Index> m_oSSE2Index;
		boost::shared_ptr<SSE2SearchQuery> m_oSSE2SearchQuery;
		boost::shared_ptr<SSE2SearchResults> m_oSSE2SearchResults;
		boost::shared_ptr<Dictionary> m_oDictionary;
		boost::shared_ptr<SearchQuery> m_oTFSearchQuery;
		boost::shared_ptr<SimilarityMeasureResults> m_oSimilarityMeasureResults;



};

#endif //
