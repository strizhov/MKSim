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
 *  File: Dictionary.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 10, 2014
 *
 */

#ifndef _DICTIONARY_H
#define _DICTIONARY_H

// System
#include <vector>
#include <string>
#include <set>

// Shared

// Module
#include <boost/noncopyable.hpp>

class Dictionary : boost::noncopyable
{
  public:
    Dictionary();
    virtual ~Dictionary();

    bool AddWords(const std::string& p_sKeyword);
    bool AddWords(const std::vector<std::string>& p_vKeywordArray);
    bool IsEmpty();
    void Clear();

    const std::set<std::string>& GetDictionary() const;
    bool SetDictionary(const std::set<std::string>& p_stKeywordDictionary);
    std::size_t GetSize();


  private:
    std::set<std::string> m_stKeywordDictionary;
};

#endif
