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
 *  File: ReadContentEvent.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: June 10, 2015
 *
 */

// System

// Shared
#include "site_defs.h"

// Module
#include "ReadContentEvent.h"
#include "Log.h"

ReadContentEvent::ReadContentEvent(const boost::filesystem::path& p_oDocumentFilename,
                                   const std::vector<std::string>& p_vContentWordList,
                                   boost::shared_ptr<Dictionary> p_oDictionary,
                                   boost::shared_ptr<TFIndex> p_oTFIndex) :
  Event(READ_CONTENT_EVENT),
  m_oDocumentFilename(p_oDocumentFilename),
  m_vContentWordList(p_vContentWordList),
  m_oDictionary(p_oDictionary),
  m_oTFIndex(p_oTFIndex)
{
}

ReadContentEvent::~ReadContentEvent()
{
}

bool ReadContentEvent::Execute()
{
  return false;
}

boost::filesystem::path ReadContentEvent::GetDocumentFilename()
{
  return m_oDocumentFilename;
}

const std::vector<std::string>& ReadContentEvent::GetContentWordList() const
{
  return m_vContentWordList;
}

boost::shared_ptr<Dictionary> ReadContentEvent::GetDictionary()
{
  return m_oDictionary;
}

boost::shared_ptr<TFIndex> ReadContentEvent::GetTFIndex()
{
  return m_oTFIndex;
}
