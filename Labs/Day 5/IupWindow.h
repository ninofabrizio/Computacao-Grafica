/*
* File: IupWindow.h
* Author: Nino
*
* Arquivo que possui a definição da classe IupWindow com seus
* atributos, construtores e métodos.
*/

#ifndef IUPWINDOW_H
#define	IUPWINDOW_H

class IupWindow {

    public:

        /**
        * Construtor default da classe
        *
        * @return Nova instância da classe IupWindow
        */
        IupWindow();

        /**
        * Destrutor da classe
        *
        */
        ~IupWindow();

        /**
        * Método para mostrar a janela
        *
        */
        void show();

        /**
        * Método para ocultar a janela
        *
        */
        void hide();


    private:

        void createWindow();

        static int calcBCallback (Ihandle *b);

        static int openBCallback (Ihandle *b);

        static int sairBCallback (Ihandle *b);

    private:

        Ihandle *dialogo;

};

#endif	/* IUPWINDOW_H */
