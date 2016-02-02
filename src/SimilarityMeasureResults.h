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
 *  File: SimilarityMeasureResults.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: July 8, 2015
 *
 */

#ifndef _SIMILARITY_MEASURE_RESULTS_H
#define _SIMILARITY_MEASURE_RESULTS_H

// System
#include <vector>

// Shared

// Module
#include <boost/shared_ptr.hpp>
#include "DocumentSimilarity.h"

class SimilarityMeasureResults
{
public:
        SimilarityMeasureResults();
        virtual ~SimilarityMeasureResults();
        void AddResult(boost::shared_ptr<DocumentSimilarity> p_oDocumentSimilarity);
        bool AreEmpty();
        void Clear();
        size_t Size();

        const std::vector<boost::shared_ptr<DocumentSimilarity>>& GetDocumentSimilarity() const;

private:
        std::vector<boost::shared_ptr<DocumentSimilarity>> m_oSimilarityMeasureResults;
};

#endif
