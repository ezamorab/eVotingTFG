// Read the publickey, read the value and cipher it.

#include "FHE.h"
#include <cstring>
#include "EncryptedArray.h"
#include <fstream>
#include "myUtils.h"
#include <iostream>
#include <stdlib.h>

#include <stack>
#include <string>
#include <NTL/ZZX.h>
#include <NTL/lzz_pXFactoring.h>
#include "powerful.h"

/* Pour la fonction cputime */
#include <time.h>
#include <sys/time.h>

// Añadidos cuando la re-encryption
#include "EvalMap.h"
#include <NTL/BasicThreadPool.h>

void greeting() {
	cout << "Bienvenido al sistema de votación con Cifrado Homomórfico" << endl;
}

int main(int argc, char **argv)
{

	greeting();
	
	// Read secret key from file
	Timer tReadPrivateKey;
	tReadPrivateKey.start();

	fstream secKeyFile("seckey.txt", fstream::in);
	unsigned long m, p, r;
	vector<long> gens, ords;
	readContextBase(secKeyFile, m, p, r, gens, ords);
	FHEcontext context(m, p, r, gens, ords);
	secKeyFile >> context;

	FHESecKey secretKey(context);
	const FHEPubKey& publicKey = secretKey;

	secKeyFile >> secretKey;

	tReadPrivateKey.stop();
	std::cout << "Time for reading private key from disk: " << tReadPrivateKey.elapsed_time() << "s" << std::endl;

	// Read ciphertext from file
	Timer tReadCiphertext;
	tReadCiphertext.start();
	fstream ciphertextFile("cipherResult.txt", fstream::in);
	Ctxt ctxt(publicKey);
	ciphertextFile >> ctxt;

	tReadCiphertext.stop();
	std::cout << "Time for reading ciphertext from disk: " << tReadCiphertext.elapsed_time() << "s" << std::endl;

	// Decrypt ciphertext
	Timer tDecryption;
	tDecryption.start();

	ZZX ptSum;
	secretKey.Decrypt(ptSum, ctxt);

	tDecryption.stop();
	std::cout << "Time for decryption: " << tDecryption.elapsed_time() << "s" << std::endl;

	std::cout << "Decrypted message: " << std::endl <<  ptSum << std::endl;

	// Output decipher result to file
	std::fstream ciphertext("decryptResult.txt", fstream::out|fstream::trunc);
	assert(ciphertext.is_open());
	ciphertext << ptSum;
	ciphertext.close();


	return 0;
}

