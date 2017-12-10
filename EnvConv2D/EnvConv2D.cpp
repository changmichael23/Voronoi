#include "glew.h"
#include "Fonctions.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib") 
#endif
#include <iostream>

#include "freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm> 
#define PI 3.14159265


std::vector<Point2D> points;
std::vector<Point2D> p;

int menuPrincipal;
int isDrawing;
void mouse(int button, int state, int x, int y);
void SpecialInput(int key, int x, int y);
void showMenu();
void EnveloppeMenuCallback(int);
void dessin();
void update();

int main(int argc, char **argv)
{

	isDrawing = 1;

	// Initialisation de glut et creation de la fenetre
	// ------------------------------------------------
	glutInit(&argc, argv);											// Initialisation
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);		// Mode d'affichage RGB, et test prafondeur

	glutInitWindowSize(500, 500);									// Dimension fenêtre
	glutInitWindowPosition(100, 100);								// Position coin haut gauche
	glutCreateWindow("Bezier");										// Nom
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0);						// Repère 2D délimitant les abscisses et les ordonnées


																	// Initialisation d'OpenGL
																	// -----------------------
	glClearColor(1.0, 1.0, 1.0, 0.50);
	glColor3f(1.0, 1.0, 1.0);			     	 // couleur: blanc
	glPointSize(2.0);							 // taille d'un point: 2px

												 /* Enregistrement des fonctions de rappel
												 => initialisation des fonctions callback appelées par glut */
	

	showMenu();
	glutMouseFunc(mouse);
	glutDisplayFunc(dessin);
	glutIdleFunc(update);

	/* rq: le callback de fonction (fonction de rappel) est une fonction qui est passée en argument à une
	autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en même temps)
	alors qu'il ne les connaît pas par avance.*/



	/* Entrée dans la boucle principale de glut, traitement des évènements */
	
	glutMainLoop();								  // lancement de la boucle de réception des évènements
	return 0;
}

void update()
{
	glutPostRedisplay();
}

void dessin()
{
	glClearColor(1.0, 1.0, 1.0, 0.50);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++)
	{
		
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();

	if (isDrawing == 0)
	{
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
		for (int k = 0; k < p.size(); k++)
		{
			
			glVertex2f(p[k].x, p[k].y);

		}

		glEnd();

	}
	glutSwapBuffers();
}


void mouse(int button, int state, int x, int y)
{
	Point2D tmp;

	// Si on appuie sur le bouton de gauche

	
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && isDrawing == 1)
		{
			tmp.x = x - 250.0f;
			tmp.y = -y + 250.0f;
			points.push_back(tmp);
		}
	
}


void showMenu()
{
	glutDestroyMenu(menuPrincipal);

	menuPrincipal = glutCreateMenu(EnveloppeMenuCallback);

	glutAddMenuEntry("Jarvis", 1);
	glutAddMenuEntry("Graham-Scan", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void EnveloppeMenuCallback(int menuItem)
{
	switch (menuItem)
	{
	case 1:
		// Jarvis
		
		p = jarvisMarch(points);
		isDrawing = 0;
		break;
	case 2:
		// Graham Scan

		isDrawing = 0;
		break;
	}
	showMenu();
}
