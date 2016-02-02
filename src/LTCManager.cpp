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
 *  File: LTCManager.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Nov 17, 2014
 *
 */

// System
#include <sstream>
#include <string>
#include <iostream>

// Shared
#include "site_defs.h"

// Module
#include <boost/lexical_cast.hpp>
#include "LTCManager.h"
#include "Log.h"

#define LTCMANAGER_SHA256 "sha256"

LTCManager::LTCManager()
{
}

LTCManager::~LTCManager()
{
}

bool LTCManager::Init()
{
  elog(DEBUG, "LTC initialization begins.\n");

  // register SHA-256
  if (register_hash(&sha256_desc) == -1)
  {
    elog(ERROR, "Error registering '%s'\n", LTCMANAGER_SHA256);
    return false;
  }

  elog(DEBUG, "LTC initialization ends.\n");

  return true;
}

bool LTCManager::GenerateKey()
{
  elog(DEBUG, "Generation of secret key for LTC begins.\n");

  int errno;
  unsigned char tmpkey[MAXBLOCKSIZE];
  unsigned char key[MAXBLOCKSIZE];
  unsigned long outlen = 0;
  int hash_idx = 0;

  // Find hash
  hash_idx = find_hash(LTCMANAGER_SHA256);
  if (hash_idx == -1)
  {
    elog(ERROR, "Unable to find hash for '%s'\n", LTCMANAGER_SHA256);
    return false;
  }

  // Get the size of key
  outlen = sizeof(key);

  // Get the hmac key from site_defs
  memset(tmpkey, '\0', sizeof(tmpkey));
  memcpy(tmpkey, LTC_HMAC_KEY, sizeof(LTC_HMAC_KEY));

  // Generate key
  if ((errno = hash_memory(hash_idx,
                           tmpkey,
                           strlen((char *)tmpkey),
                           key,
                           &outlen)) != CRYPT_OK)
  {
    elog(ERROR, "Hashing error: '%s'\n", error_to_string(errno));
    return false;
  }

  // Store newly generated key and its length
  memset(secretkey_, '\0', sizeof(secretkey_));
  memcpy(secretkey_, key, outlen);
  secretkeylength_ = outlen;

  elog(DEBUG, "Generation of secret key for LTC ends.\n");

  return true;
}

void LTCManager::PrintKey()
{
  elog(DEBUG, "Lenght of key is '%lu'\n", secretkeylength_);

  elog(DEBUG, "Printing hex of secretkey: ");

  for (int i = 0; i < MAXBLOCKSIZE; i++)
  {
    printf("0x%02x ", secretkey_[i]);
  }
  printf("\n");
}

bool LTCManager::CalculateHMAC(const std::string& stringinput, std::string& stringciphtertext)
{
  int err;
  hmac_state hmac;
  unsigned char dst[MAXBLOCKSIZE];

  unsigned long dstlen;
  int hash_idx = 0;

  if (stringinput.empty() == true)
  {
    elog(ERROR, "String input is empty\n");
    return false;
  }

  // Register SHA-256
  if (register_hash(&sha256_desc) == -1)
  {
    elog(ERROR, "Unable to register '%s'\n", LTCMANAGER_SHA256);
    return false;
  }

  // Find hash
  hash_idx = find_hash(LTCMANAGER_SHA256);
  if (hash_idx == -1)
  {
    elog(ERROR, "Unable to find hash for '%s'\n", LTCMANAGER_SHA256);
    return false;
  }

  // Start the HMAC
  if ((err = hmac_init(&hmac, hash_idx, secretkey_, secretkeylength_)) != CRYPT_OK)
  {
    elog(ERROR, "Error setting up HMAC: %s\n", error_to_string(err));
    return false;
  }

  // Process hmac on given string input
  if ((err = hmac_process(&hmac,
                          (unsigned char*)(stringinput.c_str()),
                          stringinput.length()) != CRYPT_OK))
  {
    elog(ERROR, "Error processing hmac: %s\n", error_to_string(err));
    return false;
  }

  // Get the result
  dstlen = sizeof(dst);
  memset(dst, '\0', sizeof(dst));

  if ((err = hmac_done(&hmac, dst, &dstlen)) != CRYPT_OK)
  {
    elog(ERROR, "Error finishing hmac: %s\n", error_to_string(err));
    return false;
  }

  // Convert dst ciphertext to hexstring, update by reference
  if (ConvertCiphtertextToHexString(dst, dstlen, stringciphtertext) == false)
  {
    elog(ERROR, "Unable to convert to hex ciphertext\n");
    return false;
  }

  return true;
}

bool LTCManager::ConvertCiphtertextToHexString(const unsigned char *in,
                                                 const unsigned long length,
                                                 std::string& hexciphtertext)
{
  if (length == 0)
  {
    elog(ERROR, "Length of converting array is zero\n");
    return false;
  }

  if (in == NULL)
  {
    elog(ERROR, "Array is not allocated\n");
    return false;
  }

  std::stringstream ss;

  char hex[2];
  for(unsigned long i = 0; i < length; i++)
  {
    // Output each value of array as hex
    snprintf(hex, sizeof(hex), "%02X", in[i]);

    // Add to stringstream
    ss << hex;
  }

  // Update ciphtertext
  hexciphtertext = ss.str();

  return true;
}




