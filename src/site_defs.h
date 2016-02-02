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
 *  File: site_defs.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 23, 2014
 *
 */

#ifndef _SITE_DEFS_H
#define _SITE_DEFS_H

#include <string>
#include <list>

// Config options

// Defines if interactive mode should be enabled or not
#define CONFIG_INTERACTIVE_MODE "simsearch_interactive_mode"

// Defines the directory with documents
#define CONFIG_DOCUMENT_DIRECTORY "simsearch_document_directory"

// Defines the directory where encrypted files are stored
#define CONFIG_OUTPUT_DIRECTORY "simsearch_output_directory"

// Defines the file for log output
#define CONFIG_LOG_FILE "simsearch_log_file"

// Defines the log level
#define CONFIG_LOG_LEVEL "simsearch_log_level"

// TODO ??
#define CONFIG_WORD_RANDOM_NUMBER "simsearch_word_random_number"

// Defines the number of threads in the pool
#define CONFIG_THREAD_POOL_NUMBER "simsearch_thread_number"

// libtomcrypt key for encryption
// TODO: make LTCManager generate pseudorandom password
#define LTC_HMAC_KEY "supersecretkey"

// HElib default security
#define HE_PARAM_K 80

// HElib default # of primes in the modulus chain
#define HE_PARAM_L 9

// HElib default number of columns in key switching matrices
#define HE_PARAM_C 2

// HElib default plaintext base
#define HE_PARAM_P 90313

// HElib default lifting
#define HE_PARAM_R 1

// HElib default degree of the field extension
#define HE_PARAM_D 0

// HElib default min number of plaintext slots
#define HE_PARAM_S 0

// HElib default Hamming weight of secret key
#define HE_PARAM_W 64

// Queue default size
#define QUEUE_MAX_ELEMENTS 10000

// Default parsing elements
const std::string character_literals = "\\\n\t\r\f\v\a\b\?\'\"\0";
const std::string keyboard_delimiters = "`~!@#$%^&*()[] .,;:_-=|+<>/";
const std::string english_alphabet[] = {	"a", "b", "c", "d", "e", "f", "g", "h",
											"i", "j", "k", "l", "m", "n", "o", "p",
											"q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
const std::string english_common_words[] = {	"the", "be", "to", "of", "and", "a",
												"in", "that", "have", "I", "it", "for", "not",
												"on", "with", "he", "as", "you", "do", "at",
												"this", "but", "his", "by", "from"};

// Define the length of the word.
// Ignore all words shorter or longer than specified length.
#define CHOMP_WORD_MIN_LENGTH 2
#define CHOMP_WORD_MAX_LENGTH 20

// Define encrypted TF index file extension
// For example, it original document is called "document1",
// an encrypted TF index would be document1.tfi
#define ENCR_TF_INDEX_FILE_EXTENSION ".tfi"

// Define encrypted similarity result file extension
#define ENCR_SIM_RESULT_FILE_EXTENSION ".simresult"

// Define SSE2 index filename
#define SSE2_INDEX_FILENAME "SSE2.index"

// Define dictionary filename
#define DICTIONARY_FILENAME "Dictionary.index"

typedef enum
{
  READ_CONTENT_EVENT = 0,
  SIMILARITY_MEASURE_EVENT
} task_event_type_e;


#endif
