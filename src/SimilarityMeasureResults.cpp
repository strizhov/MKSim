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
 *  File: SimilarityMeasureResults.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: July 8, 2015
 *
 */

// System

// Shared

// Module
#include "SimilarityMeasureResults.h"

SimilarityMeasureResults::SimilarityMeasureResults()
{

}

SimilarityMeasureResults::~SimilarityMeasureResults()
{

}

void SimilarityMeasureResults::AddResult(boost::shared_ptr<DocumentSimilarity> p_oDocumentSimilarity)
{
  m_oSimilarityMeasureResults.push_back(p_oDocumentSimilarity);
}

bool SimilarityMeasureResults::AreEmpty()
{
  return m_oSimilarityMeasureResults.empty();
}

void SimilarityMeasureResults::Clear()
{
  m_oSimilarityMeasureResults.clear();
}

const std::vector<boost::shared_ptr<DocumentSimilarity>>& SimilarityMeasureResults::GetDocumentSimilarity() const
{
  return m_oSimilarityMeasureResults;
}

size_t SimilarityMeasureResults::Size()
{
  return  m_oSimilarityMeasureResults.size();
}
