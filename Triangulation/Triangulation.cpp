#include "glew.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib") 
#endif
#include <iostream>

#include "freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm> 
#define PI 3.14159265

float r0;
float r1;
struct Apex;
struct Edge;
struct Triangle;


typedef struct Point
{
	float x;
	float y;
} Point;

typedef struct Apex
{
	Point* p;
	Edge* a;

	Apex() : p(nullptr), a(nullptr) {};

	Apex(Point* point) : p(point), a(nullptr) {};
} Apex;

typedef struct Edge
{
	Apex* s1;
	Apex* s2;
	Triangle* td;
	Triangle* tg;

	Edge() : s1(nullptr), s2(nullptr), td(nullptr), tg(nullptr) {};

	Edge(Apex* s1, Apex* s2) : s1(s1), s2(s2), td(nullptr), tg(nullptr) {};
} Edge;

typedef struct Bounded_Edge
{
	Point p1;
	Point p2;

	Bounded_Edge(Point p1, Point p2) : p1(p1), p2(p2) {};
} Bounded_Edge;

typedef struct Triangle
{
	Edge* a1;
	Edge* a2;
	Edge* a3;

	Triangle() : a1(nullptr), a2(nullptr), a3(nullptr) {};

	Triangle(Edge* e1, Edge* e2, Edge* e3) : a1(e1), a2(e2), a3(e3) {};
} Triangle;

typedef struct Circle
{
	Point center;
	float radius;
} Circle;

struct points_comparator
{
	inline bool operator() (const Point& p1, const Point& p2)
	{
		if (p1.x < p2.x) return true;
		if (p1.x > p2.x) return false;
		if (p1.y < p2.y) return true;
		return false;
	}
};

std::vector<Apex> apexes;
std::vector<Edge> edges;
std::vector<Triangle> triangles;
std::map<Apex*, std::vector<Bounded_Edge>> voronoi_regions;

float colinearity_tolerance = 0.0001;

std::vector<Point> points;
std::vector<Point> points1;
std::vector<Point> p;
std::vector<Point> p1;

int bezier2;
int typeRaccord;
int isTransvecting;

void bezier1();
void raccord();
int n = 10;
int menuPrincipal = 0;
int isDrawing;
int bezierfin;
std::vector<Point> neew;
int isRotating;
int isTranslating;
std::vector<Point> translate;
std::vector<Point> rotate;
std::vector<Point> transvect;
int isCreating2ndCurve;
void dessinBezier();
void bezier();
void mouse(int button, int state, int x, int y);
void SpecialInput(int key, int x, int y);
void showMenu();
void mainMenuCallback(int);
void drawingMenuCallback(int);
void secondBezierMenuCallback(int);
void transformMenuCallback(int);
void rMenuCallback(int);
void tMenuCallback(int);
void cMenuCallback(int);
void translating();
void rotating();
void mouse_activeFunc(int, int);
void raccordBezierMenuCallback(int);
int inBezier2;
void colorMenuCallBack(int);
int colorSubMenu;
std::vector<float> r;
std::vector<float> g;
std::vector<float> b;
float colorr=0.0f;
float colorg = 0.0f;
float colorb = 0.0f;
void transvecting();
void triangulate();
bool isTriangleDirect(Triangle);
bool isTriangleLeft(Triangle, Edge*);
float determinant(Apex*, Apex*, Apex*);

int main(int argc, char **argv)
{
	r.push_back(0.0f);

	g.push_back(0.0f);

	b.push_back(0.0f);

	isDrawing = 0;
	bezierfin = 0;
	isRotating = 0;
	isTranslating = 0;
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
	glutMotionFunc(mouse_activeFunc);
	glutSpecialFunc(SpecialInput);
	glutDisplayFunc(dessinBezier);


	/* rq: le callback de fonction (fonction de rappel) est une fonction qui est passée en argument à une
	autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en même temps)
	alors qu'il ne les connaît pas par avance.*/



	/* Entrée dans la boucle principale de glut, traitement des évènements */
	
	glutMainLoop();								  // lancement de la boucle de réception des évènements
	return 0;
}
void dessinBezier()
{
	glClearColor(1.0, 1.0, 1.0, 0.50);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0, 0.0f);

	for (int i = 0; i < points.size(); i++)
	{
		glBegin(GL_POINTS);
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
	glutSwapBuffers();
	glColor3f(colorr, colorg,colorb);
	
		for (int k = 0; k < p.size(); k++)
		{
			
			glBegin(GL_POLYGON);
			glBegin(GL_LINE_STRIP);
			
			glVertex2f(p[k].x, p[k].y);

		}

		glEnd();
		glutSwapBuffers();
		glColor3f(1.0f, 0, 0.0f);
		for (int i = 0; i < points1.size(); i++)
		{
			glBegin(GL_POINTS);
			glVertex2f(points1[i].x, points1[i].y);
		}
		glEnd();
		glutSwapBuffers();
		if(r.size()>=1)
		glColor3f(r[r.size()-1], g[g.size() - 1], b[b.size() - 1]);

		for (int k = 0; k < p1.size(); k++)
		{

			glBegin(GL_POLYGON);
			glBegin(GL_LINE_STRIP);

			glVertex2f(p1[k].x, p1[k].y);

		}
	

	glEnd();
	glutSwapBuffers();
}

void bezier()
{
	float t;
	Point tmp[50];
	
	p = neew;
	for (int k = 0; k < n; k++)
	{
		for (int ij = 0; ij < points.size(); ij++)
		{
			tmp[ij] = points[ij];
		}


		t = (float)k / n;
		for (int j = 1; j <= points.size() -1; j++)
		{
			for (int i = 0; i <= points.size() - 1 - j; i++)
			{
				tmp[i].x = (1-t)*tmp[i].x + t*tmp[i + 1].x;
				tmp[i].y = (1 - t)*tmp[i].y + t*tmp[i + 1].y;

			}
		}

		p.push_back(tmp[0]);


	}
	
	p.push_back(points.at(points.size()-1));
	bezierfin = 1;
}

void mouse_activeFunc(int x, int y)
{
	float delta=2.0f;
	for (int i = 0; i < points.size(); i++)
	{
		if (x - 250.0f <= points[i].x + delta && x - 250.0f >= points[i].x - delta && -y + 250.0f <= points[i].y + delta &&-y + 250.0f >= points[i].y - delta)
		{
			points[i].x = x - 250.0f;

			points[i].y = -y + 250.0f;
			bezier();
			dessinBezier();

		}
	}
	for (int i = 0; i < points1.size(); i++)
	{
		if (x - 250.0f <= points1[i].x + delta && x - 250.0f >= points1[i].x - delta && -y + 250.0f <= points1[i].y + delta &&-y + 250.0f >= points1[i].y - delta)
		{
			points1[i].x = x - 250.0f;

			points1[i].y = -y + 250.0f;
			bezier1();
			dessinBezier();

		}
	}
}

void mouse(int button, int state, int x, int y)
{
	Point tmp;

	// Si on appuie sur le bouton de gauche

	
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && bezierfin == 0 && isDrawing == 1)
		{
			tmp.x = x - 250.0f;
			tmp.y = -y + 250.0f;
			points.push_back(tmp);
		}
		else
		{
			if (button == GLUT_LEFT_BUTTON && state == GLUT_UP &&bezierfin == 1 && isTranslating == 1)
			{
				tmp.x = x - 250.0f;
				tmp.y = -y + 250.0f;
				translate.push_back(tmp);
			}
			if (button == GLUT_LEFT_BUTTON && state == GLUT_UP &&bezierfin == 1 && isRotating == 1)
			{
				tmp.x = x - 250.0f;
				tmp.y = -y + 250.0f;
				rotate.push_back(tmp);
			}
			if (button == GLUT_LEFT_BUTTON && state == GLUT_UP &&bezierfin == 1 && isTransvecting == 1)
			{
				tmp.x = x - 250.0f;
				tmp.y = -y + 250.0f;
				transvect.push_back(tmp);
			}
			if (button == GLUT_LEFT_BUTTON && state == GLUT_UP &&bezierfin == 1 && inBezier2 == 1)
			{
				
				tmp.x = x - 250.0f;
				tmp.y = -y + 250.0f;
				points1.push_back(tmp);
				Point tmp1;
				if (points1.size() == 1)
				{

					switch (typeRaccord)
					{
						case 1:
							std::cout << "Enter r0 and r1 :" << std::endl;
							std::cin >> r0 >> r1;
							std::cout << r0 << std::endl;
							std::cout << r1 << std::endl;
							tmp1.x = points1[0].x + (points[points.size()-1].x-points[points.size()-2].x)*r1/r0;
							tmp1.y = points1[0].y + (points[points.size() - 1].y - points[points.size() - 2].y)*r1 / r0;
							points1.push_back(tmp1);
							break;
						case 2:
							std::cout << "Enter r0 and r1 :" << std::endl;
							std::cin >> r0 >> r1;
							std::cout << r0 << std::endl;
							std::cout << r1 << std::endl;
							tmp1.x = points1[0].x + (points[points.size() - 1].x - points[points.size() - 2].x)*r1 / r0;
							tmp1.y = points1[0].y + (points[points.size() - 1].y - points[points.size() - 2].y)*r1 / r0;
							points1.push_back(tmp1);

							Point D;
							D.x = points[points.size() - 2].x + (points[points.size() - 2].x - points[points.size() - 3].x)*r1 / r0 - points[points.size() - 1].x + points1[0].x;
							D.y = points[points.size() - 2].y + (points[points.size() - 2].y - points[points.size() - 3].y)*r1 / r0 - points[points.size() - 1].y + points1[0].y;

							Point tmp2;
							tmp2.x = points1[1].x + (points1[1].x - D.x)*r1 / r0;
							tmp2.y = points1[1].y + (points1[1].y - D.y)*r1 / r0;
							points1.push_back(tmp2);

							break;
					}
				}
			}

		}
	
}

void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		n++;
		
		bezier();
		if(bezier2==1)bezier1();
		break;
	case GLUT_KEY_DOWN:
		n--;
		bezier();
		if (bezier2 == 1)bezier1();
		break;
	case GLUT_KEY_SHIFT_L:
		for (int i = 0; i < points.size(); i++)
		{
			points[i].x *= 1.1;
			points[i].y *=1.1;
		}
		for (int i = 0; i < points1.size(); i++)
		{
			points1[i].x *= 1.1;
			points1[i].y *= 1.1;
		}
		bezier();
		if (bezier2 == 1)bezier1();
		break;
	case GLUT_KEY_CTRL_L:
		for (int i = 0; i < points.size(); i++)
		{
			points[i].x /=1.1;
			points[i].y /= 1.1;
		}
		for (int i = 0; i < points1.size(); i++)
		{
			points1[i].x /= 1.1;
			points1[i].y /= 1.1;
		}
		bezier();
		if (bezier2 == 1)bezier1();
		break;
	}

	glutPostRedisplay();
}

void showMenu()
{
	glutDestroyMenu(menuPrincipal);
	if (isDrawing == 0)
	{
		
		colorSubMenu = glutCreateMenu(colorMenuCallBack);
		glutAddMenuEntry("Blanc", 11);
		glutAddMenuEntry("Jaune", 12);
		glutAddMenuEntry("Vert", 13);
		glutAddMenuEntry("Noir", 14);
		glutAddMenuEntry("Bleu", 15);
		glutAddMenuEntry("Orange", 16);
		glutAddMenuEntry("Violet", 17);

		menuPrincipal = glutCreateMenu(mainMenuCallback);

		glutAddSubMenu("Couleur", colorSubMenu);
		glutAddMenuEntry("Points de controle", 1);


		

		
	}
	else if (isDrawing == 1 && bezierfin == 0)
	{
		menuPrincipal = glutCreateMenu(drawingMenuCallback);
		glutAddMenuEntry("Terminer le tracé", 1);
		glutAddMenuEntry("Annuler le tracé", 2);
		glutAddMenuEntry("Trianguler", 3);
	}
	else
	{
		if (bezierfin == 1 && isRotating == 0 && isTranslating == 0)
		{
			menuPrincipal = glutCreateMenu(transformMenuCallback);
			glutAddMenuEntry("Rotation", 1);
			glutAddMenuEntry("Translation", 2);
			glutAddMenuEntry("Transvection", 3);
			if (bezier2 == 0)
			{
			glutAddSubMenu("Couleur", colorSubMenu);
			glutAddMenuEntry("Tracer C0", 4);
			glutAddMenuEntry("Tracer C1", 5);
			glutAddMenuEntry("Tracer C2", 6);
			}
		}

		if (bezierfin == 1 && isRotating == 1 && isTranslating == 0)
		{
			menuPrincipal = glutCreateMenu(rMenuCallback);
			glutAddMenuEntry("Terminer le tracé", 1);
			glutAddMenuEntry("Annuler le tracé", 2);
		}
		if (bezierfin == 1 && isRotating == 0 && isTranslating == 1)
		{
			menuPrincipal = glutCreateMenu(tMenuCallback);
			glutAddMenuEntry("Terminer le tracé", 1);
			glutAddMenuEntry("Annuler le tracé", 2);
		}
		if (bezierfin == 1 && isRotating == 0 && isTranslating == 0 && isTransvecting==1)
		{
			menuPrincipal = glutCreateMenu(cMenuCallback);
			glutAddMenuEntry("Terminer le tracé", 1);
			glutAddMenuEntry("Annuler le tracé", 2);
		}
		if (bezierfin == 1 && bezier2 == 0 && isRotating == 0 && isTranslating == 0 && inBezier2 == 1) {
			
			menuPrincipal = glutCreateMenu(secondBezierMenuCallback);
			
			glutAddMenuEntry("Terminer le tracé", 1);
			glutAddMenuEntry("Annuler le tracé", 2);
		}
		if (bezierfin == 1 && bezier2 == 1 && isRotating == 0 && inBezier2 == 1) {
			menuPrincipal = glutCreateMenu(raccordBezierMenuCallback);
			glutAddMenuEntry("Raccord", 1);
		}
		

	}

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mainMenuCallback(int menuItem)
{


	isDrawing = 1;
	showMenu();
}

void drawingMenuCallback(int menuItem)
{
	switch (menuItem)
	{
	case 1:
		if (inBezier2 == 0) {
			colorr = r[r.size() - 1];
			colorg = g[g.size() - 1];
			colorb = b[b.size() - 1];
		}
		bezier();
		dessinBezier();
		glEnd();
		glutSwapBuffers();
		break;
	case 2:
		points = neew;
		glEnd();
		glutSwapBuffers();
		break;
	case 3:
		triangulate();
		glEnd();
		glutSwapBuffers();
		break;
	}
	showMenu();
}

void transformMenuCallback(int menuItem)
{
	switch (menuItem)
	{
	case 1:
		isTransvecting = 0;
		isRotating = 1;
		isTranslating = 0;
		isCreating2ndCurve = 0;
		glEnd();
		glutSwapBuffers();
		break;
	case 2:
		isTransvecting = 0;
		isRotating = 0;
		isTranslating = 1;
		isCreating2ndCurve = 0;
		glEnd();
		glutSwapBuffers();
		break;
	case 4:
		inBezier2 = 1;
		isRotating = 0;
		isTranslating = 0;
		isTransvecting = 0;
		isCreating2ndCurve = 1;
		typeRaccord = 0;
		glEnd();
		glutSwapBuffers();
		break;
	case 5:
		isTransvecting = 0;
		inBezier2 = 1;
		isRotating = 0;
		isTranslating = 0;
		isCreating2ndCurve = 1;
		typeRaccord = 1;
		glEnd();
		glutSwapBuffers();
		break;
	case 6:
		isTransvecting = 0;
		inBezier2 = 1;
		isRotating = 0;
		isTranslating = 0;
		isCreating2ndCurve = 1;
		typeRaccord = 2;
		glEnd();
		glutSwapBuffers();
		break;
	case 3:
		isRotating = 0;
		isTranslating = 0;
		isTransvecting = 1;
		isCreating2ndCurve = 0;
		glEnd();
		glutSwapBuffers();
		break;
	}
	showMenu();
}

void tMenuCallback(int menuItem)
{
	switch (menuItem)
	{
	case 1:
		isTranslating = 0;
		translating();
		dessinBezier();
		glEnd();
		glutSwapBuffers();
		break;
	case 2:
		translate = neew;
		isTranslating = 0;
		glEnd();
		glutSwapBuffers();
		break;
	}
	showMenu();
}

void cMenuCallback(int menuItem)
{
	switch (menuItem)
	{
	case 1:
		isTransvecting = 0;
		transvecting();
		dessinBezier();
		glEnd();
		glutSwapBuffers();
		break;
	case 2:
		transvect = neew;
		isTransvecting = 0;
		glEnd();
		glutSwapBuffers();
		break;
	}
	showMenu();
}

void rMenuCallback(int menuItem)
{
	switch (menuItem)
	{
	case 1:
		isRotating = 0;
		rotating();
		dessinBezier();
		glEnd();
		glutSwapBuffers();
		break;
	case 2:
		rotate = neew;
		isRotating = 0;
		glEnd();
		glutSwapBuffers();
		break;
	}
	showMenu();
}

void secondBezierMenuCallback(int menuItem)
{
	switch (menuItem)
	{
	case 1:
		bezier1();
		dessinBezier();
		glEnd();
		glutSwapBuffers();
		break;
	case 2:
		points1 = neew;
		glEnd();
		glutSwapBuffers();
		break;
	}
	showMenu();
}

void raccordBezierMenuCallback(int menuItem)
{
	switch (menuItem)
	{
	case 1:
		raccord();
		dessinBezier();
		glEnd();
		glutSwapBuffers();
		break;
	}
	showMenu();
}

void translating()
{
	Point vecTranslate;
	
	vecTranslate.x = translate[translate.size()-1].x- translate[translate.size() - 2].x;
	vecTranslate.y = translate.at(translate.size() - 1).y - translate.at(translate.size() - 2).y;

	for (int i = 0; i < points.size(); i++)
	{
		points.at(i).x += vecTranslate.x;
		points.at(i).y += vecTranslate.y;
	}
	for (int i = 0; i < points1.size(); i++)
	{
		points1.at(i).x += vecTranslate.x;
		points1.at(i).y += vecTranslate.y;
	}
	bezier();
	if (bezier2 == 1) bezier1();
}

void rotating()
{
	Point oa;
	Point ob;
	Point tmpp;
	float co;
	float theta;

	oa.x = rotate[rotate.size() - 2].x;
	oa.y = rotate[rotate.size() - 2].y;
	
	ob.x = rotate[rotate.size() - 1].x;
	ob.y = rotate[rotate.size() - 1].y;
	
	co = (oa.x*ob.x + oa.y*ob.y) / (sqrt(pow(oa.x, 2) + pow(oa.y, 2))*sqrt(pow(ob.x, 2) + pow(ob.y, 2)));
	theta= acos(co);
	std::cerr << theta * 180 / PI << " "<< theta<<std::endl;
	std::vector<Point> tab,tab1;
	for (int j = 0; j < points.size(); j++)
	{
		tab.push_back(points[j]);
		
	}

	for (int j = 0; j < points1.size(); j++)
	{
		tab1.push_back(points1[j]);

	}

	for (int k = 0; k < theta * 180 / PI; k++)
	{
		float t= k * PI / 180.0f;
		for (int i = 0; i < points.size(); i++)
		{
			tmpp.x = tab[i].x;
			tmpp.y = tab[i].y;
			points.at(i).x = cos(t)*tmpp.x - sin(t)*tmpp.y;
			points.at(i).y = sin(t)*tmpp.x + cos(t)*tmpp.y;



		}
	
		for (int i = 0; i < points1.size(); i++)
		{
			tmpp.x = tab1[i].x;
			tmpp.y = tab1[i].y;
			points1.at(i).x = cos(t)*tmpp.x - sin(t)*tmpp.y;
			points1.at(i).y = sin(t)*tmpp.x + cos(t)*tmpp.y;



		}
		if(bezier2==1) bezier1();
		bezier();
		dessinBezier();
		Sleep(10);
	}
	

}

void raccord()
{
	inBezier2 = 0;
	int i = points.size();
	for (int j = 1; j < points1.size(); j++)
	{
		Point tmp;
		tmp.x = points1[j].x + points[i - 1].x - points1[0].x;

		tmp.y = points1[j].y + points[i - 1].y - points1[0].y;
		
		points1[j] = tmp;
	}

	

	for (int j = 0; j < p1.size(); j++)
	{
		Point tmp;
		tmp.x = p1[j].x + points[i - 1].x - points1[0].x;

		tmp.y = p1[j].y + points[i - 1].y - points1[0].y;

		p1[j] = tmp;
	}
	points1[0].x = points[i - 1].x;

	points1[0].y = points[i - 1].y;

	dessinBezier();
}

void bezier1() {
	
		float t;
		Point tmp[50];

		p1 = neew;
		for (int k = 0; k < n; k++)
		{
			for (int ij = 0; ij < points1.size(); ij++)
			{
				tmp[ij] = points1[ij];
			}


			t = (float)k / n;
			for (int j = 1; j <= points1.size() - 1; j++)
			{
				for (int i = 0; i <= points1.size() - 1 - j; i++)
				{
					tmp[i].x = (1 - t)*tmp[i].x + t*tmp[i + 1].x;
					tmp[i].y = (1 - t)*tmp[i].y + t*tmp[i + 1].y;

				}
			}

			p1.push_back(tmp[0]);


		}


		p1.push_back(points1.at(points1.size() - 1));
		bezier2 = 1;

	
}

void colorMenuCallBack(int a)
{
	
		switch (a)
		{
		case 12:
			
			r.push_back(1.0);
			g.push_back(1.0);
			b.push_back(0.0);
			break;
		case 13:
			
			r.push_back(0.0f);
			g.push_back(1.0f);
			b.push_back(0.0f);
			break;
		case 14:
			r.push_back(0.0f);
			g.push_back(0.0f);
			b.push_back(0.0f);
			break;
		case 15:
			r.push_back(0.0f);
			g.push_back(0.0f);
			b.push_back(1.0f);
			break;
		case 16:
			r.push_back(1.0f);
			g.push_back(0.5f);
			b.push_back(0.0f);
			break;
		case 17:
			r.push_back(1.0f);
			g.push_back(0.0f);
			b.push_back(1.0f);
			break;
		default:
			r.push_back(1.0f);
			g.push_back(1.0f);
			b.push_back(1.0f);
			break;
		}
	}

void transvecting()
{
		Point vecTranslate;
		float tmp;
		vecTranslate.x = transvect[transvect.size() - 1].x - transvect[transvect.size() - 2].x;
		vecTranslate.y = transvect.at(transvect.size() - 1).y - transvect.at(transvect.size() - 2).y;

		for (int i = 0; i < points.size(); i++)
		{
			tmp = points.at(i).x;
			points.at(i).x = points.at(i).x + vecTranslate.y/500*points.at(i).y;
			points.at(i).y = points.at(i).y+ vecTranslate.x/500*tmp;
		}
		for (int i = 0; i < points1.size(); i++)
		{
			tmp = points1.at(i).x;
			points1.at(i).x = points1.at(i).x + vecTranslate.y/500*points1.at(i).y;
			points1.at(i).y = points1.at(i).y + vecTranslate.x/500*tmp;
		}
		bezier();
		if (bezier2 == 1) bezier1();
	

}


void triangulate()
{
	//Sorting points by coordinates
	//-----------------------------
	std::sort(points.begin(), points.end(), points_comparator());
	apexes.reserve(points.size());
	edges.reserve(points.size() * (points.size() - 1) / 2);
	triangles.reserve(points.size() * points.size());


	//Initializing 2-triangulation
	//----------------------------
	int i = 0;

	if (points.size() == 1)
	{
		apexes.push_back(Apex(&points.at(i)));
		return;
	}

	apexes.push_back(Apex(&points.at(i)));
	apexes.push_back(Apex(&points.at(i + 1)));
	edges.push_back(Edge(&apexes.at(i), &apexes.at(i + 1)));
	apexes.at(i).a = &edges.at(i);
	apexes.at(i + 1).a = &edges.at(i);
	i += 2;

	//if point colinear, add it
	while (i < points.size()
		&&	abs(((points.at(i - 2).y - points.at(i - 1).y) * (points.at(i - 2).x - points.at(i).x))
			- ((points.at(i - 2).y - points.at(i).y) * (points.at(i - 2).x - points.at(i - 1).x)))
		<= colinearity_tolerance)
	{
		apexes.push_back(Apex(&points.at(i)));
		edges.push_back(Edge(&apexes.at(i - 1), &apexes.at(i)));
		apexes.at(i).a = &edges.at(i - 1);
		++i;
	}

	if (i == points.size())
	{
		return;
	}

	int nedges = edges.size();
	//triangulate all colinear points to first non-colinear one
	for (int j = 0; j < nedges; ++j)
	{
		apexes.push_back(Apex(&points.at(i)));

		if (j == 0)
		{
			edges.push_back(Edge(&apexes.at(i), edges.at(j).s1));
			apexes.at(i).a = &edges.at(i - 1);
		}

		edges.push_back(Edge(edges.at(j).s2, &apexes.at(i)));
		triangles.push_back(Triangle(&edges.at(j), &edges.at(edges.size() - 2), &edges.at(edges.size() - 1)));
		edges.at(j).td = &triangles.at(j);
		edges.at(edges.size() - 1).td = &triangles.at(j);
		edges.at(edges.size() - 2).td = &triangles.at(j);

		if (j > 0)
		{
			edges.at(edges.size() - 3).tg = &triangles.at(j);
		}
	}


	//Incrementing triangulation
	//--------------------------
	float d, dref;
	Triangle* tref;
	Apex a;
	Edge e1;
	Edge e2;
	Triangle tr;
	std::vector<Apex*> tr_apexes;
	tr_apexes.reserve(2);
	std::vector<Edge*> seen_edges;
	seen_edges.reserve(points.size() - 1);
	std::map<Apex*, Edge*> added_edges;
	std::map<Apex*, Edge*>::iterator it;
	Edge* tr_e1;
	Edge* tr_e2;
	for (int j = i + 1; j < points.size(); ++j)
	{
		//add point to triangulation apexes
		apexes.push_back(Apex(&points.at(j)));

		//find seen edges
		seen_edges = findSeenEdges(&apexes.back());

		//triangulate seen edges
		for (int k = 0; k < seen_edges.size(); ++k)
		{
			e1.s1 = &apexes.at(j);
			e1.s2 = seen_edges.at(k)->s1;
			e2.s1 = &apexes.at(j);
			e2.s2 = seen_edges.at(k)->s2;

			it = added_edges.find(seen_edges.at(k)->s1);
			//add new edge(s)
			if (it == added_edges.end())
			{
				edges.push_back(e1);
				added_edges.insert(std::pair<Apex*, Edge*>(seen_edges.at(k)->s1, &edges.back()));
				tr_e1 = &edges.back();
			}
			else
			{
				tr_e1 = it->second;
			}

			it = added_edges.find(seen_edges.at(k)->s2);
			if (it == added_edges.end())
			{
				edges.push_back(e2);
				added_edges.insert(std::pair<Apex*, Edge*>(seen_edges.at(k)->s2, &edges.back()));
				tr_e2 = &edges.back();
			}
			else
			{
				tr_e2 = it->second;
			}

			//create new triangle
			tr = Triangle(seen_edges.at(k), tr_e1, tr_e2);

			if (!isTriangleDirect(tr))
			{
				tr = Triangle(seen_edges.at(k), tr_e2, tr_e1);
			}

			//add new triangle to triangulation
			triangles.push_back(Triangle(seen_edges.at(k), &e1, &e2));

			//add triangle to edges
			if (isTriangleLeft(tr, tr_e1))
			{
				tr_e1->tg = &triangles.back();
				tr_e2->td = &triangles.back();
			}
			else
			{
				tr_e1->td = &triangles.back();
				tr_e2->tg = &triangles.back();
			}
			if (isTriangleLeft(tr, seen_edges.at(k)))
			{
				seen_edges.at(k)->tg = &triangles.back();
			}
			else
			{
				seen_edges.at(k)->td = &triangles.back();
			}
		}

		//clear seen edges for next point
		seen_edges.clear();
		added_edges.clear();
	}
}

void delaunayTriangulation()
{
	std::vector<Edge*> edges_copy;
	edges_copy.reserve(points.size() * (points.size() - 1) / 2);

	for (int i = 0; i < edges.size(); ++i)
	{
		edges_copy.at(i) = &edges.at(i);
	}

	Edge *a;
	Edge A;
	Apex *s1, *s2, *s3, *s4;
	Edge *a1, *a2, *a3, *a4;
	Triangle *t1, *t2;

	while (!edges_copy.empty())
	{
		a = edges_copy.back();
		edges_copy.pop_back();

		if (delaunayCriteria(a))
		{
			continue;
		}

		t1 = a->tg;
		t2 = a->td;
		
		if (t1->a1 == a)
		{
			a1 = t1->a2;
			a4 = t1->a3;
		}
		else if (t1->a2 == a)
		{
			a1 = t1->a1;
			a4 = t1->a3;
		}
		else
		{
			a1 = t1->a1;
			a4 = t1->a2;
		}

		if (t2->a1 == a)
		{
			a2 = t2->a2;
			a3 = t2->a3;
		}
		else if (t2->a2 == a)
		{
			a2 = t2->a1;
			a3 = t2->a3;
		}
		else
		{
			a2 = t2->a1;
			a3 = t2->a2;
		}

		s1 = a->s1;
		s2 = a->s2;

		if (a1->s1 == a->s1 || a1->s1 == a->s2)
		{
			s3 = a1->s2;
		}
		else
		{
			s3 = a1->s1;
		}

		if (a3->s1 == a->s1 || a3->s1 == a->s2)
		{
			s4 = a3->s2;
		}
		else
		{
			s4 = a3->s1;
		}

		A = Edge(s3, s4);
		a->s1 = s3;
		a->s2 = s4;
		s1->a = a1;
		s2->a = a4;

		t1->a1 = a;
		t1->a2 = a1;
		t1->a3 = a2;

		t2->a1 = a;
		t2->a2 = a3;
		t2->a3 = a4;

		if (!isTriangleDirect(*t1))
		{
			t1->a2 = a2;
			t1->a3 = a1;
		}
		if (!isTriangleDirect(*t2))
		{
			t2->a2 = a4;
			t2->a3 = a3;
		}

		if (isTriangleLeft(*t1, a))
		{
			a->tg = t1;
			a->td = t2;
		}
		else
		{
			a->td = t1;
			a->tg = t2;
		}
		if (isTriangleLeft(*t1, a1))
		{
			a1->tg = t1;
		}
		else
		{
			a1->td = t1;
		}
		if (isTriangleLeft(*t1, a2))
		{
			a2->tg = t1;
		}
		else
		{
			a2->td = t1;
		}
		if (isTriangleLeft(*t2, a3))
		{
			a3->tg = t2;
		}
		else
		{
			a3->td = t2;
		}
		if (isTriangleLeft(*t2, a4))
		{
			a4->tg = t2;
		}
		else
		{
			a4->td = t2;
		}

		edges_copy.push_back(a1);
		edges_copy.push_back(a2);
		edges_copy.push_back(a3);
		edges_copy.push_back(a4);
	}
}

void voronoiDiagram()
{
	Circle c;
	Apex *s1, *s2, *s3;
	std::map<Triangle*, Circle> circonscript_circles;
	std::map<Triangle*, Circle>::iterator it1;
	std::map<Edge*, Bounded_Edge> bounded_edges;
	std::map<Edge*, Bounded_Edge>::iterator it2;
	Circle c1, c2;
	Point middle;

	for (int i = 0; i < triangles.size(); ++i)
	{
		std::tie(s1, s2, s3) = apexesOfTriangle(&triangles.at(i));
		c = circonscriptCircle(s1, s2, s3);

		circonscript_circles.insert(std::pair<Triangle*, Circle>(&triangles.at(i), c));
	}

	for (int i = 0; i < edges.size(); ++i)
	{
		if (edges.at(i).td != nullptr && edges.at(i).tg != nullptr)
		{
			it1 = std::find(circonscript_circles.begin(), circonscript_circles.end(), edges.at(i).td);
			c1 = it1->second;
			it1 = std::find(circonscript_circles.begin(), circonscript_circles.end(), edges.at(i).tg);
			c2 = it1->second;

			bounded_edges.insert(std::pair<Edge*, Bounded_Edge>(&edges.at(i), Bounded_Edge(c1.center, c2.center)));
		}
		else
		{
			if (edges.at(i).td == nullptr)
			{
				it1 = std::find(circonscript_circles.begin(), circonscript_circles.end(), edges.at(i).tg);
				c1 = it1->second;

				middle.x = (edges.at(i).s1->p->x + edges.at(i).s2->p->x) / 2;
				middle.y = (edges.at(i).s1->p->y + edges.at(i).s2->p->y) / 2;
			}
			else
			{
				it1 = std::find(circonscript_circles.begin(), circonscript_circles.end(), edges.at(i).td);
				c1 = it1->second;

				middle.x = (edges.at(i).s1->p->x + edges.at(i).s2->p->x) / 2;
				middle.y = (edges.at(i).s1->p->y + edges.at(i).s2->p->y) / 2;
			}

			bounded_edges.insert(std::pair<Edge*, Bounded_Edge>(&edges.at(i), Bounded_Edge(c1.center, middle)));
		}
	}

	for (int i = 0; i < apexes.size(); ++i)
	{
		std::vector<Bounded_Edge> region;

		for (int j = 0; j < edges.size(); ++j)
		{
			if (edges.at(j).s1 == &apexes.at(i) || edges.at(j).s2 == &apexes.at(i))
			{
				it2 = std::find(bounded_edges.begin(), bounded_edges.end(), &edges.at(j));
				region.push_back(it2->second);
			}
		}

		voronoi_regions.insert(std::pair<Apex*, std::vector<Bounded_Edge>>(&apexes.at(i), region));
		region.clear();
	}
}

void delaunayAddPoint()
{
	for (int i = 0; i < apexes.size(); ++i)
	{
		if (apexes.at(i).p->x == points.back().x && apexes.at(i).p->y == points.back().y)
		{
			points.pop_back();
			return;
		}
	}

	if (triangles.empty())
	{
		if (apexes.empty())
		{
			apexes.push_back(Apex(&points.back()));
			return;
		}

		if (apexes.size() == 1)
		{
			apexes.push_back(Apex(&points.back()));
			edges.push_back(Edge(&apexes.at(0), &apexes.back()));
			apexes.at(0).a = &edges.back();
			apexes.at(1).a = &edges.back();
			return;
		}

		if(abs(((apexes.at(0).p->y - apexes.at(1).p->y) * (apexes.at(0).p->x - points.back().x))
			- ((apexes.at(0).p->y - points.back().y) * (apexes.at(0).p->x - apexes.at(1).p->x)))
			<= colinearity_tolerance)
		{
			if (points.back().x < apexes.at(0).p->x 
				|| (points.back().x == apexes.at(0).p->x && points.back().y < apexes.at(0).p->y))
			{
				apexes.push_back(Apex(&points.back()));
				edges.push_back(Edge(&apexes.back(), &apexes.at(0)));
				apexes.back().a = &edges.back();
			}
			else if (points.back().x > apexes.back().p->x 
				|| (points.back().x == apexes.back().p->x && points.back().y > apexes.back().p->y))
			{
				apexes.push_back(Apex(&points.back()));
				edges.push_back(Edge(&apexes.at(apexes.size() - 2), &apexes.back()));
				apexes.back().a = &edges.back();
			}
			else
			{
				for (int i = 0; i < edges.size(); ++i)
				{
					if ((points.back().x > edges.at(i).s1->p->x && points.back().x < edges.at(i).s2->p->x) 
						|| (points.back().x == edges.at(i).s1->p->x && points.back().y > edges.at(i).s1->p->y && points.back().y < edges.at(i).s2->p->y))
					{
						apexes.push_back(Apex(&points.back()));
						Apex* s2 = edges.at(i).s2;
						edges.at(i).s2 = &apexes.back();
						apexes.back().a = &edges.at(i);
						edges.push_back(Edge(&apexes.back(), s2));
						s2->a = &edges.back();
						break;
					}
				}
			}
		}
		else
		{
			apexes.push_back(Apex(&points.back()));

			for (int i = 0; i < apexes.size() - 1; ++i)
			{
				edges.push_back(Edge(&apexes.at(i), &apexes.back()));
			}
			apexes.back().a = &edges.back();

			for (int i = 0; i < (edges.size() - 1) / 2; ++i)
			{
				Edge *a1, *a2;
				Triangle t;
				a1 = &edges.at(i + (edges.size() - 1) / 2);
				a2 = &edges.at(i + (edges.size() - 1) / 2 + 1);

				t = Triangle(&edges.at(i), a1, a2);

				if (!isTriangleDirect(t))
				{
					t = Triangle(&edges.at(i), a2, a1);
				}

				triangles.push_back(t);

				if (isTriangleLeft(t, &edges.at(i)))
				{
					edges.at(i).tg = &triangles.back();
				}
				else
				{
					edges.at(i).td = &triangles.back();
				}

				if (isTriangleLeft(t, a1))
				{
					a1->tg = &triangles.back();
				}
				else
				{
					a1->td = &triangles.back();
				}

				if (isTriangleLeft(t, a2))
				{
					a2->tg = &triangles.back();
				}
				else
				{
					a2->td = &triangles.back();
				}
			}
		}
	}
	else
	{
		bool isInTriangle = false;
		std::vector<Edge*> seen_edges;
		seen_edges.reserve(edges.size());
		Edge* a;
		Circle c;
		Apex *s1, *s2, *s3;
		std::vector<Triangle>::iterator it1;
		std::vector<Edge>::iterator it2;

		apexes.push_back(Apex(&points.back()));

		for (int i = 0; i < triangles.size(); ++i)
		{
			if (pointIsInTriangle(&triangles.at(i), &apexes.back()))
			{
				if (std::find(seen_edges.begin(), seen_edges.end(), triangles.at(i).a1) == seen_edges.end())
				{
					seen_edges.push_back(triangles.at(i).a1);
				}
				if (std::find(seen_edges.begin(), seen_edges.end(), triangles.at(i).a2) == seen_edges.end())
				{
					seen_edges.push_back(triangles.at(i).a2);
				}
				if (std::find(seen_edges.begin(), seen_edges.end(), triangles.at(i).a3) == seen_edges.end())
				{
					seen_edges.push_back(triangles.at(i).a3);
				}
				isInTriangle = true;
			}
		}

		if (!isInTriangle)
		{
			seen_edges = findSeenEdges(&apexes.back());
		}

		while (!seen_edges.empty())
		{
			a = seen_edges.back();
			seen_edges.pop_back();

			if(a->td != nullptr)
			{
				std::tie(s1, s2, s3) = apexesOfTriangle(a->td);
				c = circonscriptCircle(s1, s2, s3);

				if (sqrt(pow(c.center.x - apexes.back().p->x, 2) + pow(c.center.y - apexes.back().p->y, 2)) <= c.radius)
				{
					if (a == a->td->a1)
					{
						seen_edges.push_back(a->td->a2);
						seen_edges.push_back(a->td->a3);
					}
					else if (a == a->td->a2)
					{
						seen_edges.push_back(a->td->a1);
						seen_edges.push_back(a->td->a3);
					}
					else
					{
						seen_edges.push_back(a->td->a1);
						seen_edges.push_back(a->td->a2);
					}

					triangles.erase(std::remove(triangles.begin(), triangles.end(), a->td), triangles.end());
					edges.erase(std::remove(edges.begin(), edges.end(), *a), edges.end());

					continue;
				}
			}
			if (a->tg != nullptr)
			{
				std::tie(s1, s2, s3) = apexesOfTriangle(a->tg);
				c = circonscriptCircle(s1, s2, s3);

				if (sqrt(pow(c.center.x - apexes.back().p->x, 2) + pow(c.center.y - apexes.back().p->y, 2)) <= c.radius)
				{
					if (a == a->tg->a1)
					{
						seen_edges.push_back(a->tg->a2);
						seen_edges.push_back(a->tg->a3);
					}
					else if (a == a->tg->a2)
					{
						seen_edges.push_back(a->tg->a1);
						seen_edges.push_back(a->tg->a3);
					}
					else
					{
						seen_edges.push_back(a->tg->a1);
						seen_edges.push_back(a->tg->a2);
					}

					triangles.erase(std::remove(triangles.begin(), triangles.end(), a->tg), triangles.end());
					edges.erase(std::remove(edges.begin(), edges.end(), *a), edges.end());

					continue;
				}
			}

			edges.push_back(Edge(a->s1, &apexes.back()));
			edges.push_back(Edge(a->s2, &apexes.back()));

			apexes.back().a = &edges.back();

			Triangle tr = Triangle(a, &edges.back(), &edges.at(edges.size() - 2));

			if (!isTriangleDirect(tr))
			{
				tr = Triangle(a, &edges.at(edges.size() - 2), &edges.back());
			}

			triangles.push_back(tr);

		}
	}
}

void delaunayDeletePoint(Point p)
{

}

bool isTriangleDirect(Triangle tr)
{
	Apex *s1 = nullptr, *s2 = nullptr, *s3 = nullptr;

	std::tie(s1, s2, s3) = apexesOfTriangle(&tr);

	return determinant(s1, s2, s3) > 0;
}

bool isTriangleLeft(Triangle tr, Edge* e)
{
	Apex *s1 = nullptr, *s2 = nullptr, *s3 = nullptr;

	if (tr.a1 == e)
	{
		if (tr.a1->s1 == tr.a2->s1)
		{
			s1 = tr.a1->s1;
			s2 = tr.a2->s2;
			s3 = tr.a1->s2;
		}
		else if (tr.a1->s1 == tr.a2->s2)
		{
			s1 = tr.a1->s1;
			s2 = tr.a2->s1;
			s3 = tr.a1->s2;
		}
		else if (tr.a1->s2 == tr.a2->s1)
		{
			s1 = tr.a1->s2;
			s2 = tr.a2->s2;
			s3 = tr.a1->s1;
		}
		else if (tr.a1->s2 == tr.a2->s2)
		{
			s1 = tr.a1->s2;
			s2 = tr.a2->s1;
			s3 = tr.a1->s1;
		}
	}
	else if (tr.a2 == e)
	{
		if (tr.a2->s1 == tr.a3->s1)
		{
			s1 = tr.a2->s1;
			s2 = tr.a3->s2;
			s3 = tr.a2->s2;
		}
		else if (tr.a2->s1 == tr.a3->s2)
		{
			s1 = tr.a2->s1;
			s2 = tr.a3->s1;
			s3 = tr.a2->s2;
		}
		else if (tr.a2->s2 == tr.a3->s1)
		{
			s1 = tr.a2->s2;
			s2 = tr.a3->s2;
			s3 = tr.a2->s1;
		}
		else if (tr.a2->s2 == tr.a3->s2)
		{
			s1 = tr.a2->s2;
			s2 = tr.a3->s1;
			s3 = tr.a2->s1;
		}
	}
	else if (tr.a3 == e)
	{
		if (tr.a3->s1 == tr.a1->s1)
		{
			s1 = tr.a3->s1;
			s2 = tr.a1->s2;
			s3 = tr.a3->s2;
		}
		else if (tr.a3->s1 == tr.a1->s2)
		{
			s1 = tr.a3->s1;
			s2 = tr.a1->s1;
			s3 = tr.a3->s2;
		}
		else if (tr.a3->s2 == tr.a1->s1)
		{
			s1 = tr.a3->s2;
			s2 = tr.a1->s2;
			s3 = tr.a3->s1;
		}
		else if (tr.a3->s2 == tr.a1->s2)
		{
			s1 = tr.a3->s2;
			s2 = tr.a1->s1;
			s3 = tr.a3->s1;
		}
	}

	return determinant(s1, s2, s3) > 0;
}

float determinant(Apex* s1, Apex* s2, Apex* s3)
{
	return 0.5 * ((s1->p->x - s2->p->x) * (s3->p->y - s2->p->y) - (s1->p->y - s2->p->y) * (s3->p->x - s2->p->x));
}

bool delaunayCriteria(Edge *a)
{
	if (a->td == nullptr || a->tg == nullptr)
	{
		return true;
	}

	Point c;
	float rc;
	Apex *s1, *s2, *s3;
	Circle circonscript;

	if (a->td != nullptr)
	{
		std::tie(s1, s2, s3) = apexesOfTriangle(a->td);

		circonscript = circonscriptCircle(s1, s2, s3);

		for (int i = 0; i < apexes.size(); ++i)
		{
			if (&apexes.at(i) == s1
				|| &apexes.at(i) == s2
				|| &apexes.at(i) == s3)
			{
				continue;
			}

			if (sqrt(pow(apexes.at(i).p->x - circonscript.center.x, 2) + pow(apexes.at(i).p->y - circonscript.center.y, 2)) <= circonscript.radius)
			{
				return false;
			}
		}
	}
	if (a->tg != nullptr)
	{
		std::tie(s1, s2, s3) = apexesOfTriangle(a->tg);

		circonscript = circonscriptCircle(s1, s2, s3);

		for (int i = 0; i < apexes.size(); ++i)
		{
			if (&apexes.at(i) == s1
				|| &apexes.at(i) == s2
				|| &apexes.at(i) == s3)
			{
				continue;
			}

			if (sqrt(pow(apexes.at(i).p->x - circonscript.center.x, 2) + pow(apexes.at(i).p->y - circonscript.center.y, 2)) <= circonscript.radius)
			{
				return false;
			}
		}
	}

	return true;
}

std::tuple<Apex*, Apex*, Apex*> apexesOfTriangle(Triangle* tr)
{
	Apex *s1, *s2, *s3;

	if (tr->a1->s1 == tr->a2->s1)
	{
		s1 = tr->a1->s1;
		s2 = tr->a2->s2;
		s3 = tr->a1->s2;
	}
	else if (tr->a1->s1 == tr->a2->s2)
	{
		s1 = tr->a1->s1;
		s2 = tr->a2->s1;
		s3 = tr->a1->s2;
	}
	else if (tr->a1->s2 == tr->a2->s1)
	{
		s1 = tr->a1->s2;
		s2 = tr->a2->s2;
		s3 = tr->a1->s1;
	}
	else if (tr->a1->s2 == tr->a2->s2)
	{
		s1 = tr->a1->s2;
		s2 = tr->a2->s1;
		s3 = tr->a1->s1;
	}

	return std::make_tuple(s1, s2, s3);
}

Circle circonscriptCircle(Apex* s1, Apex* s2, Apex* s3)
{
	Circle c;

	c.center.x = ((pow(s3->p->x, 2) - pow(s2->p->x, 2) + pow(s3->p->y, 2) - pow(s2->p->y, 2))
		/ (2 * (s3->p->y - s2->p->y))
		- (pow(s2->p->x, 2) - pow(s1->p->x, 2) + pow(s2->p->y, 2) - pow(s1->p->y, 2))
		/ (2 * (s2->p->y - s1->p->y)))
		/ (((s2->p->x - s1->p->x) / (s2->p->y - s1->p->y))
			- ((s3->p->x - s2->p->x) / (s3->p->y - s2->p->y)));

	c.center.y = -c.center.x * (s2->p->x - s1->p->x) / (s2->p->y - s1->p->y)
		+ (pow(s2->p->x, 2) - pow(s1->p->x, 2) + pow(s2->p->y, 2) - pow(s1->p->y, 2)) / (2 * (s2->p->y - s1->p->y));

	c.radius = sqrt(pow(s1->p->x - c.center.x, 2) + pow(s1->p->y - c.center.y, 2));

	return c;
}

bool pointIsInTriangle(Triangle* t, Apex* s)
{
	float d, dref;
	Apex *s1, *s2, *s3;
	std::tie(s1, s2, s3) = apexesOfTriangle(t);

	if (t->a1->s1 != s1 && t->a1->s2 != s1)
	{
		dref = (s1->p->x - t->a1->s1->p->x) * (t->a1->s2->p->y - t->a1->s1->p->y)
			- (s1->p->y - t->a1->s1->p->y) * (t->a1->s2->p->x - t->a1->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}
	else if (t->a1->s1 != s2 && t->a1->s2 != s2)
	{
		dref = (s2->p->x - t->a1->s1->p->x) * (t->a1->s2->p->y - t->a1->s1->p->y)
			- (s2->p->y - t->a1->s1->p->y) * (t->a1->s2->p->x - t->a1->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}
	else
	{
		dref = (s3->p->x - t->a1->s1->p->x) * (t->a1->s2->p->y - t->a1->s1->p->y)
			- (s3->p->y - t->a1->s1->p->y) * (t->a1->s2->p->x - t->a1->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}

	d = (s->p->x - t->a1->s1->p->x) * (t->a1->s2->p->y - t->a1->s1->p->y)
		- (s->p->y - t->a1->s1->p->y) * (t->a1->s2->p->x - t->a1->s1->p->x);
	d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

	if (d != dref && d != 0)
	{
		return false;
	}

	if (d == 0)
	{
		return true;
	}

	if (t->a2->s1 != s1 && t->a2->s2 != s1)
	{
		dref = (s1->p->x - t->a2->s1->p->x) * (t->a2->s2->p->y - t->a2->s1->p->y)
			- (s1->p->y - t->a2->s1->p->y) * (t->a2->s2->p->x - t->a2->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}
	else if (t->a2->s1 != s2 && t->a2->s2 != s2)
	{
		dref = (s2->p->x - t->a2->s1->p->x) * (t->a2->s2->p->y - t->a2->s1->p->y)
			- (s2->p->y - t->a2->s1->p->y) * (t->a2->s2->p->x - t->a2->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}
	else
	{
		dref = (s3->p->x - t->a2->s1->p->x) * (t->a2->s2->p->y - t->a2->s1->p->y)
			- (s3->p->y - t->a2->s1->p->y) * (t->a2->s2->p->x - t->a2->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}

	d = (s->p->x - t->a2->s1->p->x) * (t->a2->s2->p->y - t->a2->s1->p->y)
		- (s->p->y - t->a2->s1->p->y) * (t->a2->s2->p->x - t->a2->s1->p->x);
	d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

	if (d != dref && d != 0)
	{
		return false;
	}

	if (d == 0)
	{
		return true;
	}

	if (t->a3->s1 != s1 && t->a3->s2 != s1)
	{
		dref = (s1->p->x - t->a3->s1->p->x) * (t->a3->s2->p->y - t->a3->s1->p->y)
			- (s1->p->y - t->a3->s1->p->y) * (t->a3->s2->p->x - t->a3->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}
	else if (t->a3->s1 != s2 && t->a3->s2 != s2)
	{
		dref = (s2->p->x - t->a3->s1->p->x) * (t->a3->s2->p->y - t->a3->s1->p->y)
			- (s2->p->y - t->a3->s1->p->y) * (t->a3->s2->p->x - t->a3->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}
	else
	{
		dref = (s3->p->x - t->a3->s1->p->x) * (t->a3->s2->p->y - t->a3->s1->p->y)
			- (s3->p->y - t->a3->s1->p->y) * (t->a3->s2->p->x - t->a3->s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;
	}

	d = (s->p->x - t->a3->s1->p->x) * (t->a3->s2->p->y - t->a3->s1->p->y)
		- (s->p->y - t->a3->s1->p->y) * (t->a3->s2->p->x - t->a3->s1->p->x);
	d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

	if (d != dref && d != 0)
	{
		return false;
	}

	return true;
}

std::vector<Edge*> findSeenEdges(Apex* s)
{
	Triangle* tref;
	std::vector<Apex*> tr_apexes;
	tr_apexes.reserve(2);
	Apex a;
	std::vector<Edge*> seen_edges;
	seen_edges.reserve(edges.size());
	float d, dref;

	for (int k = 0; k < edges.size(); ++k)
	{
		//if both triangle non null => continue
		if (edges.at(k).td != nullptr
			&& edges.at(k).tg != nullptr)
		{
			continue;
		}

		//find non null triangle
		tref = (&edges.at(k).td != nullptr) ? edges.at(k).td : edges.at(k).tg;

		//add apexes of edge
		tr_apexes.push_back(edges.at(k).s1);
		tr_apexes.push_back(edges.at(k).s2);

		//find apex of triangle not in edge
		if (tref->a1 != &edges.at(k))
		{
			if (std::find(tr_apexes.begin(), tr_apexes.end(), tref->a1->s1)
				!= tr_apexes.end())
			{
				a = *tref->a1->s2;
			}
			else
			{
				a = *tref->a1->s1;
			}
		}
		else if (tref->a2 != &edges.at(k))
		{
			if (std::find(tr_apexes.begin(), tr_apexes.end(), tref->a2->s1)
				!= tr_apexes.end())
			{
				a = *tref->a2->s2;
			}
			else
			{
				a = *tref->a2->s1;
			}
		}

		//calculate reference value for side of edge
		dref = (a.p->x - edges.at(k).s1->p->x) * (edges.at(k).s2->p->y - edges.at(k).s1->p->y)
			- (a.p->y - edges.at(k).s1->p->y) * (edges.at(k).s2->p->x - edges.at(k).s1->p->x);
		dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;

		//calculate real value for side of edge
		d = (s->p->x - edges.at(k).s1->p->x) * (edges.at(k).s2->p->y - edges.at(k).s1->p->y)
			- (s->p->y - edges.at(k).s1->p->y) * (edges.at(k).s2->p->x - edges.at(k).s1->p->x);
		d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

		//check if edge is seen by point
		if (d != dref && d != 0)
		{
			seen_edges.push_back(&edges.at(k));
		}

		tr_apexes.clear();
	}

	return seen_edges;
}