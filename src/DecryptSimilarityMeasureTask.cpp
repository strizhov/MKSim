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
 *  File: DecryptSimilarityMeasureTask.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: July 10, 2015
 *
 */

// System

// Shared

// Module
#include "DecryptSimilarityMeasureTask.h"
#include "Log.h"


DecryptSimilarityMeasureTask::DecryptSimilarityMeasureTask(boost::shared_ptr<DocumentSimilarity> p_oDocumentSimilarity,
                                                           std::string p_sSimilarityScoresDirectory,
                                                           boost::shared_ptr<FileManager> p_oFileManager,
                                                           const std::string& p_sHEManagerContext,
                                                           const std::string& p_sHEManagerSeckey,
                                                           const std::string& p_sHEManagerPubkey) :
  m_oDocumentSimilarity(p_oDocumentSimilarity),
  m_sSimilarityScoresDirectory(p_sSimilarityScoresDirectory),
  m_oFileManager(p_oFileManager),
  m_sHEManagerContext(p_sHEManagerContext),
  m_sHEManagerSeckey(p_sHEManagerSeckey),
  m_sHEManagerPubkey(p_sHEManagerPubkey)
{

}



DecryptSimilarityMeasureTask::DecryptSimilarityMeasureTask(std::vector<boost::shared_ptr<DocumentSimilarity>> p_vDocumentSimilarity,
                                                           std::string p_sSimilarityScoresDirectory,
                                                           boost::shared_ptr<FileManager> p_oFileManager,
                                                           const std::string& p_sHEManagerContext,
                                                           const std::string& p_sHEManagerSeckey,
                                                           const std::string& p_sHEManagerPubkey) :
  m_vDocumentSimilarity(p_vDocumentSimilarity),
  m_sSimilarityScoresDirectory(p_sSimilarityScoresDirectory),
  m_oFileManager(p_oFileManager),
  m_sHEManagerContext(p_sHEManagerContext),
  m_sHEManagerSeckey(p_sHEManagerSeckey),
  m_sHEManagerPubkey(p_sHEManagerPubkey)
{

}

DecryptSimilarityMeasureTask::~DecryptSimilarityMeasureTask()
{

}

bool DecryptSimilarityMeasureTask::Execute()
{
  // Init HEManager
  InitHEManager();

  if (m_vDocumentSimilarity.empty() == false)
  {
    if (Decrypt(m_vDocumentSimilarity) == false)
    {
      elog(ERROR, "Unable to execute bulk decryption task.\n");
    }
  }
  else if (m_oDocumentSimilarity)
  {
    if (Decrypt(m_oDocumentSimilarity) == false)
    {
      elog(ERROR, "Unable to execute decryption task.\n");
    }
  }

  m_bDone = true;

  return true;
}

bool DecryptSimilarityMeasureTask::Done()
{
  return m_bDone;
}

bool DecryptSimilarityMeasureTask::Process()
{
  bool bRet = false;
  if (Done())
  {
    bRet = true;
  }

  return bRet;
}

void DecryptSimilarityMeasureTask::InitHEManager()
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

bool DecryptSimilarityMeasureTask::Decrypt(std::vector<boost::shared_ptr<DocumentSimilarity> > p_vDocumentSimilarity)
{
  for (const boost::shared_ptr<DocumentSimilarity>& oDocumentSimilarity : p_vDocumentSimilarity)
  {
    if (Decrypt(oDocumentSimilarity) == false)
    {
      elog(ERROR, "Decyrption failed.\n");
    }
  }

  return true;
}

bool DecryptSimilarityMeasureTask::Decrypt(boost::shared_ptr<DocumentSimilarity> p_oDocumentSimilarity)
{
  std::vector<long> vPlaintexSimilarityCoefficients;

  //const std::vector<std::string>& vEcnrSimilarityScores = p_oDocumentSimilarity->GetEncrSimilarityCoefficients();
  std::vector<std::string> vEncrSimilarityScores;
  std::string sFilename = p_oDocumentSimilarity->GetFileName();
  if (m_oFileManager->ReadEncryptedSimilarityScores(m_sSimilarityScoresDirectory,
                                                    sFilename,
                                                    vEncrSimilarityScores) == false)
  {
    elog(ERROR, "Unable to read similarity scores for '%s' file.\n", sFilename.c_str());
    return false;
  }

  if (vEncrSimilarityScores.empty() == true)
  {
    elog(ERROR, "Encrypted similarity scores are zero.\n");
    return false;
  }

  // Fill up decrypted sim coefficients by reference
  if (m_oHEManager.DecryptArray(vEncrSimilarityScores,
                                vPlaintexSimilarityCoefficients) == false)
  {
    elog(ERROR, "HEmanager decryption failed.\n");
    return false;
  }

  // Sum up all elements from decrypted similarity coefficients
  long score = 0;
  for (const long& item : vPlaintexSimilarityCoefficients)
  {
    score += item;
  }

  // Set final score
  p_oDocumentSimilarity->SetSimilarityCoefficient(score);

   return true;
}
