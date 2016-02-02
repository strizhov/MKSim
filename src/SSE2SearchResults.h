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
 *	OTHER DEALINGS IN THE SOFTWARE.\
 *
 *
 *  File: SSE2SearchResults.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: May 27, 2015
 *
 */

#ifndef _SSE2_SEARCH_RESULTS_H
#define _SSE2_SEARCH_RESULTS_H


// System
#include <set>
#include <string>
#include <vector>

// Shared

// Module

class SSE2SearchResults
{
public:
        SSE2SearchResults();
        virtual ~SSE2SearchResults();
        bool AddResult(const std::string& p_sDocumentFilename);
        bool Empty();
        void Clear();
        size_t Size();

        const std::set<std::string>& GetResults() const;
        bool GetResultsInVector(std::vector<std::string>& p_vSSE2Results);

private:
        std::set<std::string> m_sSSE2Results;
};
#endif
