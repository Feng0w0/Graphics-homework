#include<stdlib.h>
#include<GLUT.H>
#include <math.h>
#include<iostream>
using namespace std;

/*************************************************ȫ�ֱ���*************************************************/
float landWidth = 50;
/**********************************************************************************************************/

/*************************************************��������*************************************************/
void Screen();	//��������������ѩ�ˣ����ݣ���ľ������
void Screen_Land();		//���������е�����
void Wood();	//���н���������ľ׮
void Light();		//�ƹ⶯��
void Snow();		//��ѩ����
/**********************************************************************************************************/

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0); /*Specify the red, green, blue, and alpha values used when the color buffers are cleared*/
	glDepthFunc(GL_LEQUAL);		//�ڸǲ���С�ڵ��ڵıȽϷ�ʽ
	glEnable(GL_DEPTH_TEST);	//�����ڸ�
	glShadeModel(GL_SMOOTH);	//������ɫ
	//��������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 20, 100, 0, 10, 0, 0, 1, 0);	//�������������(0,50,100)λ�ã�����(0,0,0)
	glMatrixMode(GL_PROJECTION); //subsequent matrix operations are on the PROJECTION matrix
	glLoadIdentity();// load the identity matrix for the projection matrix first
	gluPerspective(45.0f, 960.0f / 720.0f, 0.1f, 200.0f); // (angle of view, aspect ratio, near, far clipping planes)
	//gluOrtho2D(0.0, 200, 0.0, 150.0);//ָ��ʹ����ͶӰ��һ��x������0~200��y����0~150��Χ�ڵľ�����������ͶӰ����ʾ������
}


void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear the color buffer,the buffers enabled for color writing*/
	glColor3f(1.0, 1.0, 1.0);// use white color to draw objects
	Screen();
	Wood();
	Light();
	Snow();
	glFlush();//force execution of GL commands
	glutPostRedisplay();
}



void NormalKeys(unsigned char normalkey, int x, int y) {
	if (normalkey == 27) {
		exit(0);
	}
}
void SpecialKeys(GLint specialkey, int x, int y) {
	switch (specialkey) {
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
		break;
	}
	return;
}
void Reshape(int w, int h) {
	h = (h == 0) ? 1 : h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	/*�ӿڵ����½�Ϊ��0,0��������봰�ڵ�ǰ������ͬ*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
}

int main(int iArgc, char** cppArgv) {

	glutInit(&iArgc, cppArgv);//initialize glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);/*use single (as opposed to double) buffer and RGB (as apposed to INDEX) color*/
	glutInitWindowSize(960, 720); // set window size, (width, height)
	glutInitWindowPosition(100, 100);  // set window position, (0,0) is the top left corner
	glutCreateWindow("POLYGON");//  name the window
	Initialize();
	glutDisplayFunc(Display);
	glutKeyboardFunc(NormalKeys);	//��ͨ�����ص�����
	glutSpecialFunc(SpecialKeys);	//���ⰴ���ص�����
	glutReshapeFunc(Reshape);		//��Ļ��С�ı�ص�����
	glutMainLoop();//put the program in an infinite event loop
	return 0;
}

void Screen() {
	Screen_Land();
}

void Wood(){}
void Light(){}
void Snow(){}

void Screen_Land() {
	glBegin(GL_QUADS);
	glColor3f(256 / 256.0, 256 / 256.0, 256 / 256.0);
	glVertex3f(-landWidth, 0.0, -landWidth);
	glVertex3f(landWidth, 0.0, -landWidth);
	glVertex3f(landWidth, 0.0, landWidth);
	glVertex3f(-landWidth, 0.0, landWidth);
	glEnd();
}