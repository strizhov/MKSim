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
 *  File: DocumentSimilarity.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 15, 2014
 *
 */

#ifndef _DOCUMENT_SIMILARITY_H
#define _DOCUMENT_SIMILARITY_H

// System
#include <string>
#include <vector>

// Shared
#include "site_defs.h"

// Module
#include "DocumentSimilarity.h"

class DocumentSimilarity
{
public:
  DocumentSimilarity(const std::string& p_sDocumentFilename);
  virtual ~DocumentSimilarity();

  void SetFileName(const std::string& p_sDocumentFilename);
  std::string GetFileName() const;

  void SetSimilarityCoefficient(const long& p_lSimilarityScore);
  long GetSimilarityCoefficient() const;

private:
  std::string m_sDocumentFilename;
  long m_lSimilarityScore;
};

#endif /* DOCUMENTSIMILARITY_H */
