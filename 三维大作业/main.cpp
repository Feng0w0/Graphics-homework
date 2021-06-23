#include<stdlib.h>
#include<GLUT.H>
#include <math.h>
#include<iostream>
using namespace std;

/*************************************************ȫ�ֱ���*************************************************/
float landWidth = 50;	//���ؿ��
float cameraX = 0;		//�����ת�Ƕ�
float woodX = 0, woodY = 0, woodZ = 0, woodDegree = 0, woodScaleX = 1, woodScaleY = 1, woodScaleZ = 1;	//ľ׮�ĳ�ʼλ�ã���ת�ǶȺͷ�������
float disWoodX = 0, disWoodY = 0, disWoodZ = 0;		//ľ׮���ƶ�λ��
bool mouseLeftDown=false;		//�������Ƿ���
int mouseX = 0, mouseY = 0;		//���λ��
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
	gluLookAt(0, 20, 100, 0, 0, 0, 0, 1, 0);	//�������������(0,50,100)λ�ã�����(0,0,0)
	glMatrixMode(GL_PROJECTION); //subsequent matrix operations are on the PROJECTION matrix
	glLoadIdentity();// load the identity matrix for the projection matrix first
	gluPerspective(45.0f, 960.0f / 720.0f, 0.1f, 200.0f); // (angle of view, aspect ratio, near, far clipping planes)
	//gluOrtho2D(0.0, 200, 0.0, 150.0);//ָ��ʹ����ͶӰ��һ��x������0~200��y����0~150��Χ�ڵľ�����������ͶӰ����ʾ������
}


void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear the color buffer,the buffers enabled for color writing*/
	glColor3f(1.0, 1.0, 1.0);// use white color to draw objects
	//�����
	Screen();
	Wood();
	Light();
	Snow();
	//���������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(sin(cameraX/180*3.1415)*100, 20, cos(cameraX / 180 * 3.1415) * 100, 0, 0, 0, 0, 1, 0);
	glFlush();//force execution of GL commands
}



void NormalKeys(unsigned char normalkey, int x, int y) {
	if (normalkey == 27) {
		exit(0);
	}
	else if (normalkey == 'q') {	//�����˳ʱ��ת
		cameraX -= 1;
	}
	else if (normalkey == 'e') {	//�������ʱ��ת
		cameraX += 1;
	}
	else if (normalkey == 17) {	//ctrl+qľ׮˳ʱ��ת
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL) {
			woodDegree -= 1;
		}
	}
	else if (normalkey == 5) {	//ctrl+eľ׮��ʱ��ת
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL) {
			woodDegree += 1;
		}
	}
	glutPostRedisplay();
}
void SpecialKeys(GLint specialkey, int x, int y) {
	//���¼���ľ׮Z�����죬���Ҽ���ľ׮XY��ͬʱ����
	switch (specialkey) {
	case GLUT_KEY_UP:
		woodScaleY += 0.1;
		break;
	case GLUT_KEY_DOWN:
		woodScaleY -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		woodScaleX += 0.1;
		woodScaleZ += 0.1;
		break;
	case GLUT_KEY_RIGHT:
		woodScaleX -= 0.1;
		woodScaleZ -= 0.1;
		break;
	}
	glutPostRedisplay();
}
void Reshape(int w, int h) {
	h = (h == 0) ? 1 : h;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	/*�ӿڵ����½�Ϊ��0,0��������봰�ڵ�ǰ������ͬ*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
}

void Mouse(int button,int state,int x,int y) {
	mouseX = x;
	mouseY = y;
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP) {
			mouseLeftDown = false;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		//ľ׮��ԭ
		woodDegree = 0;
		disWoodX = 0;
		disWoodY = 0;
		disWoodZ = 0;
		woodScaleX = 1;
		woodScaleY = 1;
		woodScaleZ = 1;
	}
	glutPostRedisplay();
}

void MouseMotion(int x, int y) {
	if (mouseLeftDown) {
		disWoodX += (float)(x - mouseX)/10;
		disWoodZ += (float)(y - mouseY)/10;
		mouseX = x;
		mouseY = y;
	}
	glutPostRedisplay();
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
	glutMouseFunc(Mouse);			//�����굥��
	glutMotionFunc(MouseMotion);		//�������϶�
	glutMainLoop();//put the program in an infinite event loop
	return 0;
}

void Screen() {
	Screen_Land();
}

void Wood(){
	glPushMatrix();
	glTranslatef(woodX + disWoodX, woodY + disWoodY, woodZ + disWoodZ);	//ľ׮λ��
	glRotatef(woodDegree, 0, 1, 0);	//ľ׮��ת
	glScalef(woodScaleX, woodScaleY, woodScaleZ);	//ľ׮����
	//ľ׮����
	glBegin(GL_QUADS);
	glColor3f(0 / 256.0, 0 / 256.0, 0 / 256.0);
	//����
	glNormal3f(0, -1, 0);
	glVertex3f(-2.0, 0.0, -2.0);
	glVertex3f(2.0, 0.0, -2.0);
	glVertex3f(2.0, 0.0, 2.0);
	glVertex3f(-2.0, 0.0, 2.0);
	//����
	glNormal3f(0, 1, 0);
	glVertex3f(-2.0, 5.0, -2.0);
	glVertex3f(2.0, 5.0, -2.0);
	glVertex3f(2.0, 5.0, 2.0);
	glVertex3f(-2.0, 5.0, 2.0);
	//ǰ
	glNormal3f(0, 0, 1);
	glColor3f(256.0 / 256.0, 0 / 256.0, 0 / 256.0);
	glVertex3f(-2.0, 0.0, 2.0);
	glVertex3f(2.0, 0.0, 2.0);
	glVertex3f(2.0, 5.0, 2.0);
	glVertex3f(-2.0, 5.0, 2.0);
	//��
	glNormal3f(0, 0, -1);
	glVertex3f(-2.0, 0.0, -2.0);
	glVertex3f(2.0, 0.0, -2.0);
	glVertex3f(2.0, 5.0, -2.0);
	glVertex3f(-2.0, 5.0, -2.0);
	//��
	glNormal3f(-1, 0, 0);
	glVertex3f(-2.0, 0.0, -2.0);
	glVertex3f(-2.0, 0.0, 2.0);
	glVertex3f(-2.0, 5.0, 2.0);
	glVertex3f(-2.0, 5.0, -2.0);
	//��
	glNormal3f(1, 0, 0);
	glVertex3f(2.0, 0.0, -2.0);
	glVertex3f(2.0, 0.0, 2.0);
	glVertex3f(2.0, 5.0, 2.0);
	glVertex3f(2.0, 5.0, -2.0);
	glEnd();
	glPopMatrix();
}
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