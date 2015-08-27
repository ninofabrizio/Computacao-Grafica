#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;

class Racional {

    private:
        int numerador, denominador;
    
    public:
	Racional(int num = 1, int den = 2);

	int getNumerador();
	int getDenominador();
        void adicao(Racional, Racional);
	void subtracao(Racional, Racional);
//	void multiplicacao(Racional, Racional);
//	void divisao(Racional, Racional);
	void imprimeNumDen();
	void imprimePontFlut();

    private:
	void reduzNumDen();
};

// Função privada para reduzir numerador e denominador
void Racional::reduzNumDen() {

	for(int i = 2; i < 10; i++) {

		if(numerador % i == 0 && denominador % i == 0) {

			numerador = numerador / i;
			denominador = denominador / i;
			i = 1;
		}
	}
}

// Construtor da classe Racional (não default)
Racional::Racional (int num, int den) {

	numerador = num;
	denominador = den;

	this->reduzNumDen();
}

// Função para devolver o valor do denominador (este é private)
int Racional::getNumerador() {

	return numerador;
}

// Função para devolver o valor do denominador (este é private)
int Racional::getDenominador() {

	return denominador;
}

// Função para somar dois números racionais e jogar o resultado na classe que chama o método
void Racional::adicao(Racional r1, Racional r2) {

	int numNovo, denNovo;

	if(r1.getDenominador() == r2.getDenominador()) {

		numNovo = r1.getNumerador() + r2.getNumerador();
		denNovo = r1.getDenominador();
	}
	else if(r1.getDenominador() % r2.getDenominador() == 0) {

		int temp = r1.getDenominador()/r2.getDenominador();
		denNovo = r1.getDenominador();
		numNovo = r1.getNumerador() + (r2.getNumerador() * temp);
	}
	else if(r2.getDenominador() % r1.getDenominador() == 0) {

		int temp = r2.getDenominador()/r1.getDenominador();
		denNovo = r2.getDenominador();
		numNovo = (r1.getNumerador() * temp) + r2.getNumerador();
	}

	numerador = numNovo;
	denominador = denNovo;

	this->reduzNumDen();
}

// Função para subtrair dois números racionais e jogar o resultado na classe que chama o método
// Obs.: Segue a orgem (r1 - r2) sempre
void Racional::subtracao(Racional r1, Racional r2) {

	int numNovo, denNovo;

	if(r1.getDenominador() == r2.getDenominador()) {

		numNovo = r1.getNumerador() - r2.getNumerador();
		denNovo = r1.getDenominador();
	}
	else if(r1.getDenominador() % r2.getDenominador() == 0) {

		int temp = r1.getDenominador()/r2.getDenominador();
		denNovo = r1.getDenominador();
		numNovo = r1.getNumerador() - (r2.getNumerador() * temp);
	}
	else if(r2.getDenominador() % r1.getDenominador() == 0) {

		int temp = r2.getDenominador()/r1.getDenominador();
		denNovo = r2.getDenominador();
		numNovo = (r1.getNumerador() * temp) - r2.getNumerador();
	}

	numerador = numNovo;
	denominador = denNovo;

	this->reduzNumDen();
}

// Função para imprimir racional no formato numerador/denominador
void Racional::imprimeNumDen() {

	cout << numerador << "/" << denominador << endl;
}

// Função para imprimir racional no formato de ponto flutuante
void Racional::imprimePontFlut() {

	float num = (float)numerador/(float)denominador;

	cout.precision(3); // Para mostrar as casas decimais (também precisa do fixed abaixo ao imprimir)

	cout << fixed << num << endl;	
}

int main () {

	Racional r1;
	Racional r2(2, 4);
	Racional r3;

	Racional r4(20, 16);
	Racional r5(18, 3);
	Racional r6(99, 98);

	r3.adicao(r1, r2);

    	cout << "Numero 1: "; 
	r1.imprimeNumDen();
	cout << endl;
    	cout << "Numero 2: ";
	r2.imprimeNumDen();
	cout << endl;
    	cout << "Numero da soma: ";
	r3.imprimeNumDen();
	cout << endl;

    	cout << "Numero 1 (ponto flutuante): ";
	r1.imprimePontFlut();
	cout << endl;
    	cout << "Numero 2 (ponto flutuante): ";
	r2.imprimePontFlut();
	cout << endl;
    	cout << "Numero da soma (ponto flutuante): ";
	r3.imprimePontFlut();
	cout << endl;

	r6.subtracao(r4, r5);	

    	cout << "Numero 4: "; 
	r4.imprimeNumDen();
	cout << endl;
    	cout << "Numero 5: ";
	r5.imprimeNumDen();
	cout << endl;
    	cout << "Numero da subtracao: ";
	r6.imprimeNumDen();
	cout << endl;

    	cout << "Numero 4 (ponto flutuante): ";
	r4.imprimePontFlut();
	cout << endl;
    	cout << "Numero 5 (ponto flutuante): ";
	r5.imprimePontFlut();
	cout << endl;
    	cout << "Numero da subtracao (ponto flutuante): ";
	r6.imprimePontFlut();
	cout << endl;

    	return 0;
}
