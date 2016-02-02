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
 *  File: Config.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 21, 2014
 *
 */


// System
#include <stdio.h>
#include <string.h>


// Shared

// Module
#include "Config.h"

#define CONFIG_LINE_LEN 1024

Config::Config()
  : file_(NULL),
    error_(NULL)
{
}

Config::~Config()
{
  if (file_ != NULL)
  {
    delete[] file_;
    file_ = NULL;
  }
}

int Config::Load(char *file)
{
  int iRet = 0;
  FILE *pFile = NULL;

  // Make sure the user specified a file.
  if (file == NULL)
  {
    error_ = "No config file specified";
  }
  // Try to open it for reading.
  else if ((pFile = fopen(file, "r")) == NULL)
  {
    error_ = "Unable to open file";
  }
  else
  {
    // If the file was opened, copy the file parameter
    // into a member variable.
    int len = strlen(file);
    file_ = new char[len + 1];
    memset(file_, 0, len + 1);
    strncpy(file_, file, len);

    char line[CONFIG_LINE_LEN + 1];
    char *ret = NULL;
    memset(line, 0, CONFIG_LINE_LEN + 1);

    configmap_.clear();

    // Get the first line of the file
    while ((ret = fgets(line, CONFIG_LINE_LEN, pFile)) != NULL)
    {
      // If this was NOT a comment line...
      if (line[0] != '#')
      {
        // Look for the first '=' (as the delimiter).
        char *szEq = index(line, '=');
        // If there was a delimiter, then this is a valid line.
        if (NULL != szEq)
        {
          // Point at the second token as the value.
          char *szValue = &(szEq[1]);
          // change the delimiter to be a NULL so we have 2 strings now (key and value).
          szEq[0] = '\0';

          // Get the string lengths.
          int iKeyLen = CONFIG_LINE_LEN - (int) (szEq - line);
          int iValLen = CONFIG_LINE_LEN - (int) (szValue - ret);

          // Get rid of trailing spaces.
          Chomp(szValue, iValLen);

          // Trim the key and values of uneeded white space.
          char *szKey = Trim(line, iKeyLen);
          szValue = Trim(szValue, iValLen);

          // Add the key and value into the member map.
          configmap_[szKey].push_back(szValue);
        }
      }
      // Reset the line.
      memset(line, 0, CONFIG_LINE_LEN + 1);
    }

    // It worked...
    iRet = 1;

    // Close the file.
    fclose(pFile);
  }

  // Return the status.
  return iRet;
}

int Config::Load(const char *file)
{
  return Load((char *) file);
}

const char *Config::GetValue(char *key)
{
  return GetValue((const char *) key);
}

const char *Config::GetValue(const char *key)
{
  const char *ret = NULL;

  // Make sure a key was specified.
  if (key != NULL)
  {
    // Find if there is a value.
    ConfigMapIter_t tIter = configmap_.find(key);

    // If we found a value...
    if (configmap_.end() != tIter)
    {
      // Return it as a c-string.
     ret = tIter->second.front().c_str();
    }
  }

  // Return whatever we found.
  return ret;
}

void Config::Chomp(char *line, int maxlen)
{
  // If we have a line...
  if (line != NULL)
  {
    int i = 0;
    // Find the length.
    for (i = 0; line[i] != '\0' && i < maxlen; i++) {}

    // If i makes sense...
    if (i < maxlen)
    {
      // Go from the end to the beginning turning \r and \n into \0 until
      // we see the first non-\r\n char.
      while (--i >= 0 && ('\n' == line[i] || '\r' == line[i]))
      {
        line[i] = '\0';
      }
    }
  }
}

char *Config::Trim(char *line, int maxlen)
{
  char *ret = NULL;

  // If we have an input line...
  if (line != NULL)
  {
    char *pLastChar = NULL;
    int i = 0;

    // From the beginning, look for leading whitespace...
    for (i = 0; line[i] != '\0' && i < maxlen; i++)
    {
      // If we haven't found the first non-whitespace char
      // yet, and this is not whitespace...
      if (NULL == pLastChar
          && ' ' != line[i]
          && '\t' != line[i])
      {
        // Set this as the head of the trimmed string
        ret = &(line[i]);
        pLastChar = ret;
      }
      else if (' ' != line[i]
               && '\t' != line[i])
      {
        pLastChar = &(line[i]);
      }
    }

    // After the loop, if we have any trailing characters that are
    // not pointed to, then they must be whitespace, so we insert a
    // NULL to chop them off.
    if (i < maxlen)
    {
      pLastChar[1] = '\0';
    }
  }

  return ret;
}




