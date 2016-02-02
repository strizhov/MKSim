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
 *  File: DocumentIndex.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 6, 2014 
 *
 */

#ifndef _DOCUMENT_INDEX_H
#define _DOCUMENT_INDEX_H

// System
#include <string>
#include <vector>
#include <unordered_map>

// Shared

// Module
#include <boost/filesystem.hpp>
#include "TF.h"

class DocumentIndex
{

public:
        DocumentIndex(const boost::filesystem::path& p_oDocumentFilename);
        virtual ~DocumentIndex();

        void AddWords(const std::vector<std::string>& p_vDocumentWordList);
        bool CheckWordAppearance(const std::string& p_sWord) const;
        long GetWordAppearance(const std::string& p_sWord) const;

        boost::filesystem::path GetFilename() const;
        const std::unordered_map<std::string, long>& GetDocumentWordList() const;

        bool SetTF(const std::string& p_sWord, const long& p_lValue);
        bool GetTFWords(std::vector<std::string>& p_vTFWords) const;

        bool CheckWordExistence(const std::string& p_sWord) const;
        long  GetTFValueByWord(const std::string& p_sWord) const;

        bool GetTfValues(std::vector<long>& p_vTFValues) const;

        bool SetEncryptedTF(std::vector<std::string>& p_vEncrTermWeights);
        const std::vector<std::string>& GetEncryptedTF() const;

private:
        boost::filesystem::path m_oDocumentFilename;

        // A map that has word/number of appearances of the word
        // For example, if word "table" appears 4 times in the document,
        // the map will have values <table, 4>
        std::unordered_map<std::string, long> m_umWordsWithCounters;

        std::vector<TF> m_vTermWeights;
        std::vector<std::string> m_vEncrTermWeights;
};

#endif /* _DOCUMENTINDEX_H */

