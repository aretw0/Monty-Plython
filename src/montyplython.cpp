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

#include <GL/glut.h>
#include <iostream>
#include "../lib/rply-1.1.4/rply.h"

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

#define COLOROPTS 7

using namespace std;

// Largura e altura da janela
GLfloat windowWidth = 500;
GLfloat windowHeight = 500;
GLfloat xMax = windowWidth/2;
GLfloat yMax = windowHeight/2;

GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};		 // "cor" 
GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0};// "brilho" 
GLfloat posicaoLuz1[4]={-25.0, 50.0, 50.0, 1.0};
GLfloat posicaoLuz2[4]={25.0, 50.0, 50.0, 1.0};

// Capacidade de brilho do material
GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
GLint especMaterial = 60;

// Variáveis
GLfloat r = 1.0f, g = 0.0f, b = 0.0f;
GLfloat r1 = 1.0f, g1 = 1.0f, b1 = 1.0f;
GLfloat r2 = 1.0f, g2 = 1.0f, b2 = 1.0f;
GLint view_w, view_h;

// variáveis da translação
GLfloat xT = 0, yT = 0, zT = 0;
// variáveis do scaling
GLfloat xS = 1.0, yS = 1.0, zS = 1.0;
// variável da rotação
GLfloat aXR = 0, aYR = 0, aZR = 0;

// câmera
GLdouble eyeX = 0, eyeY=0, eyeZ=Z_EYE, cX=0, cY=0, cZ=0, upX = 0, upY = 1, upZ=0;

GLfloat angle = 25, fAspect;

// taxa de tamanho
GLsizei rsize = 10;

bool fs = true, plyReady = false;

long nvertices, ntriangles;

// Função usada para especificar o volume de visualiza��o
void EspecificaParametrosVisualizacao(void);

void Inicializa();
void CriaMenu();
        
// Gerenciamento do menu principal           
void MenuPrincipal(int op);

// Gerenciamento do menu com as opções de cores           
void MenuCorObjeto(int op);
void MenuCorLuz1(int op);
void MenuCorLuz2(int op);
            
// Função callback chamada para gerenciar eventos do teclado   
// para teclas especiais, tais como F1, PgDn e Home
void TeclasEspeciais(int key, int x, int y);

// Função callback chamada para gerenciar eventos do teclado
void GerenciaTeclado(unsigned char key, int x, int y);

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y);

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h);

void doAnimate();

// Função callback chamada para fazer o desenho
void Desenha(void);

void StartArgs(int argc, char* argv[]);
// Programa Principal 


static int vertex_cb(p_ply_argument argument) {
    long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
    cout << ply_get_argument_value(argument);
    if (eol) cout << endl;
    else cout << " ";;
    return 1;
}

static int face_cb(p_ply_argument argument) {
    long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    switch (value_index) {
        case 0:
        case 1: 
            cout << ply_get_argument_value(argument);
            break;
        case 2:
            cout << ply_get_argument_value(argument) << endl;
            break;
        default: 
            break;
    }
    return 1;
}


int main(int argc, char** argv)
{

	StartArgs(argc,argv);
    
    int carg = 1;
    char *varg[1] = {(char*)"montyplython"};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(10,10);
    glutCreateWindow("Monty Plython");
    glutFullScreen();// making the window full screen
    
    Inicializa();

    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);   
    glutKeyboardFunc(GerenciaTeclado);
    glutMainLoop();
}


// Função callback chamada para fazer o desenho
void Desenha(void)
{
    // Limpa a janela e o depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    EspecificaParametrosVisualizacao();

    glPushMatrix();
        doAnimate();
        glColor3f(r,g,b);  
        // Desenha o ply se tiver pronto ou o teapot
		if (plyReady) {
			// Desenhando o ply
			// drawPly();
			glutSolidCone(rsize,rsize*2,100,100);
		} else {
			// glutSolidTeapot(rsize);
			glutSolidCube(rsize+rsize/2);
		}
    glPopMatrix();
    glutSwapBuffers();
}

void StartArgs(int argc, char* argv[]) {	

 	// Checando o número de parâmetros

    if (argc >= 2) {
		cout << "Argumento detectado: " << argv[1] << endl;
		// fazer leitura de arquivo aqui

        // p_ply ply = ply_open(argv[1], NULL, 0, NULL);
        // if (!ply) return;
        // if (!ply_read_header(ply)) return;
        // nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
        // ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
        // ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
        // ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);
        // cout << nvertices << endl << ntriangles << endl;
        // if (!ply_read(ply)) return;
        // ply_close(ply);
		// plyReady = true;
    } else {
		cout << "Argumento faltando!!!" << endl;
		// exit(EXIT_FAILURE);
	}
}

void doAnimate() 
{
    glTranslatef(xT, yT, zT);
    // glRotatef(aR,1,1,1);
    glRotatef(aXR,1,0,0);
    glRotatef(aYR,0,1,0);
    glRotatef(aZR,0,0,1);
    glScalef(xS, yS, zS);
}

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    // cout << "(" << w << "," << h << ")" << endl;
     // Para previnir uma divisão por zero
	if ( h == 0 ) h = 1;

	// Especifica o tamanho da viewport
    glViewport(0, 0, w, h);
    
 
	// Calcula a correção de aspecto
	fAspect = (GLfloat)w/(GLfloat)h;

    xMax = (w/40);
    yMax = (h/40);

    glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos do teclado
void GerenciaTeclado(unsigned char key, int x, int y)
{   
    // cout << "Tecla " << key << " apertada.\n";     
    switch(key)
    {
        // esc
        case 27:
            if (fs) {
                fs = false;
                glutReshapeWindow(windowWidth, windowHeight);
            } 
        break;
        
        // enter
        case 13:
            if (!fs) {
                fs = true;
                glutFullScreen();
            }
        break;
        case 'r':
        case 'R':
            // reset values
            xT = 0, yT = 0, zT = 0;
            xS = 1.0, yS = 1.0, zS = 1.0;
            aXR = 0, aYR = 0, aZR = 0;
            eyeX = 0, eyeY=0, eyeZ=Z_EYE;
            cX=0, cY=0, cZ=0;
            upX = 0, upY = 1, upZ=0;
            angle = ANGLE_CHECK;
        break;

        // Translate
        case 't': 
            --xT;
            cout << "xT: " << xT << endl;
        break;
        case 'T':
            ++xT;
            cout << "xT: " << xT << endl;
        break;
        case 'y':
            --yT;
            cout << "yT: " << yT << endl;
        break;
        case 'Y':
            ++yT;
            cout << "yT: " << yT << endl;
        break;
        case 'u':
            --zT;
            cout << "zT: " << zT << endl;
        break;
        case 'U':
            ++zT;
            cout << "zT: " << zT << endl;
        break;

        // Rotação
        case 'g':
            --aXR;
            cout  << "aXR: " << aXR << endl;
        break;
        case 'G':
            ++aXR;
            cout  << "aXR: " << aXR << endl;
        break;
        case 'h':
            --aYR;
            cout  << "aYR: " << aYR << endl;
        break;
        case 'H':
            ++aYR;
            cout  << "aYR: " << aYR << endl;
        break;
        case 'j':
            --aZR;
            cout  << "aZR: " << aZR << endl;
        break;
        case 'J':
            ++aZR;
            cout  << "aZR: " << aZR << endl;
        break;

        // Scalling
        case 'b':
            --xS;
            cout  << "xS: " << xS << endl;
        break;
        case 'B':
            ++xS;
            cout  << "xS: " << xS << endl;
        break;
        case 'n':
            --yS;
            cout  << "yS: " << yS << endl;
        break;
        case 'N':
            ++yS;
            cout << "yS: " << yS << endl;
        break;
        case 'm':
            --zS;
            cout << "zS: " << zS << endl;
        break;
        case 'M':
            ++zS;
            cout << "zS: " << zS << endl;
        break;
    };
	glutPostRedisplay();    
}

// Criacao do Menu
void CriaMenu() 
{
   
    int menu,submenu1,submenu2, submenu3, submenu31, submenu32;

	submenu1 = glutCreateMenu(MenuCorObjeto);
	glutAddMenuEntry("Vermelho",RED);
	glutAddMenuEntry("Verde",GREEN);
	glutAddMenuEntry("Azul",BLUE);
	glutAddMenuEntry("Ciano",CYAN);
	glutAddMenuEntry("Magenta",MAGENTA);
	glutAddMenuEntry("Amarelo",YELLOW);
	glutAddMenuEntry("Branco",WHITE);

    submenu2 = glutCreateMenu(MenuCorLuz1);
	glutAddMenuEntry("Vermelho",RED);
	glutAddMenuEntry("Verde",GREEN);
	glutAddMenuEntry("Azul",BLUE);
	glutAddMenuEntry("Ciano",CYAN);
	glutAddMenuEntry("Magenta",MAGENTA);
	glutAddMenuEntry("Amarelo",YELLOW);
	glutAddMenuEntry("Branco",WHITE);   
    
    submenu3 = glutCreateMenu(MenuCorLuz2);
	glutAddMenuEntry("Vermelho",RED);
	glutAddMenuEntry("Verde",GREEN);
	glutAddMenuEntry("Azul",BLUE);
	glutAddMenuEntry("Ciano",CYAN);
	glutAddMenuEntry("Magenta",MAGENTA);
	glutAddMenuEntry("Amarelo",YELLOW);
	glutAddMenuEntry("Branco",WHITE);  

    menu = glutCreateMenu(MenuPrincipal);
	glutAddSubMenu("Cor Objeto",submenu1);
    glutAddSubMenu("Cor Luz 1",submenu2);
    glutAddSubMenu("Cor Luz 2",submenu3);
    
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Gerenciamento do menu com as opções de cores           
void MenuCorObjeto(int op)
{
   switch(op) {
        case RED:
        r = 1.0f;
        g = 0.0f;
        b = 0.0f;
        break;
        case GREEN:
        r = 0.0f;
        g = 1.0f;
        b = 0.0f;
        break;
        case BLUE:
        r = 0.0f;
        g = 0.0f;
        b = 1.0f;
        break;
        case CYAN:
        r = 0.0f;
        g = 1.0f;
        b = 1.0f;
        break;
        case MAGENTA:
        r = 1.0f;
        g = 0.0f;
        b = 1.0f;
        break;
        case YELLOW:
        r = 1.0f;
        g = 1.0f;
        b = 0.0f;
        break;
		case WHITE:
		r = 1.0f;
        g = 1.0f;
        b = 1.0f;
		break;
        default:
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        break;
    }
	glutPostRedisplay();
}

// Gerenciamento do menu com as opções de cores           
void MenuCorLuz1(int op)
{
   switch(op) {
        case RED:
        r1 = 1.0f;
        g1 = 0.0f;
        b1 = 0.0f;
        break;
        case GREEN:
        r1 = 0.0f;
        g1 = 1.0f;
        b1 = 0.0f;
        break;
        case BLUE:
        r1 = 0.0f;
        g1 = 0.0f;
        b1 = 1.0f;
        break;
        case CYAN:
        r1 = 0.0f;
        g1 = 1.0f;
        b1 = 1.0f;
        break;
        case MAGENTA:
        r1 = 1.0f;
        g1 = 0.0f;
        b1 = 1.0f;
        break;
        case YELLOW:
        r1 = 1.0f;
        g1 = 1.0f;
        b1 = 0.0f;
        break;
		case WHITE:
		r1 = 1.0f;
        g1 = 1.0f;
        b1 = 1.0f;
		break;
        default:
        r1 = 1.0f;
        g1 = 1.0f;
        b1 = 1.0f;
        break;
    }

	// glutPostRedisplay();
} 

// Gerenciamento do menu com as opções de cores           
void MenuCorLuz2(int op)
{
   switch(op) {
        case RED:
        r2 = 1.0f;
        g2 = 0.0f;
        b2 = 0.0f;
        break;
        case GREEN:
        r2 = 0.0f;
        g2 = 1.0f;
        b2 = 0.0f;
        break;
        case BLUE:
        r2 = 0.0f;
        g2 = 0.0f;
        b2 = 1.0f;
        break;
        case CYAN:
        r2 = 0.0f;
        g2 = 1.0f;
        b2 = 1.0f;
        break;
        case MAGENTA:
        r2 = 1.0f;
        g2 = 0.0f;
        b2 = 1.0f;
        break;
        case YELLOW:
        r2 = 1.0f;
        g2 = 1.0f;
        b2 = 0.0f;
        break;
		case WHITE:
		r2 = 1.0f;
        g2 = 1.0f;
        b2 = 1.0f;
		break;
        default:
        r2 = 1.0f;
        g2 = 1.0f;
        b2 = 1.0f;
        break;
    }

	// glutPostRedisplay();
} 

// Gerenciamento do menu principal           
void MenuPrincipal(int op) {

}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	
	// EspecificaParametrosVisualizacao();
	// glutPostRedisplay();
}

// Inicializa parâmetros de rendering
void Inicializa ()
{   
 	// Especifica que a cor de fundo da janela ser� preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	// glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz1 );

	// Define os parâmetros da luz de número 0
	// glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente); 
	// glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa );
	// glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular );
	// glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz2 );


	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL)	;
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz de número 0
	glEnable(GL_LIGHT0);
	
	// Habilita a luz de número 1
	// glEnable(GL_LIGHT1);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

    CriaMenu();
}

// Função usada para especificar o volume de visualiza��o
void EspecificaParametrosVisualizacao(void)
{
	// Especifica sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	// Inicializa sistema de coordenadas de projeção
	glLoadIdentity();

	// Especifica a projeção perspectiva   
	gluPerspective(angle,fAspect,Z_NEAR,Z_FAR);

	// Especifica sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	glLoadIdentity();

	// Especifica posição do observador e do alvo    
    gluLookAt(eyeX, eyeY, eyeZ, cX, cY, cZ, upX, upY, upZ);
}
