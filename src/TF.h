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
 *  File: TF.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 6, 2014 
 *
 */


#ifndef _TF_H
#define _TF_H

// System
#include <stdlib.h>
#include <string>

// Shared

// Module

class TF
{

public:
		TF(const std::string& p_sWord, const long& p_lValue);
		virtual ~TF();

		std::string GetWord() const;
		long GetValue() const;

		void SetWord(const std::string& p_sWord);
		void SetValue(const long& p_lValue);

private:
		std::string m_sWord;
		long m_lValue;
};


#endif /* TF_H */
