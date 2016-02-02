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
 *  File: EncryptTFIndexTask.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 3, 2015
 *
 */

#ifndef _ENCRYPT_TF_INDEX_TASK_H
#define _ENCRYPT_IF_INDEX_TASK_H

// System

// Shared

// Module
#include <boost/shared_ptr.hpp>
#include "Task.h"
#include "DocumentIndex.h"
#include "HEManager.h"
#include "FileManager.h"
#include "TFIndexTask.h"

class EncryptTFIndexTask : public Task
{
public:
		EncryptTFIndexTask(const TFIndexTask& p_oTFIndexTask,
						   const std::string& p_sHEManagerContext,
						   const std::string& p_sHEManagerSeckey,
						   const std::string& p_sHEManagerPubkey,
						   boost::shared_ptr<FileManager> p_oFileManager);

		EncryptTFIndexTask(const std::vector<TFIndexTask>& p_vTFIndexTasks,
						   const std::string& p_sHEManagerContext,
						   const std::string& p_sHEManagerSeckey,
						   const std::string& p_sHEManagerPubkey,
						   boost::shared_ptr<FileManager> p_oFileManager);


		virtual ~EncryptTFIndexTask();

		virtual bool Execute();
		virtual bool Done();
		virtual bool Process();


private:
		void InitHEManager();
		bool EncryptBulk(const std::vector<TFIndexTask>& p_vTFIndexTasks);
		bool Encrypt(const TFIndexTask& p_oTask);

		bool m_bDone;

		TFIndexTask m_oTFIndexTask;
		std::vector<TFIndexTask> m_vTFIndexTasks;

		std::string m_sHEManagerContext;
		std::string m_sHEManagerSeckey;
		std::string m_sHEManagerPubkey;
		boost::shared_ptr<FileManager> m_oFileManager;
		HEManager m_oHEManager;
};

#endif
