#include "glut_ply.h"

#include "../lib/rply-1.1.4/rply.h"

// Largura e altura da janela
GLfloat xMax = WIDTH/2;
GLfloat yMax = HEIGHT/2;

GLfloat leftX = xMax;
GLfloat rightX = -xMax;
GLfloat topY = -yMax;
GLfloat bottomY = yMax;

// GLfloat nearZ = 200;
// GLfloat farZ = -1;
GLfloat nearZ = 200.0f;
GLfloat farZ = -1.0f;

GLfloat luzEspecular1[4]={0.3, 0.3, 0.3, 1.0};// "brilho" 
GLfloat luzEspecular2[4]={0.7, 0.7, 0.7, 1.0};// "brilho" 

// Capacidade de brilho do material
GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
GLint especMaterial = 60;

// Variáveis de cor da luz
GLfloat r1 = 1.0f, g1 = 1.0f, b1 = 1.0f;
GLfloat r2 = 1.0f, g2 = 1.0f, b2 = 1.0f;

// variáveis de posição da luz
GLfloat xl1 = 0.7;
GLfloat yl1 = 0.5;
GLfloat zl1 = 0.3;
// GLfloat lightPos0[] = { 1.5f, 2.5f, 0.0f, 1.0f};
GLfloat xl2 = -0.7;
GLfloat yl2 = 0.5;
GLfloat zl2 = 0.6;
// GLfloat lightPos1[] = { -0.6, -0.6, -(farZ + 0.111), 0.0f};
// tela
GLint view_w, view_h;

// variáveis da translação
GLfloat xT = 0, yT = 0, zT = 0;
// variáveis do scaling
GLfloat xS = 1.0, yS = 1.0, zS = 1.0;
// variável da rotação
GLfloat aXR = 0, aYR = 0, aZR = 0;

// taxa de tamanho
GLsizei rsize = 10;

int currentX;			// Current X position of mouse
int currentY;			// Current Y position of mouse
float trackBallRotate[3] = {0,0,0};	// Instantiate the trackball
int mState;			// Store Mouse State

bool fs = true, plyReady = false;

// Object of Class ReadPly
ReadPly * vertices;		// Stores Vertices
ReadPly * faces;		// Stores Faces
ReadPly * normals;		// Stores Normal of Each Face

long nvertices, ntriangles;

p_ply ply;
 
ReadPly::ReadPly(){};
// Set Value of Vertex of an Object
void ReadPly :: setter_vertex(int i, double value)
{
	if( i == 0)
		x = value;
	if( i == 1)
		y = value;
	if( i == 2)
		z = value;
}

// Retrieve Value of Vertex of an Object
double ReadPly :: getter( int i)
{
	if( i == 0)
		return x;
	if( i == 1)
		return y;
	if( i == 2)
		return z;
}

// Callback Function for Vertex
int vertex_cb(p_ply_argument argument) 
{
	static int index = 0;
	static int count_vertex = 0;
	long eol;
	ply_get_argument_user_data(argument, NULL, &eol);
	double temp = ply_get_argument_value(argument);


	switch (index)
	{
		case 0:
			if( leftX > temp) leftX = temp;
			if( rightX < temp) rightX = temp;
		break;
		case 1:
			if( topY < temp) topY = temp;
			if( bottomY > temp) bottomY = temp;
		break;
		case 2:
			if( nearZ > temp) nearZ = temp;
			if( farZ < temp) farZ = temp;
		break;
		default:
			break;
	}
	
	vertices[ count_vertex].setter_vertex( index++, temp); 

	if (eol) 
	{
		index = 0;
		count_vertex++;
	}
	return 1;
}

// Callback Function for Faces
int face_cb(p_ply_argument argument) 
{
	long length, value_index;
	static int index = 0;
	static int count_face = 0;
	ply_get_argument_property(argument, NULL, &length, &value_index);
	double temp;
	switch (value_index) 
	{
		case 0:
		case 1: 
			temp = ply_get_argument_value(argument);
			faces[ count_face].setter_vertex( index++, temp);
		break;
		case 2:
			temp = ply_get_argument_value(argument);
			faces[ count_face].setter_vertex( index++, temp);
			index = 0;
			count_face++;
		break;
		default:
		break;
	}
	return 1;
}

// Função callback chamada para fazer o desenho
void Desenha(void)
{
    // Limpa a janela e o depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity(); 
    
    // EspecificaParametrosVisualizacao();

    glPushMatrix();
        doAnimate();
		glTranslatef( 0, -0.1, 0);
        // Desenha o ply se tiver pronto ou o teapot
		glColor3f( 0.75, 0.75, 1.0);
		if (plyReady) {
			drawPly();
		} else {
			glutSolidCube(rsize+rsize/2);
		}
    glPopMatrix();
	lightning();
    glutSwapBuffers();
}

void lightning() {
	
	glPointSize(6);
	// Setting the Light Points
	glBegin(GL_POINTS);
		glColor3f(r1, g1, b1);
		glVertex3f(xl1, yl1, zl1);
		glColor3f(r2, g2, b2);
		glVertex3f(xl2, yl2, zl2);
	glEnd();

	GLfloat lightColor0[] = {r1, g1, b1, 1.0};
	GLfloat lightPos0[] = { xl1, yl1, zl1, 1.0};

	GLfloat lightColor1[] = { r2, g2, b2, 1.0};
	GLfloat lightPos1[] = { xl2, yl2, zl2, 1.0};

	glLightfv( GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv( GL_LIGHT0, GL_POSITION, lightPos0);	

	glLightfv( GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv( GL_LIGHT1, GL_POSITION, lightPos1);	
}

// Inicializa parâmetros de rendering
void Inicializa ()
{   
 	// Especifica que a cor de fundo da janela ser� preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable( GL_DEPTH_TEST);
	glEnable( GL_COLOR_MATERIAL);
	glEnable( GL_LIGHTING);
	glEnable( GL_LIGHT0);
	glEnable( GL_LIGHT1);

	glEnable( GL_NORMALIZE);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	GLfloat ambientColor[] = { 0.6f, 0.6f, 0.6f, 1.0f};
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientColor);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular1 );

	glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular2 );

	// Habilita a definição da cor do material a partir da cor corrente
	// glEnable(GL_COLOR_MATERIAL);

    CriaMenu();
	if (plyReady) initNormals();
}

void drawPly() {
	int n;
	glPushMatrix();
	for( n =0; n < ntriangles; n++)
	{
		glBegin(GL_POLYGON);
			glNormal3f( normals[n].getter(0),normals[n].getter(1), normals[n].getter(2) );
			int face_no = faces[n].getter(0);
			glVertex3f( vertices[face_no].getter(0), vertices[face_no].getter(1), vertices[face_no].getter(2)); 

			face_no = faces[n].getter(1);
			glVertex3f( vertices[face_no].getter(0), vertices[face_no].getter(1), vertices[face_no].getter(2)); 

			face_no = faces[n].getter(2);
			glVertex3f( vertices[face_no].getter(0), vertices[face_no].getter(1), vertices[face_no].getter(2)); 
		glEnd();
	}
	glPopMatrix(); 
}

void initNormals () {
	int i;
	double s[3], t[3], v[3], length;
	
	for( i = 0; i < ntriangles; i++)
	{
		// Finding Vectors
		s[0] = vertices[(int)faces[i].getter(1)].getter(0) - vertices[(int)faces[i].getter(0)].getter(0);
		s[1] = vertices[(int)faces[i].getter(1)].getter(1) - vertices[(int)faces[i].getter(0)].getter(1);
		s[2] = vertices[(int)faces[i].getter(1)].getter(2) - vertices[(int)faces[i].getter(0)].getter(2);
		
		t[0] = vertices[(int)faces[i].getter(2)].getter(0) - vertices[(int)faces[i].getter(0)].getter(0);
		t[1] = vertices[(int)faces[i].getter(2)].getter(1) - vertices[(int)faces[i].getter(0)].getter(1);
		t[2] = vertices[(int)faces[i].getter(2)].getter(2) - vertices[(int)faces[i].getter(0)].getter(2);
		
		// Cross Product
		v[0] = s[1] * t[2] - t[1] * s[2]; 
		v[1] = s[2] * t[0] - t[2] * s[0];
		v[2] = s[0] * t[1] - t[0] * s[1];
		
		// Normalization Factor
		length = sqrt( ( v[0] * v[0] ) + (v[1] * v[1]) + (v[2] * v[2]) );
		
		normals[i].setter_vertex(0 , (v[0] / length));
		normals[i].setter_vertex(1 , (v[1] / length));
		normals[i].setter_vertex(2 , (v[2] / length));
		
	}
}

void doAnimate() 
{
    quaternion( 'x',trackBallRotate[0]) ;
    quaternion( 'y',trackBallRotate[1]) ;
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
	// fAspect = (GLfloat)w/(GLfloat)h;

    xMax = (w/40);
    yMax = (h/40);

	leftX = xMax;
	rightX = -xMax;
	topY = -yMax;
	bottomY = yMax;

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

	glutPostRedisplay();
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

	glutPostRedisplay();
} 

// Gerenciamento do menu principal           
void MenuPrincipal(int op) {

}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) 
	{
        	mState = GLUT_LEFT_BUTTON;
        	currentX = x;
        	currentY = y;
	}
}

void quaternion(char ch,int angle)
{
    float quaternionRotateMatrix[16]={ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
    
    float vx = 0.0,vy = 0.0,vz = 0.0,qCOS,qSINx,qSINy,qSINz;
	switch(ch)
	{
	    case 'x':   vx = 1.0;
                    vy = 0.0;
                    vz = 0.0;
                    break;
		case 'y':   vx = 0.0;
                    vy = 1.0;
                    vz = 0.0;
                    break;
		case 'z':   vx = 0.0;
                    vy = 0.0;
                    vz = 1.0;
                    break;
	}
    // cout << angle << endl;
	float radAngle = M_PI * angle/180;
    qCOS = cos(radAngle/2);
	qSINx = sin(radAngle/2) * vx;
	qSINy = sin(radAngle/2) * vy;
	qSINz = sin(radAngle/2) * vz;

	quaternionRotateMatrix[0] = 1.0f - 2.0f * qSINy * qSINy - 2.0f * qSINz * qSINz ;
	quaternionRotateMatrix[4] = 2.0f * qSINx * qSINy - 2.0f * qCOS * qSINz;
	quaternionRotateMatrix[8] = 2.0f * qSINx * qSINz + 2.0f * qCOS * qSINy;
	quaternionRotateMatrix[12] = 0.0f;

	quaternionRotateMatrix[1] = 2.0f * qSINx * qSINy + 2.0f * qCOS * qSINz;
	quaternionRotateMatrix[5] = 1.0f - 2.0f * qSINx * qSINx - 2.0f * qSINz * qSINz;
	quaternionRotateMatrix[9] = 2.0f * qSINy * qSINz - 2.0f * qCOS * qSINx;
	quaternionRotateMatrix[13] = 0.0f;

	quaternionRotateMatrix[2] = 2.0f * qSINx * qSINz - 2.0f * qCOS * qSINy;
	quaternionRotateMatrix[6] = 2.0f * qSINy * qSINz + 2.0f * qCOS * qSINx;
	quaternionRotateMatrix[10] = 1.0f - 2.0f * qSINx * qSINx - 2.0f * qSINy * qSINy;
	quaternionRotateMatrix[14] = 0.0f;

	quaternionRotateMatrix[3] = 0.0f;
	quaternionRotateMatrix[7] = 0.0f;
	quaternionRotateMatrix[11] = 0.0f;
	quaternionRotateMatrix[15] = 1.0f;

	glMultMatrixf(quaternionRotateMatrix);
}

void dragRotation(int xPos, int yPos) 
{

	trackBallRotate[0] -= ((yPos - currentY) * 180) / 100;
	trackBallRotate[1] -= ((xPos - currentX) * 180) / 100;
	for (int i = 0; i < 3; i ++)
		if (trackBallRotate[i] > 360 || trackBallRotate[i] < -360)
			trackBallRotate[i] = 0.0f;
	currentX = xPos;
	currentY = yPos;
}

void mouseMotion (int x, int y) 
{
    if (mState == GLUT_LEFT_BUTTON) 
    {
        dragRotation (x, y);
        glutPostRedisplay();
    }
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
                glutReshapeWindow(WIDTH, HEIGHT);
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
            trackBallRotate[0] = 0;
            trackBallRotate[1] = 0;
            trackBallRotate[2] = 0;
			xl1 = 1.5;
			yl1 = 2.5;
			zl1 = 0;
			xl2 = -0.6;
			yl2 = -0.6;
			zl2 = 0.111;
        break;

		case 'i': 
		case 'I': 
			xS += 0.1;
            cout  << "xS: " << xS << endl;
            yS += 0.1;
            cout << "yS: " << yS << endl;
            zS += 0.1;
            cout << "zS: " << zS << endl;
			break;
		case 'o': 
		case 'O': 
			xS -= 0.1;
            cout  << "xS: " << xS << endl;
            yS -= 0.1;
            cout  << "yS: " << yS << endl;
            zS -= 0.1;
            cout << "zS: " << zS << endl;
			break;

		case 'q':
			xl1 -= 0.1;
			cout << "xl1: " << xl1 << endl;
		break;
		case 'Q':
			xl1 += 0.1;
			cout << "xl1: " << xl1 << endl;
		break;
		case 'w':
			yl1 -= 0.1;
			cout << "yl1: " << yl1 << endl;
		break;
		case 'W':
			yl1 += 0.1;
			cout << "yl1: " << yl1 << endl;
		break;
		case 'e':
			zl1 -= 0.1;
			cout << "zl1: " << zl1 << endl;
		break;
		case 'E':
			zl1 += 0.1;
			cout << "zl1: " << zl1 << endl;
		break;
		case 'a':
			xl2 -= 0.1;
			cout << "xl2: " << xl2 << endl;
		break;
		case 'A':
			xl2 += 0.1;
			cout << "xl2: " << xl2 << endl;
		break;
		case 's':
			yl2 -= 0.1;
			cout << "yl2: " << yl2 << endl;
		break;
		case 'S':
			yl2 += 0.1;
			cout << "yl2: " << yl2 << endl;
		break;
		case 'd':
			zl2 -= 0.1;
			cout << "zl2: " << zl2 << endl;
		break;
		case 'D':
			zl2 += 0.1;
			cout << "zl2: " << zl2 << endl;
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
   
    int menu,submenu1, submenu2;

	submenu1 = glutCreateMenu(MenuCorLuz1);
	glutAddMenuEntry("Vermelho",RED);
	glutAddMenuEntry("Verde",GREEN);
	glutAddMenuEntry("Azul",BLUE);
	glutAddMenuEntry("Ciano",CYAN);
	glutAddMenuEntry("Magenta",MAGENTA);
	glutAddMenuEntry("Amarelo",YELLOW);
	glutAddMenuEntry("Branco",WHITE);

    submenu2 = glutCreateMenu(MenuCorLuz2);
	glutAddMenuEntry("Vermelho",RED);
	glutAddMenuEntry("Verde",GREEN);
	glutAddMenuEntry("Azul",BLUE);
	glutAddMenuEntry("Ciano",CYAN);
	glutAddMenuEntry("Magenta",MAGENTA);
	glutAddMenuEntry("Amarelo",YELLOW);
	glutAddMenuEntry("Branco",WHITE);     

    menu = glutCreateMenu(MenuPrincipal);
    glutAddSubMenu("Cor Luz 1",submenu1);
    glutAddSubMenu("Cor Luz 2",submenu2);
    
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void StartArgs(int argc, char* argv[]) {	

 	// Checando o número de parâmetros

    if (argc >= 2) {
		cout << "Argumento detectado: " << argv[1] << endl;

		// Open Ply File and Get Handle
		ply = ply_open(argv[1], NULL, 0, NULL);

        if (!ply){
            cout << "Falha na leitura!\nInforme corretamente o caminho para o arquivo: ";
            string another;
            cin >> another;
            ply = ply_open(another.c_str(), NULL, 0, NULL);
            if (!ply) return;
            if (!ply_read_header(ply)) return;
        } else if (!ply_read_header(ply)) {
            cout << "Falha na leitura dos cabecalhos!\n";
            return;
        }

    } else {
		cout << "Argumento faltando!!!\nInforme corretamente o caminho para o arquivo: ";
        string str;
        cin >> str;
        ply = ply_open(str.c_str(), NULL, 0, NULL);
        if (!ply){
            cout << "Falha na leitura!\n";
            return;
        }
        if (!ply_read_header(ply)) {
            cout << "Falha na leitura dos cabecalhos!\n";
            return;
        }
	}

	// Read 1st Element type and specified properties
	nvertices = ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
	ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
	
	// Read 2nd Element type and specified properties
	ntriangles = ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);

	cout << "Nº de vertices: " << nvertices << "\nNº de Faces: " << ntriangles << endl;

	faces = new ReadPly[ntriangles];
	vertices = new ReadPly[nvertices];
	normals = new ReadPly[ntriangles];

	// Read all vertices and faces
	if (!ply_read(ply)) {
		// matar quem precisar
		delete[] faces;
		delete[] vertices;
		return;
	} 
	plyReady =  true;

	// Close Ply File
	ply_close(ply);
}