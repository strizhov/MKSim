#include <fstream>
#include <sstream>
#include <sys/time.h>

#include <NTL/lzz_pXFactoring.h>

#include "FHE.h"
#include "timing.h"
#include "EncryptedArray.h"


// compilation: g++ Test.cpp ./HElib/src/fhe.a -I./HElib/src -o Test -L/usr/local/lib -lntl

void timestamp()
{
  time_t ltime = time(NULL); /* get current cal time */
  printf("%ld\n", ltime );
}



int main(int argc, char **argv)
{
  /* On our trusted system we generate a new key
   * (or read one in) and encrypt the secret data set.
  */
  
  // Initialization: set initial parameters


  long m=0;			// specific modudus, calculated later using FindM()
  long k = 128;			// security parameter
  long L=4;			// # of primes in the modulus chai
  long c=3;			// number of columns in key switching matrices
  long p=3001;			// plaintext base
  long r=2;			// lifting
  long d=0;			// degree of the field extension
  long s=0;			// at least that many plaintext slots
  long w=64;			// Hamming weight of secret key

  ZZX G;			// Defines the plaintext space

  m = FindM(k, L, c, p, d, s, m, true);
 
  cout << "m: " << m << endl; 

 // setTimersOn();

    timestamp();
  // Key generation: output private and public keys
  
  // initialize context
  FHEcontext context(m, p, r);

  // modify the context, adding primes to the modulus chain
  buildModChain(context, L, c);
  
  // construct a secret key structure
  FHESecKey secretKey(context);
  
  // an "upcast": FHESecKey is a subclass of FHEPubKey
  const FHEPubKey& publicKey = secretKey;

  if (d == 0)
    G = context.alMod.getFactorsOverZZ()[0];
  else
    G = makeIrredPoly(p, d);


  // actually generate a secret key with Hamming weight w
  secretKey.GenSecKey(w);

  // compute key-switching matrices that we need
  addSome1DMatrices(secretKey);

  // Encryption:

  // constuct an Encrypted array object ea that is
  // associated with the given context and the polynomial G
  EncryptedArray ea(context, G);
  
  long nslots = ea.size();

  cout << "nslots: " << nslots << endl; 
  
    timestamp();

  vector<Ctxt> tf_mat;

  // Encryption
  int num_docs = 2;
  for (int i =0; i < num_docs; i++)
  {
    // Create vector of TF values
    vector<long> v0;
    for (long  i = 0; i < nslots; i++)
    {
      v0.push_back(i+2); // fill up with random values
    }
    
    // Encrypt vector with publicKey
    PlaintextArray p0(ea);
    p0.encode(v0);
  
    Ctxt c0(publicKey);
    ea.encrypt(c0, publicKey, p0);

    // Add ciphertext vector to the tf_mat
    tf_mat.push_back(c0);
  }

  // Do hohomorphic calculation
  
  // Create query vector and encrypt it with publicKey
  vector<long> v1; 
  for (long i = 0; i < nslots; i++)
  {
    v1.push_back(i*3);
  }
  
  PlaintextArray p1(ea);
  p1.encode(v1);
  Ctxt c1(publicKey);
  ea.encrypt(c1, publicKey, p1);

  for (int i = 0; i < num_docs; i++)
  {
    Ctxt ctext1 = tf_mat[i];
    ctext1.addCtxt(c1);
  }
    



  // Decryption 
  for (int i = 0; i < num_docs; i++)
  {
    Ctxt tmp = tf_mat[i];
    
    // Decrypt ciphertext vector
    PlaintextArray p00(ea);
    ea.decrypt(tmp, secretKey, p00); 
  
    vector<long> v00; 
    p00.decode(v00);

    // Print values out
    for (int i = 0 ; i < v00.size(); i++)
       cout << v00[i] << " ";
  
    cout << endl; 

  }

  // encryption part 


/*

  vector<long> v1; 
  for (long i = 0; i < nslots; i++)
  {
    v1.push_back(i*3);
  }
  
  PlaintextArray p1(ea);
  p1.encode(v1);
  Ctxt c1(publicKey);
  ea.encrypt(c1, publicKey, p1);
*/




/*
  PlaintextArray const1(ea);
  long factor1 = 5; 
  const1.encode(factor1);
  ZZX const1_poly;
  ea.encode(const1_poly, const1);

  // Homomorphic operation
  c0.multByConstant(const1_poly); // c0 = c0 + const1
*/
  
//  Ctxt result(c0);
//  tmp.multiplyBy(c1);



  //c0.addCtxt(c1);
   
/*
  // decryption part
  PlaintextArray p00(ea);
  ea.decrypt(tmp, secretKey, p00); 
  
  vector<long> v00; 
  p00.decode(v00);

  for (int i = 0 ; i < v00.size(); i++)
     cout << v00[i] << " ";
  
  cout << endl; 
*/
  return 0;
}

