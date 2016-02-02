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
 *  File: ReadContentEvent.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 10, 2015
 *
 */

#ifndef _READ_CONTENT_EVENT_H
#define _READ_CONTENT_EVENT_H

// System
#include <string>
#include <vector>

// Shared

// Module
#include <boost/shared_ptr.hpp>
#include <boost/filesystem/path.hpp>
#include "Event.h"
#include "Dictionary.h"
#include "TFIndex.h"

class ReadContentEvent : public Event
{
public:
		ReadContentEvent(const boost::filesystem::path& p_oDocumentFilename,
						 const std::vector<std::string>& p_vContentWordList,
						 boost::shared_ptr<Dictionary> p_oDictionary,
						 boost::shared_ptr<TFIndex> p_oTFIndex);
		virtual ~ReadContentEvent();

		virtual bool Execute();

		boost::filesystem::path GetDocumentFilename();
		const std::vector<std::string>& GetContentWordList() const;

		boost::shared_ptr<Dictionary> GetDictionary();
		boost::shared_ptr<TFIndex> GetTFIndex();

private:
		boost::filesystem::path m_oDocumentFilename;
		std::vector<std::string> m_vContentWordList;

		boost::shared_ptr<Dictionary> m_oDictionary;
		boost::shared_ptr<TFIndex> m_oTFIndex;

};

#endif
