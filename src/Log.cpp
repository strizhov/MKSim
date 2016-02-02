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
 *  File: Log.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 20, 2014
 *
 */

// System
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <sstream>

// Shared

// Module
#include "Log.h"

Log Log::instance_;

Log::Log()
  : level_(CRITICAL),
    file_(stdout),
    levelnames_(NULL)
{
  levelnames_ = new std::string[5];
  levelnames_[CRITICAL] = std::string("CRITICAL");
  levelnames_[ERROR] = std::string("ERROR");
  levelnames_[WARNING] = std::string("WARNING");
  levelnames_[INFO] = std::string("INFO");
  levelnames_[DEBUG] = std::string("DEBUG");

  for (int i = 0; i < 5; i++)
  {
    namemap_[(levelnames_[i])] = i;
  }

  setlogmask(LOG_UPTO(LOG_NOTICE));
  openlog("main", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
}

Log::~Log()
{
  Close();
  closelog();

  delete[] levelnames_;
}

void Log::Close()
{
  if (file_ != NULL && file_ != stdout)
  {
    fclose(file_);
    file_ = NULL;
  }
}

int Log::GetLevel()
{
  return level_;
}

void Log::SetLevel(int level)
{
  level_ = level;
}

const std::string& Log::GetFileName()
{
  return filename_;
}

bool Log::SetFileName(const char *filename)
{
  bool bRet = false;

  if (filename != NULL && filename[0] != '\0')
  {
    if (file_ != NULL && file_ != stdout)
    {
      fclose(file_);
      file_ = NULL;
    }

    filename_ = filename;

    file_ = fopen(filename_.c_str(), "w");
    if (file_ == NULL)
    {
#ifdef DEBUG
      fprintf(stderr, "%s [%d]: Unable to open file '%s': %s\n",
              __FILE__,
              __LINE__,
              m_sFile.c_str(),
              strerror(errno));
#endif
      file_ = stdout;
      filename_ = "";
    }
    else
    {
      bRet = true;
    }
  }

  return bRet;
}

inline FILE *Log::GetFile()
{
  return file_;
}

inline std::string Log::LevelToStr(int level)
{
  std::string sRet;
  if (level <= DEBUG)
  {
    sRet = levelnames_[level];
  }

  return sRet;
}

int Log::StrToLevel(std::string &level)
{
  int iRet = -1;
  LogLevelIter_t tIter = namemap_.find(level);
  if (namemap_.end() != tIter)
  {
    iRet = tIter->second;
  }

  return iRet;
}

Log& Log::GetInstance()
{
  return instance_;
}

void Log::log(const char *p_szFile, int p_iLine, int p_iLevel, const char * p_szFmt, ...)
{
  va_list tBlah;
  va_start(tBlah, p_szFmt);

  Log &logger = GetInstance();
  int iLevel = logger.GetLevel();
  FILE *file = logger.GetFile();

  if (p_iLevel <= iLevel && NULL != file)
  {
    time_t tNow = time(NULL);
    std::ostringstream oSS;
    oSS << (int) tNow
         << " "
         << p_szFile
         << " ["
         << p_iLine
         << "]: "
         << logger.LevelToStr(p_iLevel)
         << ": ";
    fprintf(file, "%s", oSS.str().c_str());
    vfprintf(file, p_szFmt, tBlah);
    fflush(file);
  }
}

void Log::sysLog(const char *p_szFile, int p_iLine, const char * p_szFmt, ...)
{
  va_list tBlah;
  va_start(tBlah, p_szFmt);

  std::ostringstream oSS;
  oSS << p_szFile
      << " ["
      << p_iLine
      << "]: "
      << p_szFmt;
  vsyslog(LOG_NOTICE, oSS.str().c_str(), tBlah);
}


