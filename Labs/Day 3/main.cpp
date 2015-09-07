/* 
 * File:   main.cpp
 * Author: Nino
 *
 * Created on September 4, 2015, 10:27 AM
 *
 * Arquivo principal de teste. Contém apenas a main onde testo as
 * funções/métodos implementados em vetor3d.cpp
 *
 * TODO
 * FALTAM IMPLEMENTAR:
 * PRODUTO VETORIAL
 * NORMA DO VETOR
 * DIVISÃO POR ESCALAR
 * ÁREA
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "vetor3d.h"

using namespace std;

int main (int argc, char** argv) {

	cout << endl << "Testando SOMA de vetores: " << endl;

	vetor3d v1 = vetor3d(1.0, 2.0, 3.0);
	printf("\nV1 = ( %.3f , %.3f , %.3f )\n", v1.getX(), v1.getY(), v1.getZ());
    
	vetor3d v2 = vetor3d(4.0, 5.0, 6.0);
	printf("V2 = ( %.3f , %.3f , %.3f )\n", v2.getX(), v2.getY(), v2.getZ());
    
    vetor3d v3 = v1 + v2;
    printf("\nV3 = V1 + V2\nV3 = ( %.3f , %.3f , %.3f )\n", v3.getX(), v3.getY(), v3.getZ());
    
	cout << endl << "Testando SUBTRACAO de vetores: " << endl;

	vetor3d v4 = vetor3d(v3);
	printf("\nV4 = ( %.3f , %.3f , %.3f )\n", v4.getX(), v4.getY(), v4.getZ());
    
	vetor3d v5 = vetor3d(2.25, 1.75, 0.005);
	printf("V5 = ( %.3f , %.3f , %.3f )\n", v5.getX(), v5.getY(), v5.getZ());
    
    vetor3d v6 = v4 - v5;
    printf("\nV6 = V4 - V5\nV6 = ( %.3f , %.3f , %.3f )\n", v6.getX(), v6.getY(), v6.getZ());

	cout << endl << "Testando MULTIPLICACAO POR ESCALAR: " << endl;

	vetor3d v7 = vetor3d(v6);
	printf("\nV7 = ( %.3f , %.3f , %.3f )\n", v7.getX(), v7.getY(), v7.getZ());
    
	double e = 5.5;
	printf("Escalar = %.3f\n", e);
    
    vetor3d v8 = v7 * e;
    printf("\nV8 = V7 * Escalar\nV8 = ( %.3f , %.3f , %.3f )\n", v8.getX(), v8.getY(), v8.getZ());

	cout << endl << "Testando PRODUTO ESCALAR: " << endl;

	vetor3d v9 = vetor3d(v3);
	printf("\nV9 = ( %.3f , %.3f , %.3f )\n", v9.getX(), v9.getY(), v9.getZ());
    
	vetor3d v10 = vetor3d(v8);
	printf("V10 = ( %.3f , %.3f , %.3f )\n", v10.getX(), v10.getY(), v10.getZ());
    
    e = v9 * v10;
    printf("\nEscalar = V9 * V10\nEscalar = %.3f\n", e);

    return 0;
}
