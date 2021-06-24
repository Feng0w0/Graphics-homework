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

/*************************************************��������*************************************************/
void Screen();	//��������������ѩ�ˣ����ݣ���ľ������
void Screen_Land();		//���������е�����
void Wood();	//���н���������ľ׮
void Light();		//�ƹ⶯��
void Snow();		//��ѩ����
unsigned char* LoadFileContent(const char* path, int& filesize);//��ȡ�ļ��������ļ����ݣ����ļ���С��ֵ��filesize     
/**********************************************************************************************************/

struct Float3       //���λ����Ϣ�ͷ�����Ϣ����������
{
	float Data[3];     //x,y,z
};

struct Float2      //�������������������
{
	float Data[2];   //u,v
};

struct Face          //����Ϣ
{
	int vertex[3][3];       //�����㹹��һ����  ÿ����������������Ϣ
};
class objModel
{
public:
	objModel(const char* objFileName);
	void setTextureFromBmp(const char* texFileName);  //��obj�ļ���������
	void objDraw();
private:
	vector<Float3> mLocation;   //λ����Ϣ
	vector<Float3> mNormal;     //������Ϣ
	vector<Float2> mTexcoord;   //����������Ϣ
	vector<Face> mFace;         //����Ϣ
	GLuint mTexture;            //ģ������	
};

objModel::objModel(const char* objFileName)
{
	int nFileSize = 0;
	unsigned char* fileContent = LoadFileContent(objFileName, nFileSize);    //��ȡ�ļ�����
	if (fileContent == nullptr)     //�ļ�Ϊ��
	{
		return;
	}
	stringstream ssFileContent((char*)fileContent);   //����ȡ�ļ�����
	string temp;       //�����޹���Ϣ
	char szoneLine[256];        //��ȡһ�е�����
	while (!ssFileContent.eof())
	{
		memset(szoneLine, 0, 256);        //  ÿ��ѭ����ʼ������szoneLine
		ssFileContent.getline(szoneLine, 256);      //����ȡһ��
		if (strlen(szoneLine) > 0)       //���в�Ϊ��
		{
			if (szoneLine[0] == 'v')     //v��ͷ������
			{
				stringstream ssOneLine(szoneLine);        //���ݴ洢������ ���㸳ֵ
				if (szoneLine[1] == 't')       //������Ϣ
				{
					ssOneLine >> temp;     //���ܱ�ʶ�� vt
					Float2 tempTexcoord;
					ssOneLine >> tempTexcoord.Data[0] >> tempTexcoord.Data[1];   //���ݴ�����ʱ������
					mTexcoord.push_back(tempTexcoord);         //��������

				}
				else if (szoneLine[1] == 'n')            //������Ϣ
				{
					ssOneLine >> temp;      //���ձ�ʶ��vn
					Float3 tempNormal;
					ssOneLine >> tempNormal.Data[0] >> tempNormal.Data[1] >> tempNormal.Data[2];
					mNormal.push_back(tempNormal);
				}
				else                          //���λ����Ϣ
				{
					ssOneLine >> temp;
					Float3 tempLocation;
					ssOneLine >> tempLocation.Data[0] >> tempLocation.Data[1] >> tempLocation.Data[2];
					mLocation.push_back(tempLocation);
				}
			}
			else if (szoneLine[0] == 'f')          //����Ϣ
			{
				stringstream ssOneLine(szoneLine);     //����ȡһ������
				ssOneLine >> temp; //���ձ�ʶ��f
				//    f��Ϣ    exp�� f 1/1/1 2/2/2 3/3/3      λ������/��������/��������   ������Ƭ �����㹹��һ����
				string vertexStr;   //������������
				Face tempFace;
				for (int i = 0; i < 3; ++i)         //ÿ����������
				{
					ssOneLine >> vertexStr;           //�����ж�ȡ���������Ϣ
					size_t pos = vertexStr.find_first_of('/');       //�ҵ���һ��/��λ��      //���ҵ����λ����Ϣ
					string locIndexStr = vertexStr.substr(0, pos);       //��ֵ���λ����Ϣ
					size_t pos2 = vertexStr.find_first_of('/', pos + 1);   //�ҵ��ڶ���/   ���ҵ��������������Ϣ
					string texIndexSrt = vertexStr.substr(pos + 1, pos2 - 1 - pos);       //��ֵ�������������Ϣ
					string norIndexSrt = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);   //��ֵ��ķ�����Ϣ
					tempFace.vertex[i][0] = atoi(locIndexStr.c_str());        //��������Ϣ�� srtingת��Ϊ int     //λ��������Ϣ��ֵ
					tempFace.vertex[i][1] = atoi(texIndexSrt.c_str());         //��������������Ϣ��ֵ
					tempFace.vertex[i][2] = atoi(norIndexSrt.c_str());         //������Ϣ��ֵ
				}
				mFace.push_back(tempFace);
			}
		}   //end ��0��
	}  //end while
	delete fileContent;
}
void objModel::objDraw()
{
	//glBindTexture(GL_TEXTURE_2D, mTexture);
	glBegin(GL_TRIANGLES);
	for (auto faceIndex = mFace.begin(); faceIndex != mFace.end(); ++faceIndex)         //ѭ������face��Ϣ
	{
		//��һ����ķ��ߣ�����λ����Ϣ
		glNormal3fv(mNormal[faceIndex->vertex[0][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[0][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[0][0] - 1].Data);
		//�ڶ�����ķ��ߣ�����λ����Ϣ
		glNormal3fv(mNormal[faceIndex->vertex[1][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[1][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[1][0] - 1].Data);
		//��������ķ��ߣ�����λ����Ϣ
		glNormal3fv(mNormal[faceIndex->vertex[2][2] - 1].Data);
		glTexCoord2fv(mTexcoord[faceIndex->vertex[2][1] - 1].Data);
		glVertex3fv(mLocation[faceIndex->vertex[2][0] - 1].Data);
	}
	glEnd();
}