/** encryptVote.cpp
**
**		FICHERO FUENTE PARA EL CIFRADO DE CADA VOTO INDIVIDUAL
**		Se lee el contexto y la clave publica
**		Se cifra el valor y se almacena el ciphertext en un fichero
**/

// Se importan los ficheros de cabecera necesarios
#include "FHE.h"
#include "EncryptedArray.h"

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>

#include <stack>
#include <string>
#include <NTL/ZZX.h>
#include <NTL/lzz_pXFactoring.h>
#include "powerful.h"

// Implementación para la medición de tiempos de ejecución
#include "myUtils.h"

/* Pour la fonction cputime */
#include <time.h>
#include <sys/time.h>

/*
	// Añadidos cuando la re-encryption
	#include "EvalMap.h"
	#include <NTL/BasicThreadPool.h>
*/

int main(int argc, char **argv)
{

	int valor = atoi(argv[1]);

	cout << "Bienvenido al sistema de votación con Cifrado Homomórfico" << endl;

	Timer tReadKeys;
	tReadKeys.start();

	// Abrimos el fichero donde está almacenado el contexto y la clave pública
	fstream pubKeyFile("pubkey.txt", fstream::in);
	assert(pubKeyFile.is_open());
	// Declaramos las variables para los parámetros
	unsigned long m, p, r;
	vector<long> gens, ords;
	// Inicializamos el objeto del contexto con los parámetros
	readContextBase(pubKeyFile, m, p, r, gens, ords);
	FHEcontext context(m, p, r, gens, ords);
	pubKeyFile >> context;
	// Obtenemos la clave pública a partir del contexto
	FHEPubKey publicKey(context);
	pubKeyFile >> publicKey;

	pubKeyFile.close();

	tReadKeys.stop();
	std::cout << "Tiempo de configuración del contexto " << tReadKeys.elapsed_time() << "s" << std::endl;

	EncryptedArray ea(context);
	uint nslots = ea.size();

	// Abrimos el fichero donde se encuentra el valor a cifrar
	// Y lo almacenamos en un array
	/*std::fstream messageFile("vote.txt", fstream::in);
	assert(messageFile.is_open());
	uint count = 0;

	std::vector<int> inputArray;
	inputArray.reserve(1);
	for(int64_t counter=0;counter<1;counter++)
	{
	    int temp;
	    messageFile>>temp;
	    inputArray.push_back(temp);
	}
	messageFile.close();*/

	std::cout << "Texto a cifrar: " << std::endl << valor << std::endl;

    // Encryption
	Timer tEncryption;
	tEncryption.start();

		// ea.encrypt(ctxt, publicKey, inputArray);

	// Ciframos el voto
	Ctxt ctxt(publicKey);
	publicKey.Encrypt(ctxt, to_ZZX(valor));

	tEncryption.stop();
	std::cout << "Tiempo empleado en el cifrado: " << tEncryption.elapsed_time() << "s" << std::endl;

	// Guardamos el voto cifrado en un fichero
	std::fstream ciphertext("ciphertext.txt", fstream::out|fstream::trunc);
	assert(ciphertextFile.is_open());
	ciphertext << ctxt;
	ciphertext.close();

	return 0;
}

