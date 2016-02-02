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
 *  File: Config.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 21, 2014
 *
 */


#ifndef _CONFIG_H
#define _CONFIG_H

// System
#include <list>
#include <map>

// Shared

// Module
#include <boost/noncopyable.hpp>


class Config : boost::noncopyable
{
  public:
    Config();
    virtual ~Config();

    int Load(char *file);
    int Load(const char *file);

    const char *GetValue(char *key);
    const char *GetValue(const char *key);

    const char *GetError();

    // Enums and Typedefs
    typedef std::list<std::string> StringList_t;
    typedef StringList_t::iterator StringListIter_t;

    typedef std::map<std::string, StringList_t> ConfigMap_t;
    typedef ConfigMap_t::iterator               ConfigMapIter_t;

  private:
    Config(Config &config);
    Config &operator=(Config &config);

    char *Trim(char *line, int maxlen);
    void Chomp(char *line, int maxlen);

    char *file_;
    const char *error_;
    ConfigMap_t configmap_;
};

#endif // CONFIG_H
