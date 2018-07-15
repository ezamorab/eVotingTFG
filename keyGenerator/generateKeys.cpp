/** generateKeys.cpp
**
**		FICHERO FUENTE DE CONFIGURACIÓN DE PARÁMETROS Y GENERACIÓN DE LAS CLAVES PÚBLICA Y PRIVADA
**    Se configuran los parámetros necesarios del esquema FHEPubKey
**    Se generan las claves criptográficas
**    Se almacenan en ficheros (junto al contexto, necesario)
**/

// Se importan los ficheros de cabecera necesarios
#include "FHE.h"
#include "EncryptedArray.h"

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <fstream>
#include <stack>
#include <string>

// Implementación para la medición de tiempos de ejecución
#include "myUtils.h"

// Libreria matematica usada por HELib
#include <NTL/ZZX.h>
#include <NTL/lzz_pXFactoring.h>

using namespace std;
stack<Ctxt> theStack;

// Declaraciones necesarias de objetos que albergarán nuestra configuración (parámetros del contexto) y claves
FHEcontext* context;
FHESecKey* secretKey;
FHEPubKey* publicKey;


int main(int argc, char **argv)
{
  cout << "Bienvenido al sistema de generación de claves" << endl;

  	Timer tInitialization;				// Temporizador para el tiempo de inicialización
  	tInitialization.start();

  	/*** BEGIN INITIALIZATION ***/
  	long m = 0;                   // Módulo específico
  	long p = 982451653; 					// Base del texto plano, DEBE SER UN Nº PRIMO
  	long r = 1;                   // Lifting [default=1]
  	long L = 16;                  // Altura máxima de los circuitos que queremos
  	long c = 3;                   // Number of columns in key-switching matrix [default=2]
  	long w = 64;                  // Hamming weight of secret key
  	long d = 0;                   // Degree of the field extension [default=1]
  	long k = 128;                 // Security parameter [default=80] A default of 80 is 80 bits of security
    long s = 0;                   // Minimum number of slots [default=0]



  	std::cout << "\nConfigurando parámetros..." << std::flush;
  	m = FindM(k, L, c, p, d, s, 0);                            // These parameters will be used to find the proper value of m using the function FindM:

  	context = new FHEcontext(m, p, r); 	                        // create a FHEcontext object to hold all the parameters
  	buildModChain(*context, L, c);                           // Modify the context, adding primes to the modulus chain

  	tInitialization.stop();
  	std::cout << "\nInicialización completada. Tiempo requerido: " << tInitialization.elapsed_time() << "s" << std::endl;

  	Timer tWriteKeys;		// Temporizador para la creación y exportación de las claves
  	tWriteKeys.start();

    // "Abrimos" los ficheros de texto donde almacenaremos las claves criptográficas
  	fstream secKeyFile("/home/espe/Sistema/Equipo-Generador-Claves/seckey.txt", fstream::out|fstream::trunc);
  	fstream pubKeyFile("/home/espe/Sistema/Equipo-Generador-Claves/pubkey.txt", fstream::out|fstream::trunc);
  	assert(secKeyFile.is_open());
  	assert(pubKeyFile.is_open());

    // Tenemos que guardar el contexto en los dos ficheros
    // para que quien las use conozca dicho contexto
    // y pueda recrear las claves
  	writeContextBase(secKeyFile, *context);
  	writeContextBase(pubKeyFile, *context);
  	secKeyFile << *context << std::endl;
  	pubKeyFile << *context << std::endl;


    // Creamos el polinomio usado para cifrar la información, haciendo uso de la clase ZZX de la libreria NTL
  	ZZX G =  context->alMod.getFactorsOverZZ()[0];

  	std::cout << "\nGenerando claves... " << std::flush;

  /*
    // Si no declaramos los objetos al principio
  secretKey = new FHESecKey(*context);
  const FHEPubKey& publicKey = *secretKey;
  publicKey = secretKey;                 // Un "upcast": FHESecKey es una subclase de FHEPubKey
  secretKey->GenSecKey(w);               // Se genera una sk con w (Hamming weight)
  addSome1DMatrices(secretKey);
  */

  // Generamos las claves pública y privada
  FHESecKey secretKey(*context);
  const FHEPubKey& publicKey = secretKey;   // Un "upcast": FHESecKey es una subclase de FHEPubKey
  secretKey.GenSecKey(w);                   // Se genera una sk con w (Hamming weight)
  addSome1DMatrices(secretKey);

  // Ahora guardamos las claves en los ficheros, y los cerramos
  secKeyFile << secretKey << std::endl;
  pubKeyFile << publicKey << std::endl;
  secKeyFile.close();
  pubKeyFile.close();

  tWriteKeys.stop();
  std::cout << "\nTiempo requerido para la creación y escritura en ficheros de las claves pk,sk: " << tWriteKeys.elapsed_time() << "s" << std::endl;
}

