/** sumVotes.cpp
**
**		FICHERO FUENTE PARA LA SUMA HOMOMÓRFICA DE LOS VOTOS INDIVIDUALES CIFRADOS
**		Se tiene un fichero que contiene la suma cifrada (inicialmente 0)
**		al que se le va sumando cada voto individual cifrado
**/

// Se importan los ficheros de cabecera necesarios
#include "FHE.h"
#include "EncryptedArray.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdlib.h>

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

// Añadidos cuando la re-encryption
#include "EvalMap.h"
#include <NTL/BasicThreadPool.h>

int main(int argc, char **argv)
{
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

	Timer tReadCiphertext;
	tReadCiphertext.start();

	// Leemos los ficheros donde está la suma total y el voto individual a sumar
	fstream ciphertextFile("cipherResult.txt", fstream::in);
	fstream ciphertextFile2("ciphertext.txt", fstream::in);

	// HELib usa la clase Ctxt para contener los textos cifrados
	Ctxt ctxt(publicKey);
	Ctxt ctxt2(publicKey);
	ciphertextFile >> ctxt;
	ciphertextFile2 >> ctxt2;

	// Creamos otro objeto Ctxt y sumamos
	Ctxt ctSum = ctxt;
	ctSum += ctxt2;

	tReadCiphertext.stop();
	std::cout << "Time for reading ciphertext from disk: " << tReadCiphertext.elapsed_time() << "s" << std::endl;

	// Guardamos el resultado de la suma cifrado en el fichero correspondiente
	std::fstream ciphertext("cipherResult.txt", fstream::out|fstream::trunc);
	assert(ciphertextFile.is_open());
	ciphertext << ctSum;
	ciphertext.close();

	return 0;
}

