/* 
 * 	Copyright (c) 2014 Colorado State University
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
 *  File: main.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Dec 15, 2014
 *
 */

// System
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <iterator>

// Shared
#include "site_defs.h"

// Module
#include <boost/filesystem.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include "FileManager.h"
#include "Log.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

bool FileManager::LoadFilenamesFromPath(const std::string& p_sPath,
                                        std::vector<boost::filesystem::path>& p_vDocumentFilenames)
{
  if (p_sPath.empty() == true)
  {
    elog(ERROR, "Given path is empty.\n");
    return false;
  }

  boost::filesystem::path p(p_sPath);
  try
  {
	if (boost::filesystem::exists(p) == true)
	{
	  if (boost::filesystem::is_regular_file(p) == true)
	  {
		elog(ERROR, "Regular file is provided in input. Please give a directory path with files.\n");
		return false;
	  }
	  else if (boost::filesystem::is_directory(p) == true)
	  {
		std::vector<boost::filesystem::path> vPathFilenames;

		std::copy(	boost::filesystem::directory_iterator(p),
					boost::filesystem::directory_iterator(),
					std::back_inserter(vPathFilenames));

		std::sort(vPathFilenames.begin(), vPathFilenames.end());

		// Place back filenames as string
		for (const boost::filesystem::path& path : vPathFilenames)
		{
		  p_vDocumentFilenames.emplace_back(path.string());
		}

		return true;
	  }
	  else
	  {
		elog(ERROR, "'%s' exist, but is neither a regular file nor a directory.\n", p_sPath.c_str());
		return false;
	  }
	}
	else
	{
	  elog(ERROR, "'%s' does not exist.\n", p_sPath.c_str());
	  return false;
	}
  }
  catch (const boost::filesystem::filesystem_error& ex)
  {
	elog (CRITICAL, "Got exception: %s\n", ex.what()) ;
  }

  elog(ERROR, "Unable to process '%s'.\n", p_sPath.c_str());
  return false;
}

bool FileManager::ReadContentFromFile(const boost::filesystem::path& p_oDocumentFilename,
                                      std::string& p_sContent)
{
  if (p_oDocumentFilename.empty() == true)
  {
    elog(ERROR, "Filename is empty. Unable to read.\n");
    return false;
  }

  // Open filename
  std::ifstream inStream(p_oDocumentFilename.string());
  if (inStream.is_open() == false)
  {
    elog(ERROR, "Attempt to open '%s' failed.\n", p_oDocumentFilename.string().c_str());
    return false;
  }

  // Stringstream
  std::stringstream buffer;
  buffer << inStream.rdbuf();

  inStream.close();

  p_sContent = buffer.str();

  return true;
}

long FileManager::GetFileLength(std::ifstream& inStream)
{
  inStream.seekg (0, inStream.end);
  long length = inStream.tellg();
  inStream.seekg (0, inStream.beg);

  return length; 
}

bool FileManager::WriteContentToFile(const boost::filesystem::path& p_oDocumentFilename,
                                     const std::string& p_sContent)
{
  if (p_oDocumentFilename.empty() == true)
  {
    elog(ERROR, "Filename is empty. Unable to write data.\n");
    return false;
  }

  if (p_sContent.empty() == true)
  {
    elog(ERROR, "Content is empty. Unable to write to '%s' file.\n", p_oDocumentFilename.string().c_str());
  }

  std::ofstream outStream(p_oDocumentFilename.string());

  // Write and close
  outStream.write(p_sContent.c_str(), p_sContent.size());
  outStream.close();

  return true;
}

bool FileManager::WriteEncryptedTFValues(const std::string& path,
										 const std::string& filename,
										 const std::vector<std::string>& tfvalues)
{
  std::string full_filename_path;
  if (ConstructTFIndexFilename(path, filename, full_filename_path) == false)
  {
	elog(ERROR, "Unable to construct filename path.\n");
	return false;
  }

  {
	std::ofstream ofs(full_filename_path);
	boost::archive::text_oarchive oa(ofs);
	oa & tfvalues;
  }

  return true;
}

bool FileManager::ReadEncryptedTFValues(const std::string& path,
										const std::string& filename,
										std::vector<std::string>& tfvalues)
{
  std::string full_filename_path;
  if (ConstructTFIndexFilename(path, filename, full_filename_path) == false)
  {
	elog(ERROR, "Unable to construct filename path.\n");
	return false;
  }

  {
	   std::ifstream ifs(full_filename_path);
	   boost::archive::text_iarchive ia(ifs);
	   ia & tfvalues;
  }

  return true;
}

bool FileManager::ConstructTFIndexFilename(const std::string& path,
										   const std::string& filename,
										   std::string& output)
{
  if (path.empty() == true)
  {
    elog(DEBUG, "Given path is empty.\n");
    return false;
  }

  if (filename.empty() == true)
  {
    elog(DEBUG, "Given filename is empty.\n");
    return false;
  }

  output = path + "/" + filename + ENCR_TF_INDEX_FILE_EXTENSION;

  return true;
}

bool FileManager::WriteEncryptedSimilarityScores(const std::string &p_sPath,
                                                 const std::string &p_sFilename,
                                                 const std::vector<std::string> &p_vEncrSimilarityScores)
{
  std::string full_filename_path;
  if (ConstructSimilarityScoresFilename(p_sPath, p_sFilename, full_filename_path) == false)
  {
    elog(ERROR, "Unable to construct filename path.\n");
    return false;
  }

  {
    std::ofstream ofs(full_filename_path);
    boost::archive::text_oarchive oa(ofs);
    oa & p_vEncrSimilarityScores;
  }

  return true;
}

bool FileManager::ReadEncryptedSimilarityScores(const std::string &p_sPath,
                                                const std::string &p_sFilename,
                                                std::vector<std::string> &p_vEncrSimilarityScores)
{
  std::string full_filename_path;
  if (ConstructSimilarityScoresFilename(p_sPath, p_sFilename, full_filename_path) == false)
  {
    elog(ERROR, "Unable to construct filename path.\n");
    return false;
  }

  {
       std::ifstream ifs(full_filename_path);
       boost::archive::text_iarchive ia(ifs);
       ia & p_vEncrSimilarityScores;
  }

  return true;
}

bool FileManager::ConstructSimilarityScoresFilename(const std::string& p_sPath,
													const std::string& p_sFilename,
													std::string& p_sOutput)
{
  if (p_sPath.empty() == true)
  {
    elog(DEBUG, "Given path is empty.\n");
    return false;
  }

  if (p_sFilename.empty() == true)
  {
    elog(DEBUG, "Given filename is empty.\n");
    return false;
  }

  p_sOutput = p_sPath + "/" + p_sFilename + ENCR_SIM_RESULT_FILE_EXTENSION;

  return true;
}


bool FileManager::ConstructDictionaryFilename(const std::string &p_sPath,
											  const std::string &p_sFilename,
											  std::string &sOutput)
{
  if (p_sPath.empty() == true)
  {
    elog(DEBUG, "Given path is empty.\n");
    return false;
  }

  if (p_sFilename.empty() == true)
  {
    elog(DEBUG, "Given filename is empty.\n");
    return false;
  }

  sOutput = p_sPath + "/" + p_sFilename;

  return true;
}

bool FileManager::ConstructSSE2IndexFilename(const std::string& path,
											 const std::string& filename,
											 std::string& output)
{
  if (path.empty() == true)
  {
	elog(DEBUG, "Given path is empty.\n");
	return false;
  }

  if (filename.empty() == true)
  {
	elog(DEBUG, "Given filename is empty.\n");
	return false;
  }

  output = path + "/" + filename;

  return true;
}


bool FileManager::WriteSSE2Index(const std::string& path,
								 const std::string& filename,
								 const std::map<std::string, std::string>& p_mSSE2Index)
{
  if (p_mSSE2Index.empty() == true)
  {
	elog(DEBUG, "SSE2Index is empty.\n");
	return false;
  }

  std::string full_filename_path;
  if (ConstructSSE2IndexFilename(path, filename, full_filename_path) == false)
  {
	elog(ERROR, "Unable to construct filename path.\n");
	return false;
  }

  {
	std::ofstream ofs(full_filename_path);
	boost::archive::text_oarchive oa(ofs);
	oa & p_mSSE2Index;
  }


  return true;
}

bool FileManager::ReadSSE2Index(const std::string& path,
								const std::string& filename,
								std::map<std::string, std::string> &p_mSSE2Index)
{
  std::string full_filename_path;
  if (ConstructSSE2IndexFilename(path, filename, full_filename_path) == false)
  {
	elog(ERROR, "Unable to construct filename path.\n");
	return false;
  }

  {
	std::ifstream ifs(full_filename_path);
	boost::archive::text_iarchive ia(ifs);
	ia & p_mSSE2Index;
  }

  return true;
}

bool FileManager::WriteKeywordDirectory(const std::string &p_sPath,
                                        const std::string &p_sFilename,
                                        const std::set<std::string> &p_stDictionary)
{
  if (p_stDictionary.empty() == true)
  {
    elog(ERROR, "Keyword dictionary is empty.\n");
    return false;
  }

  std::string sFullFilenamePath;
  if (ConstructDictionaryFilename(p_sPath, p_sFilename, sFullFilenamePath) == false)
  {
    elog(ERROR, "Unable to construct filename path.\n");
    return false;
  }

  {
    std::ofstream ofs(sFullFilenamePath);
    boost::archive::text_oarchive oa(ofs);
    oa & p_stDictionary;
  }

  return true;
}

bool FileManager::ReadKeywordDirectory(const std::string &p_sPath,
                                       const std::string &p_sFilename,
                                       std::set<std::string> &p_stDictionary)
{
  std::string sFullFilenamePath;
  if (ConstructDictionaryFilename(p_sPath, p_sFilename, sFullFilenamePath) == false)
  {
    elog(ERROR, "Unable to construct filename path.\n");
    return false;
  }

  {
    std::ifstream ifs(sFullFilenamePath);
    boost::archive::text_iarchive ia(ifs);
    ia & p_stDictionary;
  }

  return true;
}



