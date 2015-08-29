/* 
 * File:   main.cpp
 * Author: jeferson
 *
 * Created on 5 de Agosto de 2014, 19:57
 */

#include <cstdlib>
#include <iup/iup.h>
using namespace std;



void createWindow( )
{
    //Cria label.
    Ihandle *button = IupButton( "Hello IUP", NULL );

    //Cria label.
    Ihandle *label = IupLabel( "Minha primeira caixa de texto:" );

    //Cria caixa de texto.
    Ihandle *textBox = IupText( NULL );

    //Cria composicoes.
    Ihandle *hbox = IupHbox( label, textBox, NULL );

    Ihandle *vbox = IupVbox( hbox, button, NULL );
    
    //Cria dialogo.
    Ihandle *dialog = IupDialog( vbox );

    //Exibe o dialogo.
    IupShow( dialog );
}



/*
 * 
 */
int main( int argc, char** argv )
{
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

