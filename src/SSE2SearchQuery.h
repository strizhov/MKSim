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
 *  File: SSE2SearchQuery.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 14, 2014
 *
 */

#ifndef _SSE2_SEARCH_QUERY_H
#define _SSE2_SEARCH_QUERY_H

// System
#include <vector>
#include <string>

// Shared

// Module

class SSE2SearchQuery
{
  public:
    SSE2SearchQuery();
    virtual ~SSE2SearchQuery();

    bool AddWord(const std::string& p_sWord);
    void ClearSSE2SearchQuery();
    bool IsSSE2SearchQueryEmpty();
    const std::vector<std::string>& GetSearchQuery() const;

    bool AddEncryptedWord(const std::string& p_sEncrWord);
    void ClearSSE2EncryptedSearchQuery();
    bool IsSSE2EncryptedSearchQueryEmpty();
    const std::vector<std::string>& GetEncryptedSearchQuery() const;


  private:
    std::vector<std::string> m_vSearchQuery;
    std::vector<std::string> m_vEncrSearchQuery;
};

#endif
