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
 *  File: SSE2Index.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 13, 2014
 *
 */

#ifndef _SSE2_INDEX_H
#define _SSE2_INDEX_H

// System
#include <string>
#include <map>

// Shared

// Module

class SSE2Index
{
public:
		SSE2Index();
		virtual ~SSE2Index();

		bool AddEntry(const std::string& p_sWord, const std::string& p_sDocumentFilename);
		bool IsExist(const std::string& p_sWord);
		bool GetEntry(const std::string& p_sWord, std::string& p_sDocumentFilename);
		void Clear();
		bool IsEmpty();

		const std::map<std::string, std::string>& GetIndex() const;
		bool SetIndex(const std::map<std::string, std::string>& p_mIndex);

		size_t GetDocumentCollectionSize();

		void PrintSSE2Index();
private:
		std::map<std::string, std::string> m_mIndex;
};

#endif
