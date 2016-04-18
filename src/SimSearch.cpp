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
 *  File: main.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 1, 2014 
 *
 */

// System
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <signal.h>

// Shared
#include "site_defs.h"

// Module
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include "FileManager.h"
#include "IndexBuildEngine.h"
#include "SearchEngine.h"
#include "Config.h"
#include "Log.h"
#include "HEManager.h"
#include "LTCManager.h"
#include "ThreadPool.h"

void Version()
{
  fprintf(stdout, "\n simsearch 0.01 \n");
}

void Usage()
{
  Version();
  printf("simsearch [ OPTIONS ]\n");
  printf("\n");
  printf("OPTIONS:\n");
  printf("      -c <config file>          This file overrides the default location\n");
  printf("                                of the config file (./config.conf)\n");
  printf("      -p <pid file>             a file to contain the PID of the daemon process.\n");
  printf("      -o <log file>             a file to contain the output of the daemon process.\n");
  printf("      -g                        Run daemon in the foreground.\n");
  printf("      -u <user>                 Drop permissions to this user after init.\n");
  printf("      -l 'DEBUG'                The output message logging level\n");
  printf("         | 'INFO'\n");
  printf("         | 'WARNING'\n");
  printf("         | 'ERROR'\n");
  printf("         | 'CRITICAL'\n");
  printf("      -v                        Version information.\n");
  printf("      -h                        This message.\n");
}

bool SetupLogging(boost::shared_ptr<Config> mConfig)
{
  bool bRet = true;

  const char *ConfLogFile = mConfig->GetValue(CONFIG_LOG_FILE);
  if (ConfLogFile != NULL && Log::GetInstance().GetFileName() == "")
  {
    if (!(Log::GetInstance().SetFileName(ConfLogFile)))
    {
      elog(CRITICAL, "Unable to set file '%s' as output file, using stderr\n", ConfLogFile);
      bRet = false;
    }
  }

  const char *ConfLogLevel = mConfig->GetValue(CONFIG_LOG_LEVEL);
  if (ConfLogLevel != NULL)
  {
    std::string sLevel = ConfLogLevel;
    int iLogLevel = Log::GetInstance().StrToLevel(sLevel);
    if (iLogLevel == -1)
    {
      elog(CRITICAL, "Unable to set log level '%s', using stderr\n", ConfLogLevel);
      bRet = false;
    }
    else
    {
      Log::GetInstance().SetLevel(iLogLevel);
    }
  }

  return bRet;
}

int main(int argc, char **argv)
{
  std::string ConfigFile = "simsearch_config.conf";
  int LogLevel = CRITICAL;
  std::string sLevel;
  bool daemon = false;
  std::string EncrIndexDirectory;
  std::string document_directory;
  int thread_pool_number = 0;

  const char* szInteractive = NULL;
  int interactive_mode = -1;

  int c;
  while ((c = getopt(argc, argv, "c:p:o:l:dhv")) != -1)
    {
      switch (c)
      {
        case 'c':
          ConfigFile = optarg;
          break;
        case 'o':
          if (!(Log::GetInstance().SetFileName(optarg)))
          {
            elog(CRITICAL, "Unable to set file '%s' as output file, using stderr\n", optarg);
          }
          break;
        case 'd':
          daemon = true;
          break;
        case 'h':
          Usage();
          exit(EXIT_FAILURE);
          break;
        case 'v':
          Version();
          exit(EXIT_SUCCESS);
          break;
        case 'l':
          sLevel = optarg;
          LogLevel = Log::GetInstance().StrToLevel(sLevel);
          if (LogLevel == -1)
          {
            elog(CRITICAL, "Unknown log level '%s', leaving at CRITICAL\n", sLevel.c_str());
            LogLevel = CRITICAL;
          }
          break;
        case '?':
        default:
          elog(CRITICAL, "Unable to parse command line arguments\n");
          break;
      }
  }

  // Setup logging
  Log::GetInstance().SetLevel(LogLevel);

  // Setup config reader
  boost::shared_ptr<Config> mConfig = boost::make_shared<Config>();
  if (!mConfig->Load(ConfigFile.c_str()))
  {
    elog(CRITICAL, "Unable to load config file '%s'.\n", ConfigFile.c_str());
  }
  else
  {
      if (SetupLogging(mConfig) == false)
      {
        elog(CRITICAL, "Unable to initialize logging.\n");
      }
  }

  if (daemon == true)
  {
    // TODO: create daemon method
  }

  // Get the interactive mode value
  szInteractive = mConfig->GetValue(CONFIG_INTERACTIVE_MODE);
  if (szInteractive == NULL)
  {
    elog(CRITICAL, "Unable to read interactive mode value in configuration file.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    std::string mode_string = szInteractive;
    interactive_mode = boost::lexical_cast<int>(mode_string);
  }

  const char* output = mConfig->GetValue(CONFIG_OUTPUT_DIRECTORY);
  if (output == NULL)
  {
	elog(CRITICAL, "Unable to read output directory option in configuration file.\n");
	exit(EXIT_FAILURE);
  }
  else
  {
    EncrIndexDirectory = output;
  }

  const char* thread_pool_char = mConfig->GetValue(CONFIG_THREAD_POOL_NUMBER);
  if (thread_pool_char == NULL)
  {
	elog(CRITICAL, "Unable to read thread number option in configuration file.\n");
	exit(EXIT_FAILURE);
  }
  else
  {
	std::string thread_pool_string = thread_pool_char;
	thread_pool_number = boost::lexical_cast<int>(thread_pool_string);
  }

  // Filemanager reader/writer
  boost::shared_ptr<FileManager> pFileManager = boost::make_shared<FileManager>();

  // LTC manager
  boost::shared_ptr<LTCManager> pLTCManager = boost::make_shared<LTCManager>();

  // Init
  if (pLTCManager->Init() == false)
  {
    elog(CRITICAL, "Unable to initialize LTC library.\n");
    exit(EXIT_FAILURE);
  }

  if (pLTCManager->GenerateKey() == false)
  {
    elog(CRITICAL, "Unable to generate keys for LTC library.\n");
    exit(EXIT_FAILURE);
  }

  // Init HE manager
  // TODO: check return values for Init and GenerateKeys
  boost::shared_ptr<HEManager> pHEManager = boost::make_shared<HEManager>();
  pHEManager->Init();

  // Setup Thread Pool
  boost::shared_ptr<ThreadPool> pPool = boost::make_shared<ThreadPool>();
  pPool->SetSize(thread_pool_number);
  elog(DEBUG, "Thread pool size is '%d'.\n", thread_pool_number);

  elog(DEBUG, "Initializing pool...\n");
  if (pPool->Init() == false)
  {
	elog(CRITICAL, "Could not initialize pool.\n");
	exit(EXIT_FAILURE);
  }

  // Setup Queue and QueueManager

  boost::shared_ptr<EventHandler> pEventHandler = boost::make_shared<EventHandler>();

  // Create queue
  boost::shared_ptr<EventQueue> pEventQueue = boost::make_shared<EventQueue>(pEventHandler);

  if (pEventQueue->Init(QUEUE_MAX_ELEMENTS) == false)
  {
    elog(CRITICAL, "Unable to create event queue with size '%d'.\n", QUEUE_MAX_ELEMENTS);
    return false;
  }

  // Launch event queue
  if (pEventQueue->Start() == false)
  {
    elog(CRITICAL, "Unable to start event queue.\n");
    return false;
  }

  // Check interactive mode value from configuration file
  if (interactive_mode < 0)
  {
    elog(CRITICAL, "Unable to setup interactive mode.\n");
    exit(EXIT_FAILURE);
  }
  else
  if (interactive_mode == 0)
  {
    elog(INFO, "Entering non-interactive mode.\n");

    {
      // Read directory from file, generate random words, search it and display the results
      const char* dir = mConfig->GetValue(CONFIG_DOCUMENT_DIRECTORY);
      if (dir == NULL)
      {
        elog(CRITICAL, "Unable to read document directory value in configuration file.\n");
        exit(EXIT_FAILURE);
      }

      // Start the simsearch
      std::string directory = dir;

      boost::scoped_ptr<IndexBuildEngine> oIndexBuilder(new IndexBuildEngine(directory,
                                                                           EncrIndexDirectory,
                                                                           pFileManager,
                                                                           pLTCManager,
                                                                           pHEManager,
                                                                           pPool,
                                                                           pEventQueue));

      if (oIndexBuilder == NULL)
      {
        std::cout << "Could not allocate memory for IndexBuilder engine.\n";
        exit(EXIT_FAILURE);
      }

      // Init IndexBuilder
      oIndexBuilder->Init();

      // Fire up the search service
      oIndexBuilder->Startup();
    }

    {
      boost::scoped_ptr<SearchEngine> oSearch(new SearchEngine(EncrIndexDirectory,
                                                             pFileManager,
                                                             pLTCManager,
                                                             pHEManager,
                                                             pPool,
                                                             pEventQueue));

      // Init search engine
      oSearch->Init();

      //std::string word_input = "ospf bgp mime xml iscsi";
      //std::string word_input = "certificate encryption authorization authentication framework";
    
      // Get the number of words to randomly generate in search query
      std::vector<int> query_random_size{ 5,10,15};

      // Iterate on each value in random vector
      for (const auto& num : query_random_size)
      {
        std::string word_input;
        // Generate random words and put result a string
        if (oSearch->GenRandomSearchQuery(num, word_input) == false)
	{ 
	  elog(ERROR, "Unable to generate random search query.\n");
	  continue;
	}
      
	elog(DEBUG, "Number of generated RANDOM words: '%d'.\n", num);

        // Execute search
        if (oSearch->SearchIndex(word_input) == false)
        {
          elog(ERROR, "Unable to execute search.\n");
        }
        else
        {
          oSearch->PrintResults();
        }
      }
    }
/*

    // Get the number of words to randomly generate in search query
    const char* word_rnd_number = mConfig->GetValue(CONFIG_WORD_RANDOM_NUMBER);
    if (word_rnd_number == NULL)
    {
      elog(CRITICAL, "Unable to read word random number value in configuration file.\n");
      exit(EXIT_FAILURE);
    }

    // Generate random words and put result a string
    std::string word_rnd_number_string = word_rnd_number;
    std::string random_words;
    ss.GenRandomSearchQuery(boost::lexical_cast<int>(word_rnd_number_string), random_words);

    // Search it!
    ss.SearchIndex(random_words);


    // Print retuls
    ss.PrintResults();
*/
  }
  else
  {
    elog(INFO, "Entering interactive mode.\n");

    int choice = 1;
    std::string choise_str;
    std::string directory, word_input;
    std::string random_counter;

    while (choice != 0)
    {
      std::cout<<"\n----------------------------------------------\n";
      std::cout<<"0. Exit the application.\n";
      std::cout<<"1. Import a directory with documents to create searchable index.\n";
      std::cout<<"2. Input a set of words to search the index.\n";
      std::cout<<"3. Randomly generate a set of words to search the index.\n";
      std::cout<<"----------------------------------------------\n";
      std::cout<<"Select : ";

      // TODO: remove adhoc code
      std::getline(std::cin, choise_str);
      std::istringstream string_stream(choise_str);
      string_stream >> choice;

      if (choice == 1)
      {
        std::cout << "Type your directory (full path) with documents: ";
        std::getline (std::cin, directory);
        std::cout << "Reading the content of " << directory << ".\n";

        boost::scoped_ptr<IndexBuildEngine> oIndexBuilder(new IndexBuildEngine(directory,
                                                                               EncrIndexDirectory,
                                                                               pFileManager,
                                                                               pLTCManager,
                                                                               pHEManager,
                                                                               pPool,
                                                                               pEventQueue));

        if (oIndexBuilder == NULL)
        {
          std::cout << "Could not allocate memory for IndexBuilder engine.\n";
          exit(EXIT_FAILURE);
        }

        // Init IndexBuilder
        oIndexBuilder->Init();

        // Fire up the search service
        oIndexBuilder->Startup();

        std::cout << "Done!\n";
      }
      else if (choice == 2)
      {
        word_input.clear();
        std::cout << "Enter the set of words: ";
        std::getline (std::cin, word_input);

        if (word_input.empty() == true)
        {
          std::cout << "Input is empty.\n";
        }
        else
        {
          std::cout << "Searching for keywords '" << word_input << "'.\n";
          boost::scoped_ptr<SearchEngine> oSearch(new SearchEngine(EncrIndexDirectory,
                                                                   pFileManager,
                                                                   pLTCManager,
                                                                   pHEManager,
                                                                   pPool,
                                                                   pEventQueue));

          // Init search engine
          oSearch->Init();

          // Execute search
          if (oSearch->SearchIndex(word_input) == false)
          {
            elog(ERROR, "Unable to execute search.\n");
          }
          else
          {
            oSearch->PrintResults();
          }
        }
      }
      else if (choice == 3)
      {
        std::cout << "This feature is under construction \n";
/*
        if (!ss)
        {

          std::cout << "Please import a directory with documents.\n";
        }
        else
        {
          word_input.clear();
          random_counter.clear();
          std::cout << "Please provide number of random words in the search query: ";
          std::getline (std::cin, random_counter);

          // Place random words in word_input
          ss->GenRandomSearchQuery(boost::lexical_cast<int>(random_counter), word_input);

          //std::cout << "Randomly generated query: '" << word_input << "'\n";

          // Search!
          ss->SearchIndex(word_input);

          // Print!
          ss->PrintResults();
        }
*/
      }

    }

  } // if-else

  elog(DEBUG, "Killing pool.\n");
  pPool->Kill();
  elog(DEBUG, "Done!\n");

  elog(DEBUG, "Killing queue.\n");
  pEventQueue->Kill();
  elog(DEBUG, "Done!\n");

  return EXIT_SUCCESS;
}

