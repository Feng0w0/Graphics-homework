#include"Mode.h"

/*************************************************ȫ�ֱ���*************************************************/
float landWidth = 50;	//���ؿ��
float cameraDegree = 0,R=100;		//�����ת�ǶȺ���ת�뾶
float woodX = 0, woodY = 0, woodZ = 30, woodDegree = 0, woodScaleX = 1, woodScaleY = 1, woodScaleZ = 1;	//ľ׮�ĳ�ʼλ�ã���ת�ǶȺͷ�������
float disWoodX = 0, disWoodY = 0, disWoodZ = 0;		//ľ׮���ƶ�λ��
bool mouseLeftDown=false;		//�������Ƿ���
int mouseX = 0, mouseY = 0;		//���λ��
float lightDegree = 0;		//������ת�Ƕ�
objModel mode("Squirtle.obj");	//�ڹ�
objModel tree("tree1.obj");			//��
objModel cottage("cottage.obj");//����
GLfloat lightPos[] = { -1.0f, 0.0f, 0.0f,0.0f };
GLfloat ambientLight[] = { 214/256.0f, 236/256.0f, 240/256.0f, 1.0f };
GLfloat specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float number[120];//40��ѩ����x��y��z
float r = 0.1;	//ѩ���½�
int snowDegree = 0;	//ѩ����ת
bool snowDown = true;
/**********************************************************************************************************/


void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0); /*Specify the red, green, blue, and alpha values used when the color buffers are cleared*/
	glDepthFunc(GL_LEQUAL);		//�ڸǲ���С�ڵ��ڵıȽϷ�ʽ
	glEnable(GL_DEPTH_TEST);	//�����ڸ�
	glShadeModel(GL_SMOOTH);	//������ɫ
	//��������
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 20, 100, 0, 0, 0, 0, 1, 0);	//�������������(0,50,100)λ�ã�����(0,0,0)
	glMatrixMode(GL_PROJECTION); //subsequent matrix operations are on the PROJECTION matrix
	glLoadIdentity();// load the identity matrix for the projection matrix first
	gluPerspective(45.0f, 960.0f / 720.0f, 0.1f, 200.0f); // (angle of view, aspect ratio, near, far clipping planes)
	//gluOrtho2D(0.0, 200, 0.0, 150.0);//ָ��ʹ����ͶӰ��һ��x������0~200��y����0~150��Χ�ڵľ�����������ͶӰ����ʾ������

	//ѩ����ʼ��
	for (int i = 0; i < 40; i++) {
		number[i] = rand() % 100 - 50;
	}
	for (int i = 40; i < 80; i++) {
		number[i] = 70 + rand() % 100;
	}
	for (int i = 80; i < 120; i++) {
		number[i] = rand() % 100 - 50;
	}
}


void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear the color buffer,the buffers enabled for color writing*/
	glColor3f(1.0, 1.0, 1.0);// use white color to draw objects
	glMatrixMode(GL_MODELVIEW);
	//�����
	Screen();
	Wood();
	Light();
	Snow();
	//���������
	glLoadIdentity();
	gluLookAt(sin(cameraDegree/180*3.1415)*R, 20, cos(cameraDegree / 180 * 3.1415) * R, 0, 20, 0, 0, 1, 0);
	glFlush();//force execution of GL commands
}



void NormalKeys(unsigned char normalkey, int x, int y) {
	if (normalkey == 27) {
		exit(0);
	}
	else if (normalkey == 'q') {	//�����˳ʱ��ת
		cameraDegree -= 1;
	}
	else if (normalkey == 'e') {	//�������ʱ��ת
		cameraDegree += 1;
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
	else if (normalkey == 'z') {
		lightDegree -= 2;
	}
	else if (normalkey == 'w') {
		R--;
	}
	else if (normalkey == 's') {
		R++;
	}
	else if (normalkey == 'x') {
		snowDown = !snowDown;
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
	//ѩ��1
	GLfloat mat_ambient[] = { 0.174500, 0.011750, 0.011750, 0.550000 };
	GLfloat mat_diffuse[] = { 0.614240, 0.041360, 0.041360, 0.550000};
	GLfloat mat_specular[] = { 0.727811, 0.626959, 0.626959, 0.550000};
	GLfloat mat_shininess[] = { 76.800003 }; //����RGBA����ָ������ֵ��0��128��Χ��
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(-25, 0, 40);
	glScalef(2, 2, 2);
	mode.objDraw();
	glPopMatrix();
	//ѩ��2
	glPushMatrix();
	glTranslatef(25, 0, 40);
	glScalef(2, 2, 2);
	mode.objDraw();
	glPopMatrix();
	//��1
	GLfloat mat_ambient2[] = { 0.021500, 0.174500, 0.021500, 0.550000};
	GLfloat mat_diffuse2[] = { 0.075680, 0.614240, 0.075680, 0.550000 };
	GLfloat mat_specular2[] = { 0.633000, 0.727811, 0.633000, 0.550000 };
	GLfloat mat_shininess2[] = { 76.800003 }; //����RGBA����ָ������ֵ��0��128��Χ��
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess2);
	glPushMatrix();
	glTranslatef(35, 0, -35);
	glRotatef(-90, 1, 0, 0);
	glScalef(5, 5, 5);
	tree.objDraw();
	glPopMatrix();
	//��2
	glPushMatrix();
	glTranslatef(-35, 0, -35);
	glRotatef(-90, 1, 0, 0);
	glScalef(5, 5, 5);
	tree.objDraw();
	glPopMatrix();
	//��ׯ1
	GLfloat mat_ambient3[] = { 0.191250, 0.073500, 0.022500, 1.000000 };
	GLfloat mat_diffuse3[] = { 0.703800, 0.270480, 0.082800, 1.000000 };
	GLfloat mat_specular3[] = { 0.256777, 0.137622, 0.086014, 1.000000 };
	GLfloat mat_shininess3[] = { 12.800000 }; //����RGBA����ָ������ֵ��0��128��Χ��
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular3);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess3);
	glPushMatrix();
	glTranslatef(0, 0, -30);
	glRotatef(180, 0, 1, 0);
	glScalef(3, 3, 3);
	cottage.objDraw();
	glPopMatrix();
	//��ׯ2
	glPushMatrix();
	glTranslatef(30, 0, 10);
	glRotatef(90, 0, 1, 0);
	glScalef(3, 3, 3);
	cottage.objDraw();
	glPopMatrix();
	//��ׯ3
	glPushMatrix();
	glTranslatef(-30, 0, 10);
	glRotatef(270, 0, 1, 0);
	glScalef(3, 3, 3);
	cottage.objDraw();
	glPopMatrix();
}

void Wood(){
	//GLfloat mat_ambient[] = { 0.192250, 0.192250, 0.192250, 1.000000 };
	GLfloat mat_diffuse[] = { 128/256.0, 64/256.0, 0.0, 1.000000 };
	GLfloat mat_specular[] = { 128 / 256.0, 64 / 256.0, 0.0, 1.000000 };
	GLfloat mat_shininess[] = { 51.200001 }; //����RGBA����ָ������ֵ��0��128��Χ��
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
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
void Light(){
	glPushMatrix();
	glRotatef(lightDegree, 0, 0, 1);
	lightDegree -= 0.05;
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	if (lightDegree == -180) {
		//glDisable(GL_LIGHT0);
	}
	if (lightDegree == -360) {
		lightDegree = 0;
		//glEnable(GL_LIGHT0);
	}
	glPopMatrix();
	glutPostRedisplay();
}
void Snow(){
	for (int i = 0; i < 40; i++) {
		glPushMatrix();
		if (number[i + 40]< 0) {
			if (!snowDown)continue;
			number[i] = rand() % 100 - 50;
			number[i + 40] = rand() % 100 + 70;
			number[i + 80] = rand() % 100 - 50;
		}
		glTranslatef(number[i], number[i + 40], number[i + 80]);
		number[i + 40] -= r;
		glRotatef((number[i+80]/number[i])/100 * snowDegree++, 0, 0, 1);
		glRotatef((number[i] / number[i + 80]) / 100 * snowDegree, 1, 0, 0);

		GLfloat mat_ambient[] = { 0.192250, 0.192250, 0.192250, 1.000000 };
		GLfloat mat_diffuse[] = { 0.507540, 0.507540, 0.507540, 1.000000 };
		GLfloat mat_specular[] = { 0.508273, 0.508273, 0.508273, 1.000000 };
		GLfloat mat_shininess[] = { 51.200001 }; //����RGBA����ָ������ֵ��0��128��Χ��
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		glScalef(0.1, 0.1, 0.1);
		OneSnow();
		glPopMatrix();
	}
}

void Screen_Land() {
	GLfloat mat_ambient[] = { 0.053750, 0.050000, 0.066250, 0.820000 };
	GLfloat mat_diffuse[] = { 0.182750, 0.170000, 0.225250, 0.820000 };
	GLfloat mat_specular[] = { 0.332741, 0.328634, 0.346435, 0.820000 };
	GLfloat mat_shininess[] = { 38.400002 }; //����RGBA����ָ������ֵ��0��128��Χ��
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glBegin(GL_QUADS);
	glColor3f(256 / 256.0, 256 / 256.0, 256 / 256.0);
	glNormal3f(0, 1, 0);
	glVertex3f(-landWidth, 0.0, -landWidth);
	glVertex3f(landWidth, 0.0, -landWidth);
	glVertex3f(landWidth, 0.0, landWidth);
	glVertex3f(-landWidth, 0.0, landWidth);
	glEnd();
}

unsigned char* LoadFileContent(const char* path, int& filesize)
{
	unsigned char* fileContent = nullptr;
	filesize = 0;
	FILE* pFile = fopen(path, "rb");   //�����Ʒ�ʽ��ȡ
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);      //���ļ�ָ���ƶ����ļ�ĩβ
		int nLen = ftell(pFile);        //�����ļ�ͷ���ľ���   //����ָ�ļ���С
		if (nLen > 0)
		{
			rewind(pFile);          //�ƶ����ļ�ͷ��
			fileContent = new unsigned char[nLen + 1];
			//Ϊ�ļ�ָ�뿪�ٿռ�
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			//��pFile�����ݶ���fileContent
			fileContent[nLen] = '\0';         //�ļ�ĩβ����\0
			filesize = nLen;                  //Ϊ�ļ���С��ֵ
		}
		fclose(pFile);
	}
	return fileContent;
}

void OneSnow() {
	for (int i = 0; i < 6; i++) {
		glRotatef(60, 0, 0, 1);
		SnowPart();
		glScalef(1, -1, 1);
		SnowPart();
		glScalef(1, -1, 1);
	}
}
void SnowPart() {
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(0.0, 0.0, 0);
	glVertex3f(1.0, 1.0, 0);
	glVertex3f(16.0, 1.0, 0);
	glVertex3f(17.0, 0.0, 0);

	glVertex3f(10.0, 4.0, 0);
	glVertex3f(11.0, 4.0, 0);
	glVertex3f(7.0, 1.0, 0);
	glVertex3f(6.0, 1.0, 0);

	glVertex3f(15.0, 3.0, 0);
	glVertex3f(16.0, 3.0, 0);
	glVertex3f(13.0, 1.0, 0);
	glVertex3f(12.0, 1.0, 0);
	glEnd();

	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	glVertex3f(0.0, 0.0, 0);
	glVertex3f(1.0, 1.0, 0);
	glVertex3f(16.0, 1.0, 0);
	glVertex3f(17.0, 0.0, 0);

	glVertex3f(10.0, 4.0, 0);
	glVertex3f(11.0, 4.0, 0);
	glVertex3f(7.0, 1.0, 0);
	glVertex3f(6.0, 1.0, 0);

	glVertex3f(15.0, 3.0, 0);
	glVertex3f(16.0, 3.0, 0);
	glVertex3f(13.0, 1.0, 0);
	glVertex3f(12.0, 1.0, 0);
	glEnd();
}