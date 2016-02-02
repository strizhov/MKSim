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
 *  File: DecryptSimilarityMeasureTask.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: July 10, 2015
 *
 */

#ifndef _DECRYPT_SIMILARITY_MEASURE_TASK_H
#define _DECRYPT_SIMILARITY_MEASURE_TASK_H

// System
#include <vector>
#include <string>

// Shared

// Module
#include <boost/shared_ptr.hpp>
#include "DocumentSimilarity.h"
#include "HEManager.h"
#include "Task.h"
#include "FileManager.h"

class DecryptSimilarityMeasureTask : public Task
{
public:
        DecryptSimilarityMeasureTask(boost::shared_ptr<DocumentSimilarity> p_oDocumentSimilarity,
                                     std::string p_sSimilarityScoresDirectory,
                                     boost::shared_ptr<FileManager> p_oFileManager,
                                     const std::string& p_sHEManagerContext,
                                     const std::string& p_sHEManagerSeckey,
                                     const std::string& p_sHEManagerPubkey);

        DecryptSimilarityMeasureTask(std::vector<boost::shared_ptr<DocumentSimilarity>> p_vDocumentSimilarity,
                                     std::string p_sSimilarityScoresDirectory,
                                     boost::shared_ptr<FileManager> p_oFileManager,
                                     const std::string& p_sHEManagerContext,
                                     const std::string& p_sHEManagerSeckey,
                                     const std::string& p_sHEManagerPubkey);
        virtual ~DecryptSimilarityMeasureTask();

        virtual bool Execute();
        virtual bool Done();
        virtual bool Process();

private:
        void InitHEManager();
        bool Decrypt(boost::shared_ptr<DocumentSimilarity> p_oDocumentSimilarity);
        bool Decrypt(std::vector<boost::shared_ptr<DocumentSimilarity>> p_vDocumentSimilarity);

        bool m_bDone;
        boost::shared_ptr<DocumentSimilarity> m_oDocumentSimilarity;
        std::vector<boost::shared_ptr<DocumentSimilarity>> m_vDocumentSimilarity;

        std::string m_sSimilarityScoresDirectory;
        boost::shared_ptr<FileManager> m_oFileManager;

		std::string m_sHEManagerContext;
		std::string m_sHEManagerSeckey;
		std::string m_sHEManagerPubkey;
		HEManager m_oHEManager;
};

#endif
