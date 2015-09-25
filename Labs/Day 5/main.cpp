/*
* File: main.cpp
* Author: Nino
*
*
* Arquivo principal de teste. Contém apenas a main onde testo os
* métodos implementados em IupWindow.cpp
*
*/

#include <iup/iup.h>
#include <cstdlib>
#include "IupWindow.h"

int main (int argc, char** argv) {

    IupOpen(&argc, &argv);

    IupWindow *window = new IupWindow();

    window->show();

    IupMainLoop();

    delete window;

    IupClose();

    return EXIT_SUCCESS;
}
