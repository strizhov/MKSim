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
 *  File: IndexBuildEngine.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: May 27, 2015
 *
 */

#ifndef _INDEX_BUILD_ENGINE_H
#define _INDEX_BUILD_ENGINE_H


// System
#include <string>
#include <vector>
#include <unordered_set>

// Shared
#include "site_defs.h"

// Module
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include "FileManager.h"
#include "LTCManager.h"
#include "HEManager.h"
#include "EventQueue.h"
#include "Dictionary.h"
#include "DocumentIndex.h"
#include "TFIndex.h"
#include "SSE2Index.h"
#include "StatsTracker.h"
#include "ThreadPool.h"
#include "Log.h"

class IndexBuildEngine : boost::noncopyable
{
public:
		IndexBuildEngine(const std::string& p_sPathToDocumentFiles,
						 const std::string& p_sOutputDirectory,
						 boost::shared_ptr<FileManager> p_oFileManager,
						 boost::shared_ptr<LTCManager> p_oLTCManager,
						 boost::shared_ptr<HEManager> p_oHEManager,
						 boost::shared_ptr<ThreadPool> p_oPool,
						 boost::shared_ptr<EventQueue> p_oEventQueue);
		virtual ~IndexBuildEngine();

		virtual bool Init();
		void Startup();
		void Shutdown();
		//bool GenRandomSearchQuery(const uint& num_words, std::string& search_query);

private:
		//bool CreateDocumentIndexes();
		//bool MakeWordDictionary();
		bool ReadDocumentCollectionWithPool(const std::vector<boost::filesystem::path>& p_vDocumentFilenames);
		bool SaveDictionary();
		bool CalcTermFrequency();
		bool EncryptTermFrequencyWithPool();

		void PrintTermFrequencyFromDocumentIndex();
		bool EncryptTermFrequency();

		bool MakeSSE2Index();
		bool SaveSSE2Index();
		void PrintSSE2Index();

		const std::string m_sPathToDocumentFiles;
		const std::string m_sOutputDirectory;

		boost::shared_ptr<FileManager> m_oFileManager;
		boost::shared_ptr<LTCManager> m_oLTCManager;
		boost::shared_ptr<HEManager> m_oHEManager;
		boost::shared_ptr<ThreadPool> m_oPool;
		boost::shared_ptr<EventQueue> m_oEventQueue;

		boost::shared_ptr<Dictionary> m_oDictionary;
		boost::shared_ptr<TFIndex> m_oTFIndex;
		boost::shared_ptr<SSE2Index> m_oSSE2Index;

		bool shutdown_;
};


#endif //
