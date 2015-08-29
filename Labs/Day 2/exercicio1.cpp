/* 
 * File:   exercicio1.cpp
 * Author: Nino
 *
 * FAZER O DE OPEN NO FINAL DO SLIDE 2!!!!!!!!!
 */

#include <cstdlib>
#include <iup/iup.h>
#include <iostream>

using namespace std;

 // Variáveis globais para poder acessar na função callback
Ihandle *weightTB;

Ihandle *heightTB;

Ihandle *iteracoesTB;

Ihandle *rTB;

Ihandle *iTB;

// Função callback para o botão Calcular
int calcBCallback (Ihandle *b) {

    char *val = IupGetAttribute(weightTB, "VALUE");
    int w = atoi(val);

	val = IupGetAttribute(heightTB, "VALUE");
	int h = atoi(val);

	val = IupGetAttribute(iteracoesTB, "VALUE");
	int it = atoi(val);

	val = IupGetAttribute(rTB, "VALUE");
	double r = atof(val);

	val = IupGetAttribute(iTB, "VALUE");
	double i = atof(val);

    cout << "Peso: " << w << endl;
    cout << "Altura: " << h << endl;
    cout << "Iteracoes: " << it << endl;
    cout << "R: " << r << endl;
    cout << "I: " << i << endl << endl;

    return IUP_DEFAULT;
}

// Função callback para o botão Sair
int sairBCallback (Ihandle *b) {

    cout << "Saiu!" << endl;

    return IUP_CLOSE;
}

void createWindow( ) {

    // Primeiro horizontal box e conteúdo
    Ihandle *dimImagemL = IupLabel("Dimensão da imagem: ");

    Ihandle *wL = IupLabel("W: ");

    weightTB = IupText(NULL);

    IupSetAttribute(weightTB, IUP_VALUE, "1024"); // Dando valor default ao campo de peso

    IupSetAttribute(weightTB, IUP_MASK, IUP_MASK_UINT); // Máscara para receber apenas valores inteiros positivos

    Ihandle *hL = IupLabel("H: ");

    heightTB = IupText(NULL);

    IupSetAttribute(heightTB, "VALUE", "768");

    IupSetAttribute(heightTB, IUP_MASK, IUP_MASK_UINT);

    Ihandle *hboxOne = IupHbox(dimImagemL, wL, weightTB, hL, heightTB, NULL);


    // Segundo horizontal box e conteúdo
    Ihandle *numItL = IupLabel("Número de iterações: ");

    iteracoesTB = IupText(NULL);

	IupSetAttribute(iteracoesTB, "VALUE", "3000");

	IupSetAttribute(iteracoesTB, IUP_MASK, IUP_MASK_UINT);

    Ihandle *hboxTwo = IupHbox(numItL, iteracoesTB, NULL);


    // Terceiro horizontal box e conteúdo
    Ihandle *constL = IupLabel("Constante C: ");

    Ihandle *rL = IupLabel("R: ");

    rTB = IupText(NULL);

	IupSetAttribute(rTB, "VALUE", "-0.8");

	IupSetAttribute(rTB, IUP_MASK, IUP_MASK_FLOAT);

    Ihandle *iL = IupLabel("I: ");

    iTB = IupText(NULL);

	IupSetAttribute(iTB, "VALUE", "0.156");

	IupSetAttribute(iTB, IUP_MASK, IUP_MASK_FLOAT);

    Ihandle *hboxThree = IupHbox(constL, rL, rTB, iL, iTB, NULL);


    // Quarto horizontal box e conteúdo
    Ihandle *calcB = IupButton("Calcular", NULL);

    IupSetCallback(calcB, "ACTION",(Icallback) calcBCallback); // Callback para o botão

    Ihandle *sairB = IupButton("Sair", NULL);

    IupSetCallback(sairB, "ACTION",(Icallback) sairBCallback);

    Ihandle *hboxFour = IupHbox(calcB, sairB, NULL);


    // Vertical box que reúne todos os horizontal box's
    Ihandle *vbox = IupVbox(hboxOne, hboxTwo, hboxThree, hboxFour, NULL);


    // Janela de diálogo onde mostro tudo
    Ihandle *dialogo = IupDialog(vbox);

    // Atributo para título da janela de diálogo
    IupSetAttribute(dialogo, "TITLE", "Julia Set");

    // Mostrando a janela de diálogo
    IupShow(dialogo);
}

int main( int argc, char** argv ) {

    //Inicializa IUP.
    IupOpen( &argc, &argv );

    //Cria janela da IUP.
    createWindow( );
    
    //Coloca a IUP em loop.
    IupMainLoop( );

    //Fecha a IUP e libera os espacos alocados.
    IupClose( );

    return 0;
}
