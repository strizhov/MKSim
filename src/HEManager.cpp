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
 *  File: HEManager.cpp
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 23, 2014
 *
 */

// System

// Shared
#include "site_defs.h"

// Module
#include "HEManager.h"
#include "Log.h"

#define CTXT_NULL_PAD 0

//#define DEBUG_LOG

HEManager::HEManager()
  : m_(0),
    k_(HE_PARAM_K),
    L_(HE_PARAM_L),
    c_(HE_PARAM_C),
    p_(HE_PARAM_P),
    r_(HE_PARAM_R),
    d_(HE_PARAM_D),
    s_(HE_PARAM_S),
    w_(HE_PARAM_W)
{
}

HEManager::~HEManager()
{
}

void HEManager::Init()
{
#ifdef DEBUG_LOG
  elog(DEBUG, "HE initialization begins.\n");
#endif

  // Find M
  m_ = FindM(k_, L_, c_, p_, d_, s_, m_, false);

  // Initialize context
  FHEcontext context(m_, p_, r_);

  // Modify the context, adding primes to the modulus chain
  buildModChain(context, L_, c_);

#ifdef DEBUG_LOG
  elog(DEBUG, "HE initialization ends.\n");
#endif

#ifdef DEBUG_LOG
  elog(DEBUG, "Generation of public and private keys for HE begins.\n");
#endif

  // construct a secret key structure
  FHESecKey secretKey(context);

  // an "upcast": FHESecKey is a subclass of FHEPubKey
  const FHEPubKey& publicKey = secretKey;

  // Construct ZZX
  if (d_ == 0)
    G_ = context.alMod.getFactorsOverZZ()[0];
  else
    G_ = makeIrredPoly(p_, d_);

  // actually generate a secret key with Hamming weight w
  secretKey.GenSecKey(w_);

  // compute key-switching matrices
  addSome1DMatrices(secretKey);

  // Write context down and keep it
  writeContextBase(stream_context_, context);
  stream_context_ << context;

  // Write down secret and public keys
  stream_secretkey_ << secretKey;
  stream_publickey_ << publicKey;

#ifdef DEBUG_LOG
  elog(DEBUG, "Generation of public and private keys for HE ends.\n");
#endif

/*
 * READ THE CONTEXT
  unsigned long m, p, r;
  std::vector<long> gens, ords;
  readContextBase(stream_context_, m, p, r, gens, ords);
   FHEcontext context(m, p, r, gens, ords);
  stream_context_ >> context;
  stream_context_.seekg(0);
*/

}

void HEManager::ClearKeys()
{
  stream_secretkey_.flush();
  stream_publickey_.flush();
}

// Modify the secretkey by reference
bool HEManager::GetSecretKey(FHESecKey& key)
{
  // TODO: test the context of secretkey:
  // If context is null then return false

  stream_secretkey_ >> key;
  stream_secretkey_.seekg(0);

  return true;
}

// Modify the publickey by reference
bool HEManager::GetPublicKey(FHEPubKey& key)
{
  // TODO: test the context of publickey:
  // If context is null then return false

  stream_publickey_ >> key;
  stream_publickey_.seekg(0);

  return true;
}

bool HEManager::EncryptArray(const std::vector<long>& vector,
                             std::vector<std::string>& encr_vector)
{
#ifdef DEBUG_LOG
  elog(DEBUG, "HE Encryption begins.\n");
#endif

  // Sanity check
  if (vector.empty())
  {
    elog(DEBUG,"Array to encrypt is empty\n");
    return false;
  }

  // Read context and constuct FHEcontext
  unsigned long m1, p1, r1;
  std::vector<long> gens, ords;
  readContextBase(stream_context_, m1, p1, r1, gens,ords);

  FHEcontext context(m1, p1, r1, gens, ords);
  stream_context_ >> context;
  stream_context_.seekg(0);

#ifdef DEBUG_LOG
  elog(DEBUG, "m1 is '%ld', p1 is '%ld', r1 is '%ld'.\n", m1, p1, r1);
#endif

  // constuct an Encrypted array object ea that is
  // associated with the given context and the polynomial G
  EncryptedArray ea(context, G_);

  long nslots = ea.size();
  long vector_size = static_cast<long>(vector.size());

#ifdef DEBUG_LOG
  elog(DEBUG, "Vector size: %ld, number of slots is %ld\n", vector_size, nslots);
#endif

  // Get public key
  FHEPubKey publicKey(context);
  if (GetPublicKey(publicKey) == false)
  {
    elog(ERROR, "Unable to retireve public key for encryption.\n");
    return false;
  }

  // Create a ciphertext with publicKey
  Ctxt ciphertext(publicKey);

  // Check if vector size is equal or larger than number of slots in HE
  if (vector_size >= nslots)
  {
    /*
     * Since the number of slots is usually smaller that the size ot vector,
     * we split the vector to nslots vector.
     * For example: if there are 1013 elements in vector and nslots is equal to 25
     * the division quotient will be 40, the reminder will be 13.
     * We will split vector into 40 slices of nslots size and
     * add 41-st slice which will be the reminder of 25 elements in vector.
     */

    // First, fine division quotient and reminder
    long quotient = vector_size / nslots;
    long reminder = vector_size % nslots;

#ifdef DEBUG_LOG
    elog(DEBUG, "Division quotient: %ld, reminder: %ld\n", quotient, reminder);
#endif

    // Clear Ctxt
    ciphertext.clear();

    // Include even number of elements in vector, increase counter by nslots
    long i = 0;
    while (i < (quotient * nslots))
    {
      // Construct single ciphertext, fill the ciphertext by reference
      if (ConstructSingleCipehrtext(vector,
                                    i,
                                    (i + nslots),
                                    ea,
                                    publicKey,
                                    ciphertext) == false)
      {
        elog(ERROR, "Unable to encrypt the term weight vector\n.");
	return false;
      }
      else
      {
        std::string ctxt_string;
        PackCiphertextToString(ciphertext, ctxt_string);
        encr_vector.emplace_back(ctxt_string);

        // Increment i by number of slots
        i = i + nslots;
      }
    }

    // Now check if there is a reminder left
    if (reminder > 0)
    {
      // Find the last quotient element to start with
      long start = quotient * nslots;
      long end = start + reminder;

      // Clear Ctxt
      ciphertext.clear();
      
      // Construct single ciphertext, fill the ciphertext
      if (ConstructSingleCipehrtext(vector,
                                    start,
                                    end,
                                    ea,
                                    publicKey,
                                    ciphertext) == false)
      {
        elog(ERROR, "Unable to encrypt the remainder of term weight vector\n.");
        return false;
      }
      else
      {
        std::string ctxt_string;
        PackCiphertextToString(ciphertext, ctxt_string);
        encr_vector.emplace_back(ctxt_string);
      }
    }
  }
  else // Size of vector is less than number of slots
  {
#ifdef DEBUG_LOG
    elog(DEBUG, "Vector size of smalled than nslots.\n");
#endif

    // Clear ciphertext
    ciphertext.clear();

    // Construct single ciphertext, fill the ciphertext
    if (ConstructSingleCipehrtext(vector,
                                  0,
                                  vector_size,
                                  ea,
                                  publicKey,
                                  ciphertext) == false)
    {
      elog(ERROR, "Unable to encrypt the remainder of term weight vector\n.");
      return false;
    }
    else
    {
      std::string ctxt_string;
      PackCiphertextToString(ciphertext, ctxt_string);
      encr_vector.emplace_back(ctxt_string);
    }
  }

#ifdef DEBUG_LOG
  elog(DEBUG, "HE Encryption ends. Size of encrypted vector: %d\n", encr_vector.size());
#endif

  return true;
}

bool HEManager::ConstructSingleCipehrtext(const std::vector<long>& vector,
                                          const long& start,
                                          const long& end,
                                          const EncryptedArray& ea,
                                          const FHEPubKey& publicKey,
                                          Ctxt& ciphertext)
{

  long nslots = ea.size();
#ifdef DEBUG_LOG
  elog(DEBUG, "Constructing single ciphertext. Vector Start: %ld. Vector End: %ld. HE nslots: %ld.\n", start, end, nslots);
#endif

  if (vector.empty() == true)
  {
    elog(ERROR, "Term weight vector is empty.\n");
    return false;
  }

  // Allocate vector of size nslots and fill up with NULLs
  std::vector<long> v0;

  // Push (end-start) elements first
  v0.insert(v0.begin(), vector.begin() + start, vector.begin() + end);

  // Calc the position
  long num_items = (end - start);
  if (num_items < nslots)
  {
    // Get the number of null elements to insert
    long to_add_counter = (nslots - num_items);

    // Insert as many as needed at the end of vector
    v0.insert (v0.end(), to_add_counter, CTXT_NULL_PAD);
  }

  // Create HE plaintext
  PlaintextArray p0(ea);
  p0.encode(v0);

  // Encrypt with public key, the result is stored in Ctxt object
  ea.encrypt(ciphertext, publicKey, p0);

  return true;
}

bool HEManager::CalculateSimilarity(const std::vector<std::string>& encr_term_weight,
                                    const std::vector<std::string>& encr_search_query,
                                    std::vector<std::string>& encr_similarity_coefficient)
{
  if (encr_term_weight.empty() == true)
  {
    elog(ERROR, "Encrypted term weight vector is empty!\n");
    return false;
  }

  if (encr_search_query.empty() == true)
  {
    elog(ERROR, "Encrypted search query vector is empty!\n");
    return false;
  }

  if (encr_similarity_coefficient.empty() == false)
  {
    elog(ERROR, "Encrypted similarity coefficient vector is not empty!\n");
    return false;
  }

  // Consistency check
  if (encr_term_weight.size() != encr_search_query.size())
  {
    elog(ERROR, "Size of encrypted term weight vector '%d' is not equal to size of encrypted search query '%d'.\n",
         encr_term_weight.size(),
         encr_search_query.size());
    return false;
  }

  // Read context and constuct FHEcontext
  unsigned long m1, p1, r1;
  std::vector<long> gens, ords;
  readContextBase(stream_context_, m1, p1, r1, gens, ords);
  FHEcontext context(m1, p1, r1, gens, ords);
  stream_context_ >> context;
  stream_context_.seekg(0);

  // Get public key
  FHEPubKey publicKey(context);
  if (GetPublicKey(publicKey) == false)
  {
    elog(ERROR, "Unable to retireve public key for encryption.\n");
    return false;
  }

  Ctxt sq_ciphertext(publicKey);
  Ctxt result_ciphertext(publicKey);

  for (unsigned i = 0; i < encr_term_weight.size(); i++)
  {
    // Clear the content
    sq_ciphertext.clear();
    result_ciphertext.clear();

    // Unpack ciphertext
    UnpackCiphertextFromString(encr_term_weight[i], result_ciphertext);
    UnpackCiphertextFromString(encr_search_query[i], sq_ciphertext);

    // Multiply item of one vector to the item of other vector, keeping iterator the same
    result_ciphertext.multiplyBy(sq_ciphertext);

    // Pack result ciphtertext back to string
    std::string result_string;
    PackCiphertextToString(result_ciphertext, result_string);

    // Push back resulted ciphertext array
    encr_similarity_coefficient.emplace_back(result_string);
  }

  return true;
}

bool HEManager::DecryptArray(const std::vector<std::string>& encr_vector, std::vector<long>& decr_vector)
{
  if (encr_vector.empty())
  {
    elog(ERROR, "Vector to decrypt is empty.\n");
    return false;
  }

  // Read context and constuct FHEcontext
  unsigned long m1, p1, r1;
  std::vector<long> gens, ords;
  readContextBase(stream_context_, m1, p1, r1, gens, ords);
  FHEcontext context(m1, p1, r1, gens, ords);
  stream_context_ >> context;
  stream_context_.seekg(0);

  // constuct an Encrypted array object ea that is
  // associated with the given context and the polynomial G
  EncryptedArray ea(context, G_);

  // Get public key
  FHEPubKey publicKey(context);
  if (GetPublicKey(publicKey) == false)
  {
    elog(ERROR, "Unable to retireve public key for encryption.\n");
    return false;
  }

  // Reconstruct secret key
  FHESecKey secretKey(context);
  if (GetSecretKey(secretKey) == false)
  {
    elog(ERROR, "Unable to retrieve secret key for decryption.\n");
    return false;
  }

  Ctxt ciphertext(publicKey);

  for (const std::string& item : encr_vector)
  {
    ciphertext.clear();

    // Unpack ciphertext
    UnpackCiphertextFromString(item, ciphertext);

    // Decrypt the context of encrypted vector and put it in plaintext array
    PlaintextArray p00(ea);
    ea.decrypt(ciphertext, secretKey, p00);

    // Decode values from plaintext array and put them into temporal vector
    std::vector<long> v00;
    p00.decode(v00);

    // Push back all elements to decrypted vector
    decr_vector.insert(decr_vector.end(), v00.begin(), v00.end());
  }

  return true;
}

void HEManager::PackCiphertextToString(const Ctxt& ciphertext, std::string& output)
{
  std::stringstream ctxtstream;
  ctxtstream << ciphertext;
  output = ctxtstream.str();
}

void HEManager::UnpackCiphertextFromString(const std::string& ciphertext, Ctxt& output)
{
  std::stringstream ctxtstream;
  ctxtstream.str(ciphertext);
  ctxtstream >> output;
}

std::string HEManager::GetInitContext()
{
  return stream_context_.str();
}

std::string HEManager::GetInitSecretKey()
{
  return stream_secretkey_.str();
}

std::string HEManager::GetInitPublicKey()
{
  return stream_publickey_.str();
}

void HEManager::SetInitContext(const std::string& context)
{
  stream_context_.str(context);
}

void HEManager::SetInitSecretKey(const std::string& seckey)
{
  stream_secretkey_.str(seckey);
}

void HEManager::SetInitPublicKey(const std::string& pubkey)
{
  stream_publickey_.str(pubkey);
}


