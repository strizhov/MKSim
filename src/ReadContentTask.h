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
 *  File: ReadContentTask.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 3, 2015
 *
 */

#ifndef _READ_CONTENT_TASK_H
#define _READ_CONTENT_TASK_H

// System
#include <string>
#include <vector>

// Shared

// Module
#include <boost/shared_ptr.hpp>
#include <boost/filesystem.hpp>
#include "Task.h"
#include "FileManager.h"
#include "EventQueue.h"
#include "TFIndex.h"
#include "Dictionary.h"

class ReadContentTask : public Task
{
public:
		ReadContentTask(const boost::filesystem::path& p_sDocumentFilename,
						boost::shared_ptr<EventQueue> p_oEventQueue,
						boost::shared_ptr<FileManager> p_oFileManager,
						boost::shared_ptr<TFIndex> p_oTFIndex,
						boost::shared_ptr<Dictionary> p_oDictionary);

		ReadContentTask(const std::vector<boost::filesystem::path>& p_vDocumentFilenames,
						boost::shared_ptr<EventQueue> p_oEventQueue,
						boost::shared_ptr<FileManager> p_oFileManager,
						boost::shared_ptr<TFIndex> p_oTFIndex,
						boost::shared_ptr<Dictionary> p_oDictionary);

		virtual ~ReadContentTask();

		virtual bool Execute();
		virtual bool Done();
		virtual bool Process();

private:
		bool ReadBulkFileContent(const std::vector<boost::filesystem::path>& p_vFilenames);
		bool ReadFileContent(const boost::filesystem::path& p_sFilename);
		bool ChompDocumentContent(std::string& content,
								  std::vector<std::string>& content_word_list);
		boost::shared_ptr<EventQueue> GetEventQueue();

		bool m_bDone;
		boost::filesystem::path m_sDocumentFilename; // For Single Document
		std::vector<boost::filesystem::path> m_vDocumentFilenames; // For Bulk of Documents
		boost::shared_ptr<EventQueue>  m_oEventQueue;
		boost::shared_ptr<FileManager> m_oFileManager;
		boost::shared_ptr<TFIndex> m_oTFIndex;
		boost::shared_ptr<Dictionary> m_oDictionary;
};

#endif
