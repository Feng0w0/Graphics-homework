#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<GLUT.H>
#include <math.h>
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
using namespace std;

/*************************************************函数声明*************************************************/
void Screen();	//基础场景，包括雪人，房屋，树木，土地
void Screen_Land();		//基础场景中的土地
void Wood();	//进行交互操作的木桩
void Light();		//灯光动画
void Snow();		//下雪动画
void SnowPart();
void OneSnow();
unsigned char* LoadFileContent(const char* path, int& filesize);//读取文件，返回文件内容，把文件大小赋值给filesize     
/**********************************************************************************************************/

struct Float3       //点的位置信息和法线信息的数据类型
{
	float Data[3];     //x,y,z
};

struct Float2      //点的纹理坐标数据类型
{
	float Data[2];   //u,v
};

struct Face          //面信息
{
	int vertex[4][3];       //三个点构成一个面  每个点有三个索引信息
};
class objModel
{
public:
	objModel(const char* objFileName);
	void setTextureFromBmp(const char* texFileName);  //从obj文件创建纹理
	void objDraw();
private:
	vector<Float3> mLocation;   //位置信息
	vector<Float3> mNormal;     //法线信息
	vector<Float2> mTexcoord;   //纹理坐标信息
	vector<Face> mFace;         //面信息
	GLuint mTexture;            //模型纹理	
};

objModel::objModel(const char* objFileName)
{
	int nFileSize = 0;
	unsigned char* fileContent = LoadFileContent(objFileName, nFileSize);    //读取文件内容
	if (fileContent == nullptr)     //文件为空
	{
		return;
	}
	stringstream ssFileContent((char*)fileContent);   //流读取文件内容
	string temp;       //接受无关信息
	char szoneLine[256];        //读取一行的数据
	while (!ssFileContent.eof())
	{
		memset(szoneLine, 0, 256);        //  每次循环初始化数组szoneLine
		ssFileContent.getline(szoneLine, 256);      //流读取一行
		if (strlen(szoneLine) > 0)       //该行不为空
		{
			if (szoneLine[0] == 'v')     //v开头的数据
			{
				stringstream ssOneLine(szoneLine);        //数据存储到流中 方便赋值
				if (szoneLine[1] == 't')       //纹理信息
				{
					ssOneLine >> temp;     //接受标识符 vt
					Float2 tempTexcoord;
					ssOneLine >> tempTexcoord.Data[0] >> tempTexcoord.Data[1];   //数据存入临时变量中
					mTexcoord.push_back(tempTexcoord);         //存入容器

				}
				else if (szoneLine[1] == 'n')            //法线信息
				{
					ssOneLine >> temp;      //接收标识符vn
					Float3 tempNormal;
					ssOneLine >> tempNormal.Data[0] >> tempNormal.Data[1] >> tempNormal.Data[2];
					mNormal.push_back(tempNormal);
				}
				else                          //点的位置信息
				{
					ssOneLine >> temp;
					Float3 tempLocation;
					ssOneLine >> tempLocation.Data[0] >> tempLocation.Data[1] >> tempLocation.Data[2];
					mLocation.push_back(tempLocation);
				}
			}
			else if (szoneLine[0] == 'f')          //面信息
			{
				stringstream ssOneLine(szoneLine);     //流读取一行数据
				ssOneLine >> temp; //接收标识符f
				//    f信息    exp： f 1/1/1 2/2/2 3/3/3      位置索引/纹理索引/法线索引   三角面片 三个点构成一个面
				string vertexStr;   //接收流的内容
				Face tempFace;
				for (int i = 0; i < 4; ++i)         //每个面三个点
				{
					if (ssOneLine >> vertexStr) {           //从流中读取点的索引信息
						size_t pos = vertexStr.find_first_of('/');       //找到第一个/的位置      //即找到点的位置信息
						string locIndexStr = vertexStr.substr(0, pos);       //赋值点的位置信息
						size_t pos2 = vertexStr.find_first_of('/', pos + 1);   //找到第二个/   即找到点的纹理坐标信息
						string texIndexSrt = vertexStr.substr(pos + 1, pos2 - 1 - pos);       //赋值点的纹理坐标信息
						string norIndexSrt = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);   //赋值点的法线信息
						tempFace.vertex[i][0] = atoi(locIndexStr.c_str());        //将索引信息从 srting转换为 int     //位置索引信息赋值
						tempFace.vertex[i][1] = atoi(texIndexSrt.c_str());         //纹理坐标索引信息赋值
						tempFace.vertex[i][2] = atoi(norIndexSrt.c_str());         //法线信息赋值
					}
					else {
						tempFace.vertex[i][0] = tempFace.vertex[i - 1][0];
						tempFace.vertex[i][1] = tempFace.vertex[i - 1][1];
						tempFace.vertex[i][2] = tempFace.vertex[i - 1][2];
					}
				}
				mFace.push_back(tempFace);
			}
		}   //end 非0行
	}  //end while
	delete fileContent;
}
void objModel::objDraw()
{
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	glBegin(GL_QUADS);
	for (auto faceIndex = mFace.begin(); faceIndex != mFace.end(); ++faceIndex)         //循环遍历face信息
	{
		//第一个点的法线，纹理，位置信息
		glNormal3fv(mNormal[faceIndex->vertex[0][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[0][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[0][0] - 1].Data);
		//第二个点的法线，纹理，位置信息
		glNormal3fv(mNormal[faceIndex->vertex[1][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[1][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[1][0] - 1].Data);
		//第三个点的法线，纹理，位置信息
		glNormal3fv(mNormal[faceIndex->vertex[2][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[2][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[2][0] - 1].Data);
		//第四个点的法线，纹理，位置信息
		glNormal3fv(mNormal[faceIndex->vertex[3][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[3][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[3][0] - 1].Data);
	}
	glEnd();
}