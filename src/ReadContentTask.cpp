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

// System
#include <stdio.h>
#include <string.h>

// Shared
#include "site_defs.h"

// Module
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "ReadContentTask.h"
#include "Log.h"
#include "StatsTracker.h"
#include "ReadContentEvent.h"


ReadContentTask::ReadContentTask(const boost::filesystem::path& p_sDocumentFilename,
								 boost::shared_ptr<EventQueue> p_oEventQueue,
								 boost::shared_ptr<FileManager> p_oFileManager,
								 boost::shared_ptr<TFIndex> p_oTFIndex,
								 boost::shared_ptr<Dictionary> p_oDictionary) :
  m_bDone(false),
  m_sDocumentFilename(p_sDocumentFilename),
  m_oEventQueue(p_oEventQueue),
  m_oFileManager(p_oFileManager),
  m_oTFIndex(p_oTFIndex),
  m_oDictionary(p_oDictionary)
{
}

ReadContentTask::ReadContentTask(const std::vector<boost::filesystem::path> &p_vDocumentFilenames,
								 boost::shared_ptr<EventQueue> p_oEventQueue,
								 boost::shared_ptr<FileManager> p_oFileManager,
								 boost::shared_ptr<TFIndex> p_oTFIndex,
								 boost::shared_ptr<Dictionary> p_oDictionary) :
  m_bDone(false),
  m_vDocumentFilenames(p_vDocumentFilenames),
  m_oEventQueue(p_oEventQueue),
  m_oFileManager(p_oFileManager),
  m_oTFIndex(p_oTFIndex),
  m_oDictionary(p_oDictionary)
{
}

ReadContentTask::~ReadContentTask()
{
}

bool ReadContentTask::Execute()
{
  if (m_vDocumentFilenames.size() != 0)
  {
    if (ReadBulkFileContent(m_vDocumentFilenames) == false)
    {
      elog(ERROR, "Unable to execute bulk read task.\n");
    }
  }
  else if (m_sDocumentFilename.empty() == false)
  {
    if (ReadFileContent(m_sDocumentFilename) ==false)
    {
      elog(CRITICAL, "Unable to execute task with document '%s'\n", m_sDocumentFilename.c_str());
    }
  }
  else
  {
    elog(CRITICAL, "Unable to run task! Constructor is created incorrectly.\n");
  }

  m_bDone = true;

  return true;
}

bool ReadContentTask::Done()
{
  return m_bDone;
}

bool ReadContentTask::Process()
{
  bool bRet = false;
  if (Done())
  {
	bRet = true;
  }

  return bRet;
}

boost::shared_ptr<EventQueue> ReadContentTask::GetEventQueue()
{
  return m_oEventQueue;
}

bool ReadContentTask::ReadBulkFileContent(const std::vector<boost::filesystem::path> &p_vFilenames)
{
  for (const boost::filesystem::path& mFilename : p_vFilenames)
  {
    if (ReadFileContent(mFilename) == false)
    {
      elog(DEBUG, "Unable to read '%s'.\n", mFilename.c_str());
    }
  }

  return true;
}

bool ReadContentTask::ReadFileContent(const boost::filesystem::path& p_sFilename)
{
  bool bRet = false;

  // Read the content of document
  std::string sContent;

  // Read the content of provided file
  if (m_oFileManager->ReadContentFromFile(p_sFilename, sContent) == false)
  {
    elog(ERROR, "Unable to read content of '%s' file.\n", p_sFilename.string().c_str());
  }

  // Check if we got some content
  if (sContent.empty() == false)
  {
    std::vector<std::string> vContentWordList;

	// Chomp unnecessary content
	if (ChompDocumentContent(sContent, vContentWordList) == false)
	{
	  elog(ERROR, "Unable to chomp the content of '%s' file.\n", p_sFilename.string().c_str());
	}
	else
	{
	  boost::shared_ptr<ReadContentEvent> pEvent =
		  boost::make_shared<ReadContentEvent>(p_sFilename,
											   vContentWordList,
											   m_oDictionary,
											   m_oTFIndex);

	  // Enqueue Event object
	  if (GetEventQueue()->Enqueue(pEvent) == false)
	  {
		elog(CRITICAL, "Unable to enqueue with zone: %s\n", p_sFilename.string().c_str() );
	  }
	  else
	  {
		m_bDone = true;
		bRet = true;
	  }
	}
  }

  return bRet;
}

bool ReadContentTask::ChompDocumentContent(std::string& content, std::vector<std::string>& content_word_list)
{
  if (content.empty() == true)
  {
    elog(ERROR, "Document content is empty.\n");
	return false;
  }

  // Make content use lowercase
  boost::to_lower(content);

  // Remove delimiters
  std::string alldelims = character_literals + keyboard_delimiters;
  boost::split(content_word_list, content, boost::is_any_of(alldelims), boost::token_compress_on);

  // Remove english letter from vector of unique words
  for (const auto& letter : english_alphabet)
	content_word_list.erase(std::remove(content_word_list.begin(), content_word_list.end(), letter), content_word_list.end());

  // Remove most commonly used words from the vector of unique words
  for (const auto& word : english_common_words)
	content_word_list.erase(std::remove(content_word_list.begin(), content_word_list.end(), word), content_word_list.end());

  // Find words that are shorter than 2 characters
  std::vector<std::string> remove_words;
  for (const std::string& list_word : content_word_list)
  {
    if ((list_word.length() < CHOMP_WORD_MIN_LENGTH) || (list_word.length() > CHOMP_WORD_MAX_LENGTH))
      remove_words.emplace_back(list_word);
  }

  // Remove short words
  for (const std::string& word : remove_words)
    content_word_list.erase(std::remove(content_word_list.begin(), content_word_list.end(), word), content_word_list.end());

  return true;
}

