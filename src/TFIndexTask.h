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
 *	OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 *  File: TFIndexTask.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 25, 2015
 *
 */


#ifndef TF_INDEX_TASK_H
#define TF_INDEX_TASK_H

// System
#include <string>
#include <vector>

// Shared

// Module

class TFIndexTask
{
public:
		TFIndexTask();
		TFIndexTask(const std::string& p_sOutputDirectory,
					const std::string& p_sFilename,
					const std::vector<long>& p_vTFValues);

		virtual ~TFIndexTask();

		bool SetOutputDirectory(const std::string& p_sOutputDirectory);
		std::string GetOutputDirectory() const;

		bool SetFileName(const std::string& p_sFilename);
		std::string GetFilename() const;

		bool SetTFValues(const std::vector<long> p_vTFValues);
		std::vector<long> GetTFValues() const;

private:
		std::string m_sOutputDirectory;
		std::string m_sFilename;
		std::vector<long> m_vTFValues;
};

#endif // TF_INDEX_TASK_H

