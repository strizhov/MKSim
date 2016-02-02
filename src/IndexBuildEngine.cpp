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
 *  File: IndexBuildEngine.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: May 27, 2014
 *
 */

// System
#include <iostream>
#include <iterator>
#include <vector>
#include <stdlib.h>
#include <time.h>

// Shared
#include "site_defs.h"

// Module
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "IndexBuildEngine.h"
#include "ReadContentTask.h"
#include "TFIndexTask.h"
#include "EncryptTFIndexTask.h"
#include "ThreadPool.h"
#include "Thread.h"

IndexBuildEngine::IndexBuildEngine(const std::string& p_sPathToDocumentFiles,
								   const std::string& p_sOutputDirectory,
								   boost::shared_ptr<FileManager> p_oFileManager,
								   boost::shared_ptr<LTCManager> p_oLTCManager,
								   boost::shared_ptr<HEManager> p_oHEManager,
								   boost::shared_ptr<ThreadPool> p_oPool,
								   boost::shared_ptr<EventQueue> p_oEventQueue) :
	m_sPathToDocumentFiles(p_sPathToDocumentFiles),
	m_sOutputDirectory(p_sOutputDirectory),
	m_oFileManager(p_oFileManager),
	m_oLTCManager(p_oLTCManager),
	m_oHEManager(p_oHEManager),
	m_oPool(p_oPool),
	m_oEventQueue(p_oEventQueue)
{
}

IndexBuildEngine::~IndexBuildEngine()
{
}

bool IndexBuildEngine::Init()
{
  m_oDictionary = boost::make_shared<Dictionary>();
  m_oTFIndex = boost::make_shared<TFIndex>();
  m_oSSE2Index = boost::make_shared<SSE2Index>();

  return true;
}

void IndexBuildEngine::Startup()
{
  shutdown_ = false;

  std::vector<boost::filesystem::path> m_vDocumentFilenames;

  // Find file names in provided file directory
  if (m_oFileManager->LoadFilenamesFromPath(m_sPathToDocumentFiles, m_vDocumentFilenames) == false)
  {
    elog(ERROR, "Unable to read %s.\n", m_sPathToDocumentFiles.c_str());
    return;
  }
  else
  {
	elog(DEBUG, "Reading document collection begins.\n");
	if (ReadDocumentCollectionWithPool(m_vDocumentFilenames) == false)
	{
	  elog(ERROR, "Unable to read document collection.\n");
	  return;
	}
	elog(DEBUG, "Reading document collection ends.\n");

        // Let it finish completely
	sleep(10);

	elog(DEBUG, "Writing keyword dictionary to file begins.\n");
	if (SaveDictionary() == false)
	{
	  elog(ERROR, "Unable to write keyword dictionary to the file.\n");
	  return;
	}
	elog(DEBUG, "Writing keyword dictionary to file ends.\n");

	// TF Index construction and encryption
	elog(DEBUG, "Calculating term frequency begins.\n");
	if (CalcTermFrequency() == false)
	{
	  elog(ERROR, "Unable to calcualte term frequency for the document collection.\n");
	  return;
	}
	elog(DEBUG, "Calculating term frequency ends.\n");

	elog(DEBUG, "Homomorphic Encryption of term frequency begins.\n");
	if (EncryptTermFrequencyWithPool() == false)
	{
	  elog(ERROR, "Unable to encrypt term frequency for  the document collection\n");
	  return;
	}
	elog(DEBUG, "Homomorphic Encryption of term frequency ends.\n");

	// SSE2 Index construction and encryption
    elog(DEBUG, "SSE2 index construction begins.\n");
    if (MakeSSE2Index() == false)
	{
      elog(ERROR, "Unable to construct SSE2 index for the document collection\n");
	  return;
	}
    elog(DEBUG, "SSE2 index construction ends.\n");

    elog(DEBUG, "Writing SSE2 index to the file begins.\n");
    if (SaveSSE2Index() == false)
    {
      elog(ERROR, "Unable to write SSE2 index to the file.\n");
      return;
    }
    elog(DEBUG, "Writing SSE2 index to the file ends.\n");

	//PrintTermFrequencyFromDocumentIndex();

  } // LoadFiles

  return;
}

void IndexBuildEngine::Shutdown()
{
  shutdown_ = true;
}


bool IndexBuildEngine::ReadDocumentCollectionWithPool(const std::vector<boost::filesystem::path>& p_vDocumentFilenames)
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (p_vDocumentFilenames.empty() == true)
  {
	elog(ERROR, "No document files found.\n");
	return false;
  }

  int iDocumentNumber = p_vDocumentFilenames.size();
  int iPoolSize = m_oPool->GetSize();

  int iDocumentOnEachThread = iDocumentNumber / iPoolSize;
  int iReminder = iDocumentNumber % iPoolSize;

  int i = 0;
  while (i < (iDocumentOnEachThread * iPoolSize))
  {
    std::vector<boost::filesystem::path> vBulkFilenames(p_vDocumentFilenames.begin() + i,
                                                        p_vDocumentFilenames.begin() + i + iDocumentOnEachThread);

	boost::shared_ptr<ReadContentTask> pTask =
		  boost::make_shared<ReadContentTask>(vBulkFilenames,
											  m_oEventQueue,
											  m_oFileManager,
											  m_oTFIndex,
											  m_oDictionary);

	Thread *pThrd = m_oPool->CheckOut();
	pThrd->SetTask(pTask);

    i = i + iDocumentOnEachThread;
  }

  // Check if there are some documents left
  if (iReminder > 0)
  {
    int start = iDocumentOnEachThread * iPoolSize;
    int end = start + iReminder;

    for (i = start; i < end; i++)
    {
      boost::shared_ptr<ReadContentTask> pTask =
            boost::make_shared<ReadContentTask>(p_vDocumentFilenames[i],
                                                m_oEventQueue,
                                                m_oFileManager,
                                                m_oTFIndex,
                                                m_oDictionary);

      Thread *pThrd = m_oPool->CheckOut();
      pThrd->SetTask(pTask);
    }
  }

  while (m_oPool->GetNumFree() < m_oPool->GetSize())
  {
	sleep(2);
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

bool IndexBuildEngine::SaveDictionary()
{

  if (m_oDictionary->IsEmpty() == true)
  {
    elog(ERROR, "Keyword Dictionary is empty.\n");
    return false;
  }

  if (m_oFileManager->WriteKeywordDirectory(m_sOutputDirectory,
                                              DICTIONARY_FILENAME,
                                              m_oDictionary->GetDictionary()) == false)
  {
    elog(ERROR, "Filemanager failed for writing dictionary to the file.\n");
    return false;
  }

  return true;
}

bool IndexBuildEngine::CalcTermFrequency()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (m_oDictionary->IsEmpty() == true)
  {
	elog(ERROR, "Dictionary is empty.\n");
	return false;
  }

  if (m_oTFIndex->IsEmpty() == true)
  {
	elog(ERROR, "Document indexes are empty.\n");
	return false;
  }

  elog(DEBUG, "Keyword Dictionary size is '%zu'.\n", m_oDictionary->GetSize());

  // Get the dictionary and size
  const std::set<std::string>& vDictionary = m_oDictionary->GetDictionary();

  // Get document indexes
  const std::vector<boost::shared_ptr<DocumentIndex>>& vDocumentIndexes = m_oTFIndex->GetTFIndexes();

  // Iterate thru word dictionary
  for (const std::string& sWord : vDictionary)
  {
    // Iterate document indexes, we will modify each index
    for (boost::shared_ptr<DocumentIndex> oDocumentIndex : vDocumentIndexes)
    {
      // Find if dictionary word is in document
      if (oDocumentIndex->CheckWordAppearance(sWord) == false)
      {
        // Set TF to 0
        oDocumentIndex->SetTF(sWord, 0);
      }
      else
      {
        // Get number of times a word appears in the document
        long lWordCounter = oDocumentIndex->GetWordAppearance(sWord);

        // Calculate term frequency for a word
        long tf = static_cast<long>(m_oDictionary->GetSize()) / static_cast<long>(lWordCounter);

        // Set word-tf value in document index
        oDocumentIndex->SetTF(sWord, tf);
      }
    }
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

bool IndexBuildEngine::EncryptTermFrequencyWithPool()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (m_oTFIndex->IsEmpty() == true)
  {
	elog(ERROR, "Document indexes are empty.\n");
	return false;
  }

  // Get document indexes
  const std::vector<boost::shared_ptr<DocumentIndex>>& vDocumentIndexes = m_oTFIndex->GetTFIndexes();

  int iNumberOfIndexes = vDocumentIndexes.size();
  int iPoolSize = m_oPool->GetSize();

  int iNumberOfIndexesOnEachThread = iNumberOfIndexes / iPoolSize;
  int iReminder = iNumberOfIndexes % iPoolSize;

  int i = 0;
  while (i < (iNumberOfIndexesOnEachThread * iPoolSize))
  {
    // Bulk of TF indexes
    std::vector<TFIndexTask> vIndexTasks;

	for (int j = i; j < i + iNumberOfIndexesOnEachThread; j++)
	{
	  const boost::shared_ptr<DocumentIndex>& oDocumentIndex = vDocumentIndexes[j];
	  std::vector<long> vTFValues;
	  // Get values by reference
	  if (oDocumentIndex->GetTfValues(vTFValues) == false)
	  {
		elog(ERROR, "Unable to get term frequency values from document collection.\n");
		return false;
	  }

	  // Get filename without path from each document index
	  std::string sFilename = oDocumentIndex->GetFilename().stem().string();

	  TFIndexTask oTask = TFIndexTask(m_sOutputDirectory, sFilename, vTFValues);
	  vIndexTasks.emplace_back(oTask);
	}

	// Create encryption task for each thread
	boost::shared_ptr<EncryptTFIndexTask> pTask =
		boost::make_shared<EncryptTFIndexTask>(vIndexTasks,
											   m_oHEManager->GetInitContext(),
											   m_oHEManager->GetInitSecretKey(),
											   m_oHEManager->GetInitPublicKey(),
											   m_oFileManager);

	Thread *pThrd = m_oPool->CheckOut();
	pThrd->SetTask(pTask);

    i = i + iNumberOfIndexesOnEachThread;
  }

  if (iReminder > 0)
  {
    int start = iNumberOfIndexesOnEachThread * iPoolSize;
    int end = start + iReminder;

	for (i = start; i < end; i++)
	{
	  const boost::shared_ptr<DocumentIndex>& oDocumentIndex = vDocumentIndexes[i];
	  std::vector<long> vTFValues;
	  // Get values by reference
	  if (oDocumentIndex->GetTfValues(vTFValues) == false)
	  {
		elog(ERROR, "Unable to get term frequency values from document collection.\n");
		return false;
	  }

	  // Get filename without path from each document index
	  std::string sFilename = oDocumentIndex->GetFilename().stem().string();

	  TFIndexTask oTask = TFIndexTask(m_sOutputDirectory, sFilename, vTFValues);

	  // Create encryption task for each thread
	  boost::shared_ptr<EncryptTFIndexTask> pTask =
			boost::make_shared<EncryptTFIndexTask>(oTask,
												   m_oHEManager->GetInitContext(),
												   m_oHEManager->GetInitSecretKey(),
												   m_oHEManager->GetInitPublicKey(),
												   m_oFileManager);
	  Thread *pThrd = m_oPool->CheckOut();
	  pThrd->SetTask(pTask);
	}
  }

  while (m_oPool->GetNumFree() < m_oPool->GetSize())
  {
	sleep(2);
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

/*
bool IndexBuildEngine::GenRandomSearchQuery(const uint& num_words, std::string& search_query)
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (search_query.empty() == false)
    search_query.clear();

  // Check if dictionary is empty
  if (dictionary.IsEmpty() == true)
  {
    elog(ERROR, "Keyword dictionary is empty.\n");
    return false;
  }

  // Get dictionary size
  std::size_t dict_size = dictionary.GetSize();

  // Get dictionary
  const std::vector<std::string>& dict = dictionary.GetDictionary();

  // Init prng
  srand (time(NULL));

  // Get
  for (uint i = 0; i < num_words; i++)
  {
    // Get random index element within the dictionary boundary
    uint elem = rand() % dict_size;

    // Contatenate search query with word from dictionary
    search_query = search_query + " " + dict[elem];
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}
*/


/*
bool IndexBuildEngine::CreateDocumentIndexes()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (document_filenames_.empty() == true)
  {
    elog(ERROR, "No document files found.\n");
    return false;
  }

  // Read the content of each document
  for (const boost::filesystem::path& filename : document_filenames_)
  {
    std::string filecontent;

    // Read the content of provided file
    if (mFileManager->ReadContentFromFile(filename.string(), filecontent) == false)
    {
      elog(ERROR, "Unable to read content of '%s' file.\n", filename.string().c_str());
      continue; // go with next file
    }

    // Check if we got some content
    if (filecontent.empty() == false)
    {
      std::vector<std::string> content_word_list;

      // Chomp unnecessary content
      if (ChompDocumentContent(filecontent, content_word_list) == false)
      {
        elog(ERROR, "Unable to chomp the content of '%s' file.\n", filename.string().c_str());
        continue; // go with next file
      }
      else
      {
        // Create document index
        DocumentIndex index = DocumentIndex(filename.string());

        // Add found words
        index.AddWords(content_word_list);

        // Add document indexes
        document_indexes_.push_back(index);

        // Append words to the dictionary
        dictionary.AddWords(content_word_list);
      }
    }
  }
  elog(DEBUG, "Size of dictionary: '%d'\n", dictionary.GetSize());

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}
*/

/*
bool IndexBuildEngine::MakeWordDictionary()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (document_indexes_.empty() == true)
  {
    elog(ERROR, "No document indexes found.\n");
    return false;
  }

  if (dictionary.IsEmpty() == false)
  {
    elog(ERROR, "Word dictionary is not empty!\n");
    return false;
  }

  // Iterate document indexes, combine all words and create unique list
  for (const DocumentIndex& di : document_indexes_)
  {
     // Retrieve document word list
    const std::vector<std::string>& document_word_list = di.GetDocumentWordList();

    // Add words
    if (dictionary.AddWords(document_word_list) == false)
    {
      elog(ERROR, "Unable to add words from document '%s' to the dictionary\n", di.GetFilename().c_str());
      return false;
    }
  }

  // Sort and unique
  if (dictionary.SortAndMakeUnique() == false)
  {
    elog(ERROR, "Unable to sort and unique dictionary\n");
    return false;
  }

  elog(DEBUG, "Dictionary size: %ld\n", dictionary.GetSize());

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}
*/


void IndexBuildEngine::PrintTermFrequencyFromDocumentIndex()
{
  // Print out dictionary
  const std::set<std::string>& dict = m_oDictionary->GetDictionary();

  elog(DEBUG, "Keyword dictionary has size '%d':\n", dict.size());
/*
  for (const std::string& dict_word : dict)
  {
    elog(DEBUG, "%s\n", dict_word.c_str());
  }
*/
  // Get document indexes
  const std::vector<boost::shared_ptr<DocumentIndex>>& document_indexes_ = m_oTFIndex->GetTFIndexes();

  // Print TF-IDF values for each document
  //for (boost::shared_ptr<DocumentIndex> di : document_indexes_)
  boost::shared_ptr<DocumentIndex> di = document_indexes_[10];
  {
    std::vector<std::string> tf_words;
	if (di->GetTFWords(tf_words) == false)
    {
      elog(DEBUG, "Unable to retrieve words from TF object\n");
      return;
    }

    for (const std::string& tf_word : tf_words)
    {
      long tf_value = di->GetTFValueByWord(tf_word);
      elog(DEBUG, "Document '%s', word '%s' has TF value as '%ld'.\n",
		   di->GetFilename().c_str(),
           tf_word.c_str(),
           tf_value);
    }
  }
}

/*
bool IndexBuildEngine::EncryptTermFrequency()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  std::vector<long> tf_index;

  // Get document indexes
  std::vector<boost::shared_ptr<DocumentIndex>> document_indexes_ = mTFIndex->GetTFIndexes();

  // Iterate document indexes, we will modify each index
  for (boost::shared_ptr<DocumentIndex> di: document_indexes_)
  {
    tf_index.clear();

    // Get values by reference
	if (di->GetTFValuesAsLong(tf_index) == false)
    {
      elog(ERROR, "Unable to get term frequency values from document collection.\n");
      return false;
    }

	elog(DEBUG, "Term frequencry from '%s'\n", di->GetFilename().c_str());


    std::vector<std::string> encr_term_weights;

    // Encrypt! Store ciphtertext in document index object
	if (GetHEManager()->EncryptArray(tf_index, encr_term_weights) == false)
    {
      elog(ERROR, "Array encryption failed!\n");
      return false;
    }
    else
    {
	  di->SetEncryptedTF(encr_term_weights);
    }
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}
*/


bool IndexBuildEngine::MakeSSE2Index()
{
  StatsTracker::GetInstance().RecordMethodStartTime();

  if (m_oDictionary->IsEmpty() == true)
  {
	elog(ERROR, "Dictionary is empty.\n");
	return false;
  }

  if (m_oTFIndex->IsEmpty() == true)
  {
	elog(ERROR, "Document indexes are empty.\n");
	return false;
  }

  // Clear up the lookup table
  if (m_oSSE2Index->IsEmpty() == false)
    m_oSSE2Index->Clear();

  // Get the dictionary
  const std::set<std::string>& vDictionary = m_oDictionary->GetDictionary();

  // Get document indexes
  const std::vector<boost::shared_ptr<DocumentIndex>>& vDocumentIndexes = m_oTFIndex->GetTFIndexes();

  // Word existence counter
  long ctr = 0;

  // Run on each dictionary word
  for (const std::string& sWord : vDictionary)
  {
    ctr = 0;

    // Get the each document
	for (boost::shared_ptr<DocumentIndex> oDocumentIndex : vDocumentIndexes)
    {
	  // Find if dictionary word is in document
	  if (oDocumentIndex->CheckWordAppearance(sWord) == true)
	  {
		// Create a table word as concatenation of word and counter
		std::string sSSE2Word = sWord + boost::lexical_cast<std::string>(ctr);

		// Encrypt it
		std::string sSSE2EncryptedWord;
		if (m_oLTCManager->CalculateHMAC(sSSE2Word,
										   sSSE2EncryptedWord) == false)
		{
		  elog(ERROR, "Unable to encrypt a SSE2 index word\n");
		  return false;
		}

        // Get Filename without path and extension
        std::string sFilename = oDocumentIndex->GetFilename().stem().string();

		// Add to the sse2index: tableword and filename
		m_oSSE2Index->AddEntry(sSSE2EncryptedWord, sFilename);

		// Increment the counter
		ctr++;

	  }
    }
  }

  StatsTracker::GetInstance().RecordMethodEndTime();
  StatsTracker::GetInstance().GetResultPerformanceTime();

  return true;
}

bool IndexBuildEngine::SaveSSE2Index()
{
  if (m_oSSE2Index->IsEmpty() == true)
  {
    elog(ERROR, "SSE2 Index is empty.\n");
    return false;
  }

  if (m_oFileManager->WriteSSE2Index(m_sOutputDirectory,
                                       SSE2_INDEX_FILENAME,
                                       m_oSSE2Index->GetIndex()) == false)
  {
    elog(ERROR, "Filemanager failed to write SSE2 index to the file.\n");
    return false;
  }

  return true;
}


void IndexBuildEngine::PrintSSE2Index()
{
  m_oSSE2Index->PrintSSE2Index();
}


