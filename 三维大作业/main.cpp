#include"Mode.h"

/*************************************************全局变量*************************************************/
float landWidth = 50;	//土地宽度
float cameraDegree = 0,R=100;		//相机旋转角度和旋转半径
float woodX = 0, woodY = 0, woodZ = 30, woodDegree = 0, woodScaleX = 1, woodScaleY = 1, woodScaleZ = 1;	//木桩的初始位置，旋转角度和放缩比例
float disWoodX = 0, disWoodY = 0, disWoodZ = 0;		//木桩的移动位置
bool mouseLeftDown=false;		//鼠标左键是否按下
int mouseX = 0, mouseY = 0;		//鼠标位置
float lightDegree = 0;		//光线旋转角度
objModel mode("Squirtle.obj");	//乌龟
objModel tree("tree1.obj");			//树
objModel cottage("cottage.obj");//村舍
GLfloat lightPos[] = { -1.0f, 0.0f, 0.0f,0.0f };
GLfloat ambientLight[] = { 214/256.0f, 236/256.0f, 240/256.0f, 1.0f };
GLfloat specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };
float number[120];//40朵雪花的x，y，z
float r = 0.1;	//雪花下降
int snowDegree = 0;	//雪花旋转
bool snowDown = true;
/**********************************************************************************************************/


void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0); /*Specify the red, green, blue, and alpha values used when the color buffers are cleared*/
	glDepthFunc(GL_LEQUAL);		//遮盖采用小于等于的比较方式
	glEnable(GL_DEPTH_TEST);	//开启遮盖
	glShadeModel(GL_SMOOTH);	//顶点着色
	//光照设置
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 20, 100, 0, 0, 0, 0, 1, 0);	//将摄像机放置于(0,50,100)位置，看向(0,0,0)
	glMatrixMode(GL_PROJECTION); //subsequent matrix operations are on the PROJECTION matrix
	glLoadIdentity();// load the identity matrix for the projection matrix first
	gluPerspective(45.0f, 960.0f / 720.0f, 0.1f, 200.0f); // (angle of view, aspect ratio, near, far clipping planes)
	//gluOrtho2D(0.0, 200, 0.0, 150.0);//指定使用正投影将一个x坐标在0~200，y坐标0~150范围内的矩形坐标区域投影到显示器窗口

	//雪花初始化
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
	//场景搭建
	Screen();
	Wood();
	Light();
	Snow();
	//摄像机控制
	glLoadIdentity();
	gluLookAt(sin(cameraDegree/180*3.1415)*R, 20, cos(cameraDegree / 180 * 3.1415) * R, 0, 20, 0, 0, 1, 0);
	glFlush();//force execution of GL commands
}



void NormalKeys(unsigned char normalkey, int x, int y) {
	if (normalkey == 27) {
		exit(0);
	}
	else if (normalkey == 'q') {	//摄像机顺时针转
		cameraDegree -= 1;
	}
	else if (normalkey == 'e') {	//摄像机逆时针转
		cameraDegree += 1;
	}
	else if (normalkey == 17) {	//ctrl+q木桩顺时针转
		int mod = glutGetModifiers();
		if (mod == GLUT_ACTIVE_CTRL) {
			woodDegree -= 1;
		}
	}
	else if (normalkey == 5) {	//ctrl+e木桩逆时针转
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
	//上下键是木桩Z轴拉伸，左右键是木桩XY轴同时拉伸
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
	/*视口的左下角为（0,0），宽高与窗口当前参数相同*/
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
		//木桩复原
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
	glutKeyboardFunc(NormalKeys);	//普通按键回调函数
	glutSpecialFunc(SpecialKeys);	//特殊按键回调函数
	glutReshapeFunc(Reshape);		//屏幕大小改变回调函数
	glutMouseFunc(Mouse);			//检测鼠标单击
	glutMotionFunc(MouseMotion);		//检测鼠标拖动
	glutMainLoop();//put the program in an infinite event loop
	return 0;
}

void Screen() {
	Screen_Land();
	//雪人1
	GLfloat mat_ambient[] = { 0.174500, 0.011750, 0.011750, 0.550000 };
	GLfloat mat_diffuse[] = { 0.614240, 0.041360, 0.041360, 0.550000};
	GLfloat mat_specular[] = { 0.727811, 0.626959, 0.626959, 0.550000};
	GLfloat mat_shininess[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范围内
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
	//雪人2
	glPushMatrix();
	glTranslatef(25, 0, 40);
	glScalef(2, 2, 2);
	mode.objDraw();
	glPopMatrix();
	//树1
	GLfloat mat_ambient2[] = { 0.021500, 0.174500, 0.021500, 0.550000};
	GLfloat mat_diffuse2[] = { 0.075680, 0.614240, 0.075680, 0.550000 };
	GLfloat mat_specular2[] = { 0.633000, 0.727811, 0.633000, 0.550000 };
	GLfloat mat_shininess2[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范围内
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
	//树2
	glPushMatrix();
	glTranslatef(-35, 0, -35);
	glRotatef(-90, 1, 0, 0);
	glScalef(5, 5, 5);
	tree.objDraw();
	glPopMatrix();
	//村庄1
	GLfloat mat_ambient3[] = { 0.191250, 0.073500, 0.022500, 1.000000 };
	GLfloat mat_diffuse3[] = { 0.703800, 0.270480, 0.082800, 1.000000 };
	GLfloat mat_specular3[] = { 0.256777, 0.137622, 0.086014, 1.000000 };
	GLfloat mat_shininess3[] = { 12.800000 }; //材质RGBA镜面指数，数值在0～128范围内
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
	//村庄2
	glPushMatrix();
	glTranslatef(30, 0, 10);
	glRotatef(90, 0, 1, 0);
	glScalef(3, 3, 3);
	cottage.objDraw();
	glPopMatrix();
	//村庄3
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
	GLfloat mat_shininess[] = { 51.200001 }; //材质RGBA镜面指数，数值在0～128范围内
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glPushMatrix();
	glTranslatef(woodX + disWoodX, woodY + disWoodY, woodZ + disWoodZ);	//木桩位置
	glRotatef(woodDegree, 0, 1, 0);	//木桩旋转
	glScalef(woodScaleX, woodScaleY, woodScaleZ);	//木桩放缩
	//木桩绘制
	glBegin(GL_QUADS);
	glColor3f(0 / 256.0, 0 / 256.0, 0 / 256.0);
	//底面
	glNormal3f(0, -1, 0);
	glVertex3f(-2.0, 0.0, -2.0);
	glVertex3f(2.0, 0.0, -2.0);
	glVertex3f(2.0, 0.0, 2.0);
	glVertex3f(-2.0, 0.0, 2.0);
	//顶面
	glNormal3f(0, 1, 0);
	glVertex3f(-2.0, 5.0, -2.0);
	glVertex3f(2.0, 5.0, -2.0);
	glVertex3f(2.0, 5.0, 2.0);
	glVertex3f(-2.0, 5.0, 2.0);
	//前
	glNormal3f(0, 0, 1);
	glColor3f(256.0 / 256.0, 0 / 256.0, 0 / 256.0);
	glVertex3f(-2.0, 0.0, 2.0);
	glVertex3f(2.0, 0.0, 2.0);
	glVertex3f(2.0, 5.0, 2.0);
	glVertex3f(-2.0, 5.0, 2.0);
	//后
	glNormal3f(0, 0, -1);
	glVertex3f(-2.0, 0.0, -2.0);
	glVertex3f(2.0, 0.0, -2.0);
	glVertex3f(2.0, 5.0, -2.0);
	glVertex3f(-2.0, 5.0, -2.0);
	//左
	glNormal3f(-1, 0, 0);
	glVertex3f(-2.0, 0.0, -2.0);
	glVertex3f(-2.0, 0.0, 2.0);
	glVertex3f(-2.0, 5.0, 2.0);
	glVertex3f(-2.0, 5.0, -2.0);
	//右
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
		GLfloat mat_shininess[] = { 51.200001 }; //材质RGBA镜面指数，数值在0～128范围内
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
	GLfloat mat_shininess[] = { 38.400002 }; //材质RGBA镜面指数，数值在0～128范围内
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
	FILE* pFile = fopen(path, "rb");   //二进制方式读取
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);      //将文件指针移动到文件末尾
		int nLen = ftell(pFile);        //距离文件头部的距离   //这里指文件大小
		if (nLen > 0)
		{
			rewind(pFile);          //移动到文件头部
			fileContent = new unsigned char[nLen + 1];
			//为文件指针开辟空间
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			//将pFile的内容读入fileContent
			fileContent[nLen] = '\0';         //文件末尾加上\0
			filesize = nLen;                  //为文件大小赋值
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