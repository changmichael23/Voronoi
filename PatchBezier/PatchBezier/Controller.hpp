#pragma once
#include "Patch.hpp"
#include "glew.h"
#include "freeglut.h"

extern float TimeInSeconds;
extern int TimeSinceAppStartedInMS;
extern int OldTime;
extern float DeltaTime;
extern float heightStep;
extern float rotateStep;
extern float translateStep;
extern float resizeStep;
extern Patch tmp;

float COS(float angle)
{
	return cos(angle * 2 * 0.00872665);
}
float SIN(float angle)
{
	return sin(angle * 2 * 0.00872665);
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		if (CamType == 0)
		{
			posZ += moveSpeed *  DeltaTime * COS(rotY);
			posX += moveSpeed *  DeltaTime * -SIN(rotY);
			posY += moveSpeed *  DeltaTime * SIN(rotX);
		}
		else
		{
			distance += moveSpeed *  DeltaTime;
		}
	}
	if (key == 'a')
	{
		posZ += moveSpeed *  DeltaTime * COS(rotY - 90);
		posX += moveSpeed *  DeltaTime * -SIN(rotY - 90);

	}
	if (key == 's')
	{
		if (CamType == 0)
		{
			posZ -= moveSpeed *  DeltaTime * COS(rotY);
			posX -= moveSpeed *  DeltaTime * -SIN(rotY);
			posY -= moveSpeed *  DeltaTime * SIN(rotX);
		}
		else
		{
			distance -= moveSpeed *  DeltaTime;
		}
	}
	if (key == 'd')
	{
		posZ -= moveSpeed *  DeltaTime * COS(rotY - 90);
		posX -= moveSpeed *  DeltaTime * -SIN(rotY - 90);
	}
	if (key == 27)	// Echap
	{
		exit(0);
	}
	if (key == 32)	// Space Bar
	{
		posY -= moveSpeed *  DeltaTime;
	}
	if (key == 'f')
	{
		posY += moveSpeed *  DeltaTime;
	}
	if (key == '1')	 // 1 Mode FPS
	{
		ChangeCam(0);
	}
	if (key == '2')	 // 2 Mode Orbit
	{
		ChangeCam(1);
	}

	if (key == 'o')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == 'p')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void mouse(int x, int y)
{

	//rotX = (float)(y-height*0.5f) * rotSpeed;
	//rotY = (float)(x-width*0.5f) * rotSpeed;
	if (abs(y - lastposY) < 10 && abs(x - lastposX) < 10)
	{
		rotX += (y - lastposY)* rotSpeed;
		rotY += (x - lastposX)* rotSpeed;
	}
	lastposX = x;
	lastposY = y;

}

void SpecialInput(int key, int x, int y)
{	
	if (gs == drawing)
	{
		switch (key)
		{
		case GLUT_KEY_RIGHT:
			++pointIdx;
			if (pointIdx == nbPoints)
			{
				pointIdx = 0;
			}
			break;
		case GLUT_KEY_LEFT:
			--pointIdx;
			if (pointIdx < 0)
			{
				pointIdx = nbPoints - 1;
			}
			break;
		case GLUT_KEY_UP:
			tmpPatch.MovePoint(pointIdx, heightStep);
			break;
		case GLUT_KEY_DOWN:
			tmpPatch.MovePoint(pointIdx, -heightStep);
			break;
		}
	}

	if (gs == editing || gs == deleting)
	{
		switch (key)
		{
		case GLUT_KEY_RIGHT:
			++patchIdx;
			if (patchIdx == patches.size())
			{
				patchIdx = 0;
			}
			break;
		case GLUT_KEY_LEFT:
			--patchIdx;
			if (patchIdx < 0)
			{
				patchIdx = patches.size() - 1;
			}
			break;
		}

		if (gs == editing)
		{
			switch (key)
			{
			case GLUT_KEY_UP:
				switch (em)
				{
				case 0:
					patches.at(patchIdx).Rotate(0, rotateStep);
					break;
				case 1:
					patches.at(patchIdx).Rotate(1, rotateStep);
					break;
				case 2:
					patches.at(patchIdx).Rotate(2, rotateStep);
					break;
				case 3:
					patches.at(patchIdx).Translate(0, translateStep);
					break;
				case 4:
					patches.at(patchIdx).Translate(1, translateStep);
					break;
				case 5:
					patches.at(patchIdx).Translate(2, translateStep);
					break;
				case 6:
					patches.at(patchIdx).Resize(resizeStep);
					break;
				}
			case GLUT_KEY_DOWN:
				switch (em)
				{
				case 0:
					patches.at(patchIdx).Rotate(0, -rotateStep);
					break;
				case 1:
					patches.at(patchIdx).Rotate(1, -rotateStep);
					break;
				case 2:
					patches.at(patchIdx).Rotate(2, -rotateStep);
					break;
				case 3:
					patches.at(patchIdx).Translate(0, -translateStep);
					break;
				case 4:
					patches.at(patchIdx).Translate(1, -translateStep);
					break;
				case 5:
					patches.at(patchIdx).Translate(2, -translateStep);
					break;
				case 6:
					patches.at(patchIdx).Resize(1 / resizeStep);
					break;
				}
			}
		}

		if (gs == deleting)
		{
			switch (key)
			{
			case GLUT_KEY_DOWN:
				ConfirmDelete();
				break;
			}
		}
	}

	glutPostRedisplay();
}