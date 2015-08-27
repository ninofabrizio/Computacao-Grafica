#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;


float calcMedia (float *vet, int n) {

	float med = 0.0;

	for(int i = 0; i < n; i++)
		med += vet[i];

	return med/(float)n;
}

float * retVetLin (float mat[][3], int n, int *tam) {

	float *vet;
	int k = 0;

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < 3; j++) {

			if(i % 2 != 0) {
				if(k == 0) {
					vet = new float[1];
					vet[0] = mat[i][j];
				}
				else {
					float *temp = new float[k + 1];
					copy(vet, vet + k, temp);
					delete []vet;
					vet = temp;
					vet[k] = mat[i][j];
				}

				k++;
			}
		}
	}
	*tam = k;

	return vet;
}

float * retVetCol (float mat[][3], int n, int *tam) {

	float *vet;
	int k = 0;

	for(int i = 0; i < n; i++) {
		for(int j = 0; j < 3; j++) {

			if(j % 2 == 0) {
				if(k == 0) {
					vet = new float[1];
					vet[0] = mat[i][j];
				}
				else {
					float *temp = new float[k + 1];
					copy(vet, vet + k, temp);
					delete []vet;
					vet = temp;
					vet[k] = mat[i][j];
				}

				k++;
			}
		}
	}
	*tam = k;

	return vet;
}

float calcDifMedias (float mat[][3], int n) {

	int tamLin, tamCol;
	float *lin = retVetLin(mat, n, &tamLin);
	float *col = retVetCol(mat, n, &tamCol);

	float dif = abs(calcMedia(lin, tamLin) - calcMedia(col, tamCol));
	
	delete []lin;
	delete []col;	

	return dif;
}

int main () {

	float mat[5][3] = {1.0 , 2.5 , 3.0 ,
					   4.5 , 5.0 , 6.5 ,
					   7.0 , 8.5 , 9.0 ,
					   10.5, 11.0, 12.5,
					   13.0, 14.5, 15.0};

	cout << "Diferenca das medias: " << calcDifMedias(mat, 5) << endl;

	return 0;
}