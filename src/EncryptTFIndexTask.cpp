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
 *  File: EncryptTFIndexTask.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 3, 2015
 *
 */

// System
#include <vector>
#include <string>

// Shared
#include "site_defs.h"

// Module
#include "EncryptTFIndexTask.h"
#include "Log.h"

//#define DEBUG_LOG

EncryptTFIndexTask::EncryptTFIndexTask(const TFIndexTask& p_oTFIndexTask,
									   const std::string& p_sHEManagerContext,
									   const std::string& p_sHEManagerSeckey,
									   const std::string& p_sHEManagerPubkey,
									   boost::shared_ptr<FileManager> p_oFileManager) :
  m_bDone(false),
  m_oTFIndexTask(p_oTFIndexTask),
  m_sHEManagerContext(p_sHEManagerContext),
  m_sHEManagerSeckey(p_sHEManagerSeckey),
  m_sHEManagerPubkey(p_sHEManagerPubkey),
  m_oFileManager(p_oFileManager)
{

}

EncryptTFIndexTask::EncryptTFIndexTask(const std::vector<TFIndexTask>& p_vTFIndexTasks,
									   const std::string& p_sHEManagerContext,
									   const std::string& p_sHEManagerSeckey,
									   const std::string& p_sHEManagerPubkey,
									   boost::shared_ptr<FileManager> p_oFileManager) :
  m_bDone(false),
  m_vTFIndexTasks(p_vTFIndexTasks),
  m_sHEManagerContext(p_sHEManagerContext),
  m_sHEManagerSeckey(p_sHEManagerSeckey),
  m_sHEManagerPubkey(p_sHEManagerPubkey),
  m_oFileManager(p_oFileManager)
{

}

EncryptTFIndexTask::~EncryptTFIndexTask()
{

}

bool EncryptTFIndexTask::Execute()
{
  // Initialize HEManager
  InitHEManager();

  // Bulk encryption
  if (m_vTFIndexTasks.size() != 0)
  {
    if (EncryptBulk(m_vTFIndexTasks) == false)
    {
      elog(ERROR, "Unable to execute bulk encryption task.\n");
    }
  }
  // Check if TF array is not empty
  else if (m_oTFIndexTask.GetTFValues().size() != 0)
  {
    if (Encrypt(m_oTFIndexTask) == false)
    {
      elog(CRITICAL, "Unable to execute task with document '%s'.\n",
           m_oTFIndexTask.GetFilename().c_str());
    }
  }
  else
  {
    elog(CRITICAL, "Unable to run task! Constructor is created incorrectly.\n");
  }


  m_bDone = true;

  return true;
}

bool EncryptTFIndexTask::Done()
{
  return m_bDone;
}

bool EncryptTFIndexTask::Process()
{
  bool bRet = false;
  if (Done())
  {
	bRet = true;
  }

  return bRet;
}

void EncryptTFIndexTask::InitHEManager()
{
#ifdef DEBUG_LOG
  elog(DEBUG, "HEManager initialization begins for document '%s'.\n", m_oTFIndexTask.GetFilename().c_str());
#endif

  // Init first HE manager and generate random keys (we will update them)
  m_oHEManager.Init();

  // Update context and keys so we can decrypt data later
  m_oHEManager.SetInitContext(m_sHEManagerContext);
  m_oHEManager.SetInitSecretKey(m_sHEManagerSeckey);
  m_oHEManager.SetInitPublicKey(m_sHEManagerPubkey);

#ifdef DEBUG_LOG
  elog(DEBUG, "HEManager initialization ends for document '%s'.\n", m_oTFIndexTask.GetFilename().c_str());
#endif
}

bool EncryptTFIndexTask::EncryptBulk(const std::vector<TFIndexTask>& p_vTFIndexTasks)
{
  for (const TFIndexTask& oTask : p_vTFIndexTasks)
  {
    if (Encrypt(oTask) == false)
    {
      elog(ERROR, "Unable to encrypt TF values of '%s' document.\n", oTask.GetFilename().c_str());
    }
  }

  return true;
}

bool EncryptTFIndexTask::Encrypt(const TFIndexTask& p_oTask)
{
  bool bRet = false;

  // Check if values exist
  if (p_oTask.GetTFValues().empty() == true)
  {
	elog(ERROR, "Unable to get term frequency values from document '%s'.\n",
	   p_oTask.GetFilename().c_str());
  }
  else
  {
	// Vector for encrypted weights
	std::vector<std::string> encr_term_weights;

	// Encrypt! Store ciphtertext in document index object
	if (m_oHEManager.EncryptArray(p_oTask.GetTFValues(), encr_term_weights) == false)
	{
	  elog(ERROR, "Array encryption failed!\n");
	}
	else
	{
	  if (m_oFileManager->WriteEncryptedTFValues(p_oTask.GetOutputDirectory(),
												 p_oTask.GetFilename(),
												 encr_term_weights) == false)
	  {
		elog(ERROR, "Unable to write out TF index for document '%s'.\n", p_oTask.GetFilename().c_str());
	  }
	  else
	  {
		elog(DEBUG, "TF index encryption is completed for document '%s'.\n", p_oTask.GetFilename().c_str());
		m_bDone = true;
		bRet = true;
	  }
	}
  }

  return bRet;
}

