#include <GL/glut.h>
#include <iostream>
#include <math.h>

#define ANGLE_CHECK 25

#define Z_EYE 100
#define Z_NEAR 0.5
#define Z_FAR 500

#define RED 0
#define GREEN 1
#define BLUE 2
#define CYAN 3
#define MAGENTA 4
#define YELLOW 5
#define WHITE 6
#define DEFAULT 7

#define COLOROPTS 8

#define WIDTH 500
#define HEIGHT 500

using namespace std;

// Read and Store Ply File - Class
class ReadPly 
{
	private:
		double x;
		double y;
		double z;
		
	public:
		ReadPly();
		double getter(int i);
		void setter_vertex( int i , double value); 
};

// Função callback chamada para gerenciar eventos do teclado
void GerenciaTeclado(unsigned char key, int x, int y);
// Função callback chamada para gerenciar eventos do teclado   
// para teclas especiais, tais como F1, PgDn e Home
void TeclasEspeciais(int key, int x, int y);
void mouseMotion (int x, int y) ;
// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y);
// Função callback chamada para fazer o desenho
void Desenha(void);
void lightning();
// Função usada para especificar o volume de visualiza��o
// void EspecificaParametrosVisualizacao(void);

void CriaMenu();
        
// Gerenciamento do menu principal           
void MenuPrincipal(int op);
// menu do estilo de display do objeto
void MenuObjectStyle(int op);
// Gerenciamento do menu com as opções de cores           
void MenuCorObjeto(int op);
void MenuCorLuz1(int op);
void MenuCorLuz2(int op);
            
void Inicializa();

void quaternion(char ch,int angle);
void dragRotation(int xPos, int yPos); 


// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h);

void doAnimate();

void StartArgs(int argc, char* argv[]);

void initNormals();

void drawPly();