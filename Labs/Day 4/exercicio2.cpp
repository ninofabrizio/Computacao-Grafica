#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>

// Função de desenhar casas
void drawHouse(int x, int y) {

    //Define qual area da janela deve ser utilizada para desenho.
    glViewport( x, y, 200, 200 );

	//Define a cor de desenho: verde
	glColor3f( 0,1,0 );
	
	//Desenha um triangulo: telhado
	glBegin( GL_TRIANGLES );
		glVertex2d( 5,5 );
		glVertex2d( -5,5 );
		glVertex2d( 0,8 );
	glEnd();

    glBegin( GL_QUADS );
    	//Define a cor de desenho: azul
        glColor3f( 0,0,1 );
        //Desenha um quadrado: casa
        glVertex2d( -5,-5 );
        glVertex2d( 5,-5 );
        glVertex2d( 5,5 );
        glVertex2d( -5,5 );

    	//Define a cor de desenho: vermelho
        glColor3f( 1,0,0 );

        //Desenha um retangulo: porta
        glVertex2d( 0,-5 );
        glVertex2d( 0,-2 );
        glVertex2d( 3,-2 );
        glVertex2d( 3,-5 );
    glEnd();

}

//Funcao callback de redesenho da janela de visualizacao
void redraw( )
{
    std::cout << "Chamou a funcao Desenha" << std::endl;

    //Limpa a janela de visualizacao com a cor branca
    glClearColor( 1,1,1,0 );
	glClear( GL_COLOR_BUFFER_BIT );
	
  // Desenhar casa inferior esquerda espelhado no y

    //Define a matriz de projecao como corrente
    //para definifir o sistema de coordenadas.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define o sistema de coordenadas do mundo
    //variando de [x1,x2]=[y1,[y2]=[-10,10]
    gluOrtho2D( -10, 10, 10, -10 );

    drawHouse(0, 5);

  // Desenhar casa superior esquerda espelhado no x

    //Define a matriz de projecao como corrente
    //para definifir o sistema de coordenadas.    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define o sistema de coordenadas do mundo
    //variando de [x1,x2]=[y1,[y2]=[-10,10]
    gluOrtho2D( 10, -10, -10, 10 );

    drawHouse(0, 200);

  // Desenhar casa inferior direita maior (zoom +)

    //Define a matriz de projecao como corrente
    //para definifir o sistema de coordenadas.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define o sistema de coordenadas do mundo
    //variando de [x1,x2]=[y1,[y2]=[-10,10]
    gluOrtho2D( -8, 8, -8, 8 );

    drawHouse(200, 5);

  // Desenhar casa superior direita menor (zoom -)

    //Define a matriz de projecao como corrente
    //para definifir o sistema de coordenadas.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Define o sistema de coordenadas do mundo
    //variando de [x1,x2]=[y1,[y2]=[-10,10]
    gluOrtho2D( -15, 15, -15, 15 );

    drawHouse(200, 200);
	
	//Executa os comandos OpenGL e descarrega
    //o resultado na tela.
	glFlush();
}

// Funcao callback chamada para gerenciar eventos de "resize" da janela.
void resize( int w, int h )
{   
    std::cout << "Chamou a funcao MudaTamanho: " << w << " " << h << std::endl;

    //Redesenha a janela.
    redraw();
}

// Programa Principal 
int main( int argc, char ** argv )
{
    //Inicializa a glut com os argumentos do programa.
    glutInit( &argc, argv );

	//Define do modo de operacao da GLUT. Apenas um buffer e
    //cores RGB.
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB ); 
 
	//Especifica o tamanho inicial em pixels da janela GLUT.
	glutInitWindowSize( 400,400 ); 

    //Determina a posicao inicial da janela.
    glutInitWindowPosition( 100,500 );
 
	// Cria a janela passando como argumento o titulo da mesma.
	glutCreateWindow( "Exercício 2" );
 
	// Registra a funcao callback de redesenho da janela de visualizacao.
	glutDisplayFunc( redraw );

    //Trata eventos de quando a janela muda de tamanho.
    glutReshapeFunc( resize );
 
	//Inicia o processamento e aguarda interacoes do usuario.
	glutMainLoop();
 
	return 0;
}
