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
 *  File: HEManager.h
 *  Authors: Mikhail Strizhov
 *
 *  Date: Oct 22, 2014
 *
 */

#ifndef _HE_MANAGER_H
#define _HE_MANAGER_H

// System
#include <sstream>

// Shared

// Module
#include <boost/noncopyable.hpp>
#include <NTL/lzz_pXFactoring.h>
#include "FHE.h"
#include "EncryptedArray.h"

class HEManager : boost::noncopyable
{
  public:
    HEManager();
    virtual ~HEManager();

    void Init();
    void ClearKeys();

	std::string GetInitContext();
	void SetInitContext(const std::string& context);

	std::string GetInitSecretKey();
	void SetInitSecretKey(const std::string& seckey);

	std::string GetInitPublicKey();
	void SetInitPublicKey(const std::string& pubkey);

    bool EncryptArray(const std::vector<long>& vector, std::vector<std::string>& encr_vector);
    bool DecryptArray(const std::vector<std::string>& encr_vector, std::vector<long>& decr_vector);

    bool CalculateSimilarity(const std::vector<std::string>& encr_term_weight,
                             const std::vector<std::string>& encr_search_query,
                             std::vector<std::string>& encr_similarity_coefficient);
  private:
    HEManager(HEManager &other);
    HEManager &operator=(HEManager &other);

	bool GetSecretKey(FHESecKey& key);
	bool GetPublicKey(FHEPubKey& key);

    bool ConstructSingleCipehrtext(const std::vector<long>& vector,
                                   const long& start,
                                   const long& end,
                                   const EncryptedArray& ea,
                                   const FHEPubKey& publicKey,
                                   Ctxt& ciphertext);

    void PackCiphertextToString(const Ctxt& ciphertext, std::string& output);
    void UnpackCiphertextFromString(const std::string& ciphertext, Ctxt& output);

    long m_;			// specific modudus, calculated using FindM() in HE
    long k_;			// security parameter
    long L_;			// # of primes in the modulus chai
    long c_;			// number of columns in key switching matrices
    long p_;			// plaintext base
    long r_;			// lifting
    long d_;			// degree of the field extension
    long s_;			// at least that many plaintext slots
    long w_;			// Hamming weight of secret key

    ZZX G_;                     // Defines the plaintext

    std::stringstream stream_context_;
    std::stringstream stream_secretkey_;
    std::stringstream stream_publickey_;

};


#endif // _HEMANAGER_H
