/*
	
 	UNIVERSIDADE FEDERAL RURAL DO SEMI-ÁRIDO
	CENTRO DE CIENCIAS EXATAS E NATURAIS-CCEN
	CURSO DE CIENCIAS DA COMPUTAÇÃO
	DISCIPLINA DE COMPUTAÇÃO GRÁFICA
	PROFº LEANDRO SOUZA
	=====================================================================
	ARTHUR ALEKSANDRO ALVES SILVA
	
	
 * 1. Implementar um visualizador de objetos 3D:
 *	- Construa uma estrutura de dados para representação de coordenadas, vértices e faces da estrutura.
 *  - Implemente um leitor de arquivos do formato ply (http://en.wikipedia.org/wiki/PLY_%28file_format%29 e http://paulbourke.net/dataformats/ply/ ) para fazer a leitura dos objetos.
 *  - O sistema deve possibilitar a manipulação por parte do usuário através de rotação, translação e escala do objeto na cena.
 * 2. Incluir 2 luzes na cena gráfica com iluminação difusa e especular. Permita que o usuário modifique a posição e as cores das luzes na cena.
 */

#include "glut_ply.h"

int main(int argc, char** argv)
{

	StartArgs(argc,argv);
    
    int carg = 1;
    char *varg[1] = {(char*)"MontyPlython"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Monty Plython");
    glutFullScreen();// making the window full screen
    
    Inicializa();

    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);   
    glutKeyboardFunc(GerenciaTeclado);
    glutMouseFunc(GerenciaMouse);	
    glutMotionFunc(mouseMotion);
    glutMainLoop();
}