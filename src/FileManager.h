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
 *	OTHER DEALINGS IN THE SOFTWARE.\
 * 
 * 
 *  File: FileManager.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 1, 2014 
 *
 */

#ifndef _FILE_MANAGER_H
#define _FILE_MANAGER_H

// System

// Shared

// Module
#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <map>
#include <set>

class FileManager : public boost::noncopyable
{
public:
		FileManager();
		virtual ~FileManager();

		// Method to find document names in given path
		bool LoadFilenamesFromPath(const std::string& p_sPath,
								   std::vector<boost::filesystem::path>& p_vDocumentFilenames);
		
		// Method to read the file
		bool ReadContentFromFile(const boost::filesystem::path& p_oDocumentFilename,
								 std::string& p_sContent);

		// Calculate file length
		long GetFileLength(std::ifstream& inStream);

		bool WriteContentToFile(const boost::filesystem::path& p_oDocumentFilename,
								const std::string& p_sContent);

		bool WriteKeywordDirectory(const std::string& p_sPath,
								   const std::string& p_sFilename,
								   const std::set<std::string>& p_stDictionary);

		bool ReadKeywordDirectory(const std::string& p_sPath,
								  const std::string& p_sFilename,
								  std::set<std::string>& p_stDictionary);

		bool WriteEncryptedTFValues(const std::string& path,
									const std::string& filename,
									const std::vector<std::string>& tfvalues);

		bool ReadEncryptedTFValues(const std::string& path,
								   const std::string& filename,
								   std::vector<std::string>& tfvalues);

		bool WriteEncryptedSimilarityScores(const std::string& p_sPath,
											const std::string& p_sFilename,
											const std::vector<std::string>& p_vEncrSimilarityScores);

		bool ReadEncryptedSimilarityScores(const std::string& p_sPath,
										   const std::string& p_sFilename,
										   std::vector<std::string>& p_vEncrSimilarityScores);

		bool WriteSSE2Index(const std::string& path,
							const std::string& filename,
							const std::map<std::string, std::string>& p_mSSE2Index);

		bool ReadSSE2Index(const std::string& path,
						   const std::string& filename,
						   std::map<std::string, std::string>& p_mSSE2Index);


private:
		bool ConstructTFIndexFilename(const std::string& path,
									  const std::string& filename,
									  std::string& output);

		bool ConstructSimilarityScoresFilename(const std::string& p_sPath,
											   const std::string& p_sFilename,
											   std::string& p_sOutput);

		bool ConstructSSE2IndexFilename(const std::string& path,
										const std::string& filename,
										std::string& output);

		bool ConstructDictionaryFilename(const std::string& p_sPath,
										 const std::string& p_sFilename,
										 std::string& sOutput);
};

#endif  /* _FILEMANAGER_H */

