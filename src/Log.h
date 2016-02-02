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
 *	OTHER DEALINGS IN THE SOFTWARE.\
 *
 *
 *  File: Log.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 20, 2014
 *
 */

#ifndef _LOG_H
#define _LOG_H

// System
#include <string>
#include <map>

// Shared

// Module
#include <boost/noncopyable.hpp>


#define elog(XX_LVL, X, ...) Log::log(__FILE__, __LINE__, XX_LVL, X, ##__VA_ARGS__)
#define syslog(X, ...) Log::sysLog(__FILE__, __LINE__, X, ##__VA_ARGS__)

enum LOGLEVEL
{
  CRITICAL = 0,
  ERROR,
  WARNING,
  INFO,
  DEBUG
};

class Log : boost::noncopyable
{
  public:
    Log();
    virtual ~Log();

    void Close();

    int GetLevel();
    void SetLevel(int level);

    const std::string& GetFileName();
    bool SetFileName(const char *file);

    FILE *GetFile();
    std::string LevelToStr(int level);
    int StrToLevel(std::string& level);

    static Log& GetInstance();

    static void log(const char *p_szFile, int p_iLine, int p_iLevel, const char * p_szFmt, ...);
    static void sysLog(const char *p_szFile, int p_iLine, const char * p_szFmt, ...);

    typedef std::map<std::string, int> LogLevelMap_t;
    typedef LogLevelMap_t::iterator LogLevelIter_t;

  private:
    static Log instance_;

    int level_;
    FILE *file_;
    std::string *levelnames_;
    std::string filename_;
    LogLevelMap_t namemap_;
};

#endif

