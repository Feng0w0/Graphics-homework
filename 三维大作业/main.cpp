#include"Mode.h"

/*************************************************全局变量*************************************************/
float landWidth = 50;	//土地宽度
float cameraX = 0;		//相机旋转角度
float woodX = 0, woodY = 0, woodZ = 0, woodDegree = 0, woodScaleX = 1, woodScaleY = 1, woodScaleZ = 1;	//木桩的初始位置，旋转角度和放缩比例
float disWoodX = 0, disWoodY = 0, disWoodZ = 0;		//木桩的移动位置
bool mouseLeftDown=false;		//鼠标左键是否按下
int mouseX = 0, mouseY = 0;		//鼠标位置
objModel a("Squirtle.obj");
objModel tree("Tree.obj");
objModel cottage("cottage.obj");
/**********************************************************************************************************/

GLfloat lightPos[] = { 0.0f, 100.0f, 0.0f, 1.0f };
GLfloat spotDir[] = { 0.0f, 0.0f, 1.0f };
void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0); /*Specify the red, green, blue, and alpha values used when the color buffers are cleared*/
	glDepthFunc(GL_LEQUAL);		//遮盖采用小于等于的比较方式
	glEnable(GL_DEPTH_TEST);	//开启遮盖
	glShadeModel(GL_SMOOTH);	//顶点着色
	//光照设置
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 20, 100, 0, 0, 0, 0, 1, 0);	//将摄像机放置于(0,50,100)位置，看向(0,0,0)
	glMatrixMode(GL_PROJECTION); //subsequent matrix operations are on the PROJECTION matrix
	glLoadIdentity();// load the identity matrix for the projection matrix first
	gluPerspective(45.0f, 960.0f / 720.0f, 0.1f, 200.0f); // (angle of view, aspect ratio, near, far clipping planes)
	//gluOrtho2D(0.0, 200, 0.0, 150.0);//指定使用正投影将一个x坐标在0~200，y坐标0~150范围内的矩形坐标区域投影到显示器窗口
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
	gluLookAt(sin(cameraX/180*3.1415)*100, 20, cos(cameraX / 180 * 3.1415) * 100, 0, 0, 0, 0, 1, 0);
	glFlush();//force execution of GL commands
}



void NormalKeys(unsigned char normalkey, int x, int y) {
	if (normalkey == 27) {
		exit(0);
	}
	else if (normalkey == 'q') {	//摄像机顺时针转
		cameraX -= 1;
	}
	else if (normalkey == 'e') {	//摄像机逆时针转
		cameraX += 1;
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

	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(-25, 0, -25);
	glScalef(10, 10, 10);
	a.objDraw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(25, 0, -25);
	glScalef(10, 10, 10);
	a.objDraw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glScalef(0.005, 0.005, 0.005);
	tree.objDraw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0);
	glScalef(4, 4, 4);
	//cottage.objDraw();
	glPopMatrix();
}

void Wood(){
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
	int a;
}
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

