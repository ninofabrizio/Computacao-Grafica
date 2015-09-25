/*
* File: IupWindow.cpp
* Author: Nino
*
* Arquivo que contém todas as implementações dos construtores e
* métodos da classe IupWindow definida em IupWindow.h.
*/

#include <cstdlib>
#include <iup/iup.h>
#include <iostream>
#include "IupWindow.h"

using namespace std;

IupWindow::IupWindow() {

	createWindow();
}

IupWindow::~IupWindow() {

    IupDestroy(dialogo);
}

void IupWindow::show() {
   
    IupShow(dialogo);
}

void IupWindow::hide() {

    IupHide(dialogo);
}

void IupWindow::createWindow() {

    // Primeiro horizontal box e conteúdo
    Ihandle *dimImagemL = IupLabel("Dimensão da imagem: ");

    Ihandle *wL = IupLabel("W: ");
//TODO
    Ihandle *weightTB = IupText(NULL);

    IupSetAttribute(weightTB, IUP_VALUE, "1024"); // Dando valor default ao campo de peso

    IupSetAttribute(weightTB, IUP_MASK, IUP_MASK_UINT); // Máscara para receber apenas valores inteiros positivos

    Ihandle *hL = IupLabel("H: ");
//TODO
    Ihandle *heightTB = IupText(NULL);

    IupSetAttribute(heightTB, "VALUE", "768");

    IupSetAttribute(heightTB, IUP_MASK, IUP_MASK_UINT);

    Ihandle *hboxOne = IupHbox(dimImagemL, wL, weightTB, hL, heightTB, NULL);


    // Segundo horizontal box e conteúdo
    Ihandle *numItL = IupLabel("Número de iterações: ");
//TODO
    Ihandle *iteracoesTB = IupText(NULL);

	IupSetAttribute(iteracoesTB, "VALUE", "3000");

	IupSetAttribute(iteracoesTB, IUP_MASK, IUP_MASK_UINT);

    Ihandle *hboxTwo = IupHbox(numItL, iteracoesTB, NULL);


    // Terceiro horizontal box e conteúdo
    Ihandle *constL = IupLabel("Constante C: ");

    Ihandle *rL = IupLabel("R: ");
//TODO
    Ihandle *rTB = IupText(NULL);

	IupSetAttribute(rTB, "VALUE", "-0.8");

	IupSetAttribute(rTB, IUP_MASK, IUP_MASK_FLOAT);

    Ihandle *iL = IupLabel("I: ");
//TODO
    Ihandle *iTB = IupText(NULL);

	IupSetAttribute(iTB, "VALUE", "0.156");

	IupSetAttribute(iTB, IUP_MASK, IUP_MASK_FLOAT);

    Ihandle *hboxThree = IupHbox(constL, rL, rTB, iL, iTB, NULL);


    // Quarto horizontal box e conteúdo
    Ihandle *calcB = IupButton("Calcular", NULL);

    IupSetCallback(calcB, "ACTION",(Icallback) calcBCallback); // Callback para o botão

	Ihandle * openB = IupButton("Abrir", NULL);

	IupSetCallback(openB, "ACTION", (Icallback) openBCallback);

    Ihandle *sairB = IupButton("Sair", NULL);

    IupSetCallback(sairB, "ACTION",(Icallback) sairBCallback);

    Ihandle *hboxFour = IupHbox(calcB, openB, sairB, NULL);


    // Vertical box que reúne todos os horizontal box's
    Ihandle *vbox = IupVbox(hboxOne, hboxTwo, hboxThree, hboxFour, NULL);

	// Definição e atributos da janela para abrir arquivo
	Ihandle *dlg = IupFileDlg();

	IupSetAttribute(dlg, "DIALOGTYPE", "OPEN");
	IupSetAttribute(dlg, "TITLE", "IupFileDlg Test");
	IupSetAttribute(dlg, "FILTER", "*bmp");
	IupSetAttributes(dlg, "FILTERINFO = Bitmap Files");

    // Janela de diálogo onde mostro tudo
    dialogo = IupDialog(vbox);

    // Atributo para título da janela de diálogo
    IupSetAttribute(dialogo, "TITLE", "Julia Set");

	IupSetAttribute(dialogo, "weightTB", (char *)weightTB);
	IupSetAttribute(dialogo, "heightTB", (char *)heightTB);
	IupSetAttribute(dialogo, "iteracoesTB", (char *)iteracoesTB);
	IupSetAttribute(dialogo, "rTB", (char *)rTB);
	IupSetAttribute(dialogo, "iTB", (char *)iTB);
	IupSetAttribute(dialogo, "dlg", (char *)dlg);

	this->hide();
}

int IupWindow::calcBCallback (Ihandle *b) {

	Ihandle *weightTB = (Ihandle *)IupGetAttribute(b, "weightTB");
    char *val = IupGetAttribute(weightTB, "VALUE");
    int w = atoi(val);

	Ihandle *heightTB = (Ihandle *)IupGetAttribute(b, "heightTB");
	val = IupGetAttribute(heightTB, "VALUE");
	int h = atoi(val);

	Ihandle *iteracoesTB = (Ihandle *)IupGetAttribute(b, "iteracoesTB");
	val = IupGetAttribute(iteracoesTB, "VALUE");
	int it = atoi(val);

	Ihandle *rTB = (Ihandle *)IupGetAttribute(b, "rTB");
	val = IupGetAttribute(rTB, "VALUE");
	double r = atof(val);

	Ihandle *iTB = (Ihandle *)IupGetAttribute(b, "iTB");
	val = IupGetAttribute(iTB, "VALUE");
	double i = atof(val);

    cout << "Peso: " << w << endl;
    cout << "Altura: " << h << endl;
    cout << "Iteracoes: " << it << endl;
    cout << "R: " << r << endl;
    cout << "I: " << i << endl << endl;

    return IUP_DEFAULT;
}

int IupWindow::openBCallback (Ihandle *b) {

	Ihandle *dlg = (Ihandle *)IupGetAttribute(b, "dlg");
	// Define posição que vai aparecer
	IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);

	// Se retorno for diferente de -1, existe um endereço válido
	if(IupGetInt(dlg, "STATUS") != -1) {

		cout << "OK" << endl;
		cout << "	VALUE(" << IupGetAttribute(dlg, "VALUE") << ")\n" << endl;
	}
	else
		cout << "Cancelou!\n" << endl;

    return IUP_DEFAULT;
}

int IupWindow::sairBCallback (Ihandle *b) {

    cout << "Saiu!" << endl;

    return IUP_CLOSE;
}
