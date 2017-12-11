#include "glew.h"
#ifdef _MSC_VER
#pragma comment(lib, "glew32.lib")
#include <windows.h>
#endif
#include <iostream>

#include "freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>
#include <algorithm> 
#include <functional> 
#include <set> 
#define PI 3.14159265

#include "Point.hpp"
#include <math.h>
#include "Camera.hpp"
#include "Controller.hpp"
#include "common/EsgiShader.h"
#include "common/mat4.h"
#include "Controller.hpp"
int sizetab, sizeind;

//--------------------------------------------------------------------------------------------------------------------------------------------------------
EsgiShader g_BasicShader;
bool voronoi3d=false;
int nbCount;
int* firsts;
int* count;
std::vector<Colore> col;
std::vector<Point3D> p3D, tmpVectorPoints;           // Tous les Point3Ds en 3D
float* tabPoints, *tmpPoints;         //Tous les Point3Ds en 3D
GLushort* createInd(int);
GLushort* indi, *indTmp;			// Tab indice


GLuint VAO;
GLuint VBO0;	// identifiant du Vertex Buffer Object 0
GLuint VBO1;	// identifiant du Vertex Buffer Object 1
GLuint IBO, IBO1;	// identifiant du Index Buffer Object
GLuint TexObj; // identifiant du Texture Object

std::vector<int>* indicesVector = new std::vector<int>();


struct Point;
struct Apex;
struct Edge;
struct Bounded_Edge;
struct Triangle;
struct Circle;

typedef struct Point
{
	float x;
	float y;
} Point;

bool operator==(const Point& p1, const Point& p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
	{
		return true;
	}

	return false;
}

typedef struct Apex
{
	Point* p;
	Edge* a;

	Apex() : p(nullptr), a(nullptr) {};

	Apex(Point* point) : p(point), a(nullptr) {};
} Apex;

bool operator==(const Apex& s1, const Apex& s2)
{
	if (*s1.p == *s2.p)
	{
		return true;
	}

	return false;
}

typedef struct Edge
{
	Apex* s1;
	Apex* s2;
	Triangle* td;
	Triangle* tg;

	Edge() : s1(nullptr), s2(nullptr), td(nullptr), tg(nullptr) {};

	Edge(Apex* s1, Apex* s2) : s1(s1), s2(s2), td(nullptr), tg(nullptr) {};
} Edge;

bool operator==(const Edge& a1, const Edge& a2)
{
	if (a1.s1 == a2.s1 && a1.s2 == a2.s2)
	{
		return true;
	}

	return false;
}

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

bool operator==(const Triangle& t1, const Triangle& t2)
{
	if (t1.a1 == t2.a1 && t1.a2 == t2.a2 && t1.a3 == t2.a3)
	{
		return true;
	}

	return false;
}

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

//Triangulation elements
std::vector<Point> points;
std::vector<Apex> apexes;
std::vector<Edge> edges;
std::vector<Triangle> triangles;
std::map<Apex*, std::vector<Bounded_Edge>> voronoi_regions;

//Triangulation parameters
int maxPoints = 50;
float colinearity_tolerance = 0.0001;
bool voronoi;

//Glut parameters
int menuPrincipal = 0;
int colorSubMenu;
int voronoiColorSubMenu;
std::tuple<float, float, float> triangulationColor;
std::tuple<float, float, float> voronoiColor;

//Glut functions
void dessin();
void mouse(int button, int state, int x, int y);
void showMenu();
void mainMenuCallback(int);
void colorMenuCallBack(int);
void voronoiColorMenuCallBack(int);

//Triangulation functions
void triangulate();
void delaunayTriangulation();
void voronoiDiagram();
void delaunayAddPoint();
void delaunayDeletePoint(Point*);

//Utility functions
bool isTriangleDirect(Triangle);
bool isTriangleLeft(Triangle, Edge*);
float determinant(Apex*, Apex*, Apex*);
bool delaunayCriteria(Edge*);
std::tuple<Apex*, Apex*, Apex*> apexesOfTriangle(Triangle*);
Circle circonscriptCircle(Apex*, Apex*, Apex*);
bool pointIsInTriangle(Triangle*, Apex*);
std::vector<Edge*> findSeenEdges(Apex*);
std::vector<Edge*> findAdjacentEdges(Point*);
float orientedAngle(Edge*, Edge*, Point*);
std::vector<Edge*> findOrientedAdjacentEdges(Point*);
std::vector<Triangle*> findOrientedAdjacentTriangles(std::vector<Edge*>);
std::vector<Edge*> findNonAdjacentEdges(std::vector<Edge*>, std::vector<Triangle*>);
void addTriangleToItsEdges(Triangle*);
bool edgeExists(Apex*, Apex*);




float * structToTabColor(std::vector<Point3D> newPoints, std::vector<Colore> c,bool col=false)
{
	float* tabP = new float[newPoints.size() * 9];
	int j = 0;
	for (int i = 0; i < newPoints.size() * 9; i += 9)
	{
		tabP[i] = newPoints[j].x;
		tabP[i + 1] = newPoints[j].y;
		tabP[i + 2] = newPoints[j].z;

		tabP[i + 3] = newPoints[j].n1;
		tabP[i + 4] = newPoints[j].n2;
		tabP[i + 5] = newPoints[j].n3;

		/*if (c[j] == Colore(purple))
		{
		tabP[i + 6] = 1;
		tabP[i + 7] = 0;
		tabP[i + 8] = 1;
		}
		if (c[j] == Colore(red))
		{
		tabP[i + 6] = 1;
		tabP[i + 7] = 0;
		tabP[i + 8] = 0;
		}
		if (c[j] == Colore(blue))
		{
		tabP[i + 6] = 0;
		tabP[i + 7] = 0;
		tabP[i + 8] = 1;
		}*/
		if (!col)
		{
			tabP[i + 6] = RandomFloat(0, 1);
			tabP[i + 7] = RandomFloat(0, 1);
			tabP[i + 8] = RandomFloat(0, 1);
		}
		else
		{
			tabP[i + 6] = 1.0f;
			tabP[i + 7] = 1.0f;
			tabP[i + 8] = 1.0f;
		}
		j++;
	}

	return tabP;
}

bool Initialize()
{
	std::vector<Point3D> centerPoints3D = createRandomPoints(10);


	std::vector<Colore> tmpColore;

	//p3D = transformPointsToCube(centerPoints3D);
	p3D = createVoronoiExtCube();
	for (int i = 0; i < p3D.size(); i++)
	{
		tmpColore.push_back(Colore(red));
	}

	tabPoints = structToTabColor(p3D, tmpColore);

	indi = createInd(centerPoints3D.size() * 24);

	std::vector<Colore> tm;
	std::vector<std::vector<Point3D>> allVectorPoints;
	tmpVectorPoints.clear();
	//tmpVectorPoints = createVoronoi2DFaces(indicesVector);
/*
	std::vector<Point3D> voronoiIns1;
	voronoiIns1.push_back(Point3D(-50, 25, 150, 0.0f, 0.0f, +1.0f));
	voronoiIns1.push_back(Point3D(0, 50, 150, 0.0f, 0.0f, +1.0f));
	voronoiIns1.push_back(Point3D(25, 50, 150, 0.0f, 0.0f, +1.0f));
	voronoiIns1.push_back(Point3D(25, -25, 150, 0.0f, 0.0f, +1.0f));
	voronoiIns1.push_back(Point3D(-50, -25, 150, 0.0f, 0.0f, +1.0f));


	allVectorPoints.push_back(voronoiIns1);

	std::vector<Point3D> voronoiIns2;
	voronoiIns2.push_back(Point3D(25, 50, 150, 0.0f, 0.0f, +1.0f));
	voronoiIns2.push_back(Point3D(50, 50, 150, 0.0f, 0.0f, +1.0f));
	voronoiIns2.push_back(Point3D(50, -25, 150, 0.0f, 0.0f, +1.0f));
	voronoiIns2.push_back(Point3D(25, -25, 150, 0.0f, 0.0f, +1.0f));


	allVectorPoints.push_back(voronoiIns2);*/

	std::vector<Point3D> newTmp = tmpVectorPoints;

	
	for (int i = 0; i < apexes.size(); i++)
	{
		
			for (int k = 0; k < voronoi_regions.at(&apexes.at(i)).size(); k++)
			{
				std::vector<Point3D> tmpP3d = *new std::vector<Point3D>();
				/*glVertex2f(voronoi_regions.at(&apexes.at(i)).at(k).p1.x,
					voronoi_regions.at(&apexes.at(i)).at(k).p1.y);
				glVertex2f(voronoi_regions.at(&apexes.at(i)).at(k).p2.x,
					voronoi_regions.at(&apexes.at(i)).at(k).p2.y);*/

				tmpP3d.push_back(Point3D(voronoi_regions.at(&apexes.at(i)).at(k).p1.x, voronoi_regions.at(&apexes.at(i)).at(k).p1.y, 150, 0.0f, 0.0f, +1.0f));
				tmpP3d.push_back(Point3D(voronoi_regions.at(&apexes.at(i)).at(k).p2.x, voronoi_regions.at(&apexes.at(i)).at(k).p2.y, 150, 0.0f, 0.0f, +1.0f));
				allVectorPoints.push_back(tmpP3d);
				
			}
			
			//indicesVector->push_back(voronoi_regions.at(&apexes.at(i)).size()*2);
			
	}



	tmpVectorPoints = combineVector(newTmp, algo3d(allVectorPoints, indicesVector));









	indTmp = createInd(tmpVectorPoints.size());
	tmpPoints = structToTabColor(tmpVectorPoints, tm,true);




	firsts = firstsFromVectorIndex(indicesVector);
	count = countFromVectorIndex(indicesVector);
	nbCount = nbCountFromVectorIndex(indicesVector);

	glewInit();
	g_BasicShader.LoadVertexShader("basic.vs");
	g_BasicShader.LoadFragmentShader("basic.fs");
	g_BasicShader.CreateProgram();

	glGenTextures(1, &TexObj);
	glBindTexture(GL_TEXTURE_2D, TexObj);
	int w, h, c; //largeur, hauteur et # de composantes du fichier
	/*uint8_t* bitmapRGBA = stbi_load("../data/dragon.png",
		&w, &h, &c, STBI_rgb_alpha);*/

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //GL_NEAREST)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, // Destination
	//	GL_RGBA, GL_UNSIGNED_BYTE, bitmapRGBA);		// Source

	glGenerateMipmap(GL_TEXTURE_2D);

	//stbi_image_free(bitmapRGBA);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenVertexArrays(1, &VBO0); // Créer le VAO
	glBindVertexArray(VBO0); // Lier le VAO pour l'utiliser
	glEnableVertexAttribArray(0);


	//glGenBuffers(1, &VBO0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO0);
	glBufferData(GL_ARRAY_BUFFER, p3D.size() * 9 * sizeof(float), tabPoints, GL_STATIC_DRAW);
	//---
	glGenVertexArrays(1, &VBO1); // Créer le VAO
	glBindVertexArray(VBO1); // Lier le VAO pour l'utiliser
	glEnableVertexAttribArray(0);


	//glGenBuffers(1, &VBO0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, tmpVectorPoints.size() * 9 * sizeof(float), tmpPoints, GL_STATIC_DRAW);

	// rendu indexe
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, p3D.size() * sizeof(GLushort), indi, GL_STATIC_DRAW);
	glGenBuffers(1, &IBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, tmpVectorPoints.size() * sizeof(GLushort), indTmp, GL_STATIC_DRAW);

	// le fait de specifier 0 comme BO desactive l'usage des BOs
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ChangeCam(CamType);
	return true;
}

void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(10.0f);
		break;
	case GLUT_KEY_DOWN:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0f);
		break;
	case GLUT_KEY_RIGHT:
		voronoi3d = true;
		glLineWidth(10.0f);
		Initialize();
		break;

	}
	glutPostRedisplay();
}

void Terminate()
{
	glDeleteTextures(1, &TexObj);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO0);
	glDeleteBuffers(1, &IBO1);
	glDeleteBuffers(1, &VBO1);
	g_BasicShader.DestroyProgram();
}

void update()
{
	if (voronoi3d)
	{
		glutPostRedisplay();
	}
}
void mouse1(int x, int y)
{
	if(voronoi3d)
	{
		//rotX = (float)(y-height*0.5f) * rotSpeed;
		//rotY = (float)(x-width*0.5f) * rotSpeed;
		rotX += (y - lastposY)* rotSpeed;
		rotY += (x - lastposX)* rotSpeed;
		lastposX = x;
		lastposY = y;
	}
}
int main(int argc, char **argv)
{
	/*r.push_back(0.0f);
	g.push_back(0.0f);
	b.push_back(0.0f);*/
	voronoi = false;
	points.reserve(maxPoints);
	apexes.reserve(maxPoints);
	edges.reserve(maxPoints * (maxPoints - 1) / 2);
	triangles.reserve(maxPoints * maxPoints);


	// Initialisation de glut et creation de la fenetre
	// ------------------------------------------------
	glutInit(&argc, argv);											// Initialisation
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);		// Mode d'affichage RGB, et test prafondeur

	glutInitWindowSize(800, 800);									// Dimension fenêtre
	glutInitWindowPosition(100, 10);								// Position coin haut gauche
	glutCreateWindow("Triangulation");								// Nom
	gluOrtho2D(-400.0, 400.0, -400.0, 400.0);						// Repère 2D délimitant les abscisses et les ordonnées


																	// Initialisation d'OpenGL
																	// -----------------------
	glClearColor(1.0, 1.0, 1.0, 0.50);
	glColor3f(1.0, 1.0, 1.0);			     	 // couleur: blanc
	glPointSize(2.0);							 // taille d'un point: 2px
	glLineWidth(1);

												 /* Enregistrement des fonctions de rappel
												 => initialisation des fonctions callback appelées par glut */
	

	showMenu();
	glutMouseFunc(mouse);
	glutSpecialFunc(SpecialInput);
	glutPassiveMotionFunc(mouse1);
	//glutMotionFunc(mouse_activeFunc);
	glutDisplayFunc(dessin);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);

	/* rq: le callback de fonction (fonction de rappel) est une fonction qui est passée en argument à une
	autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en même temps)
	alors qu'il ne les connaît pas par avance.*/



	/* Entrée dans la boucle principale de glut, traitement des évènements */
	
	glutMainLoop();								  // lancement de la boucle de réception des évènements
	
	Terminate();
	return 0;
}

void dessin()
{
	if (!voronoi3d)
	{
		float r, g, b;

		glClearColor(1.0, 1.0, 1.0, 0.50);
		glClear(GL_COLOR_BUFFER_BIT);
		std::tie(r, g, b) = triangulationColor;

		glBegin(GL_POINTS);
		glColor3f(r, g, b);
		for (int i = 0; i < apexes.size(); i++)
		{
			glVertex2f(apexes.at(i).p->x, apexes.at(i).p->y);
		}

		glEnd();

		glBegin(GL_LINES);
		glColor3f(r, g, b);
		for (int k = 0; k < edges.size(); ++k)
		{
			glVertex2f(edges.at(k).s1->p->x, edges.at(k).s1->p->y);
			glVertex2f(edges.at(k).s2->p->x, edges.at(k).s2->p->y);
		}

		glEnd();

		if (voronoi)
		{
			std::tie(r, g, b) = voronoiColor;
			glColor3f(r, g, b);

			glBegin(GL_LINES);

			for (int i = 0; i < apexes.size(); i++)
			{
				for (int k = 0; k < voronoi_regions.at(&apexes.at(i)).size(); k++)
				{
					glVertex2f(voronoi_regions.at(&apexes.at(i)).at(k).p1.x,
						voronoi_regions.at(&apexes.at(i)).at(k).p1.y);
					glVertex2f(voronoi_regions.at(&apexes.at(i)).at(k).p2.x,
						voronoi_regions.at(&apexes.at(i)).at(k).p2.y);
				}
			}

			glEnd();

		}


		glutSwapBuffers();
	}
	else {
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// afin d'obtenir le deltatime actuel
		TimeSinceAppStartedInMS = glutGet(GLUT_ELAPSED_TIME);
		TimeInSeconds = TimeSinceAppStartedInMS / 1000.0f;
		DeltaTime = (TimeSinceAppStartedInMS - OldTime) / 1000.0f;
		OldTime = TimeSinceAppStartedInMS;

		glViewport(0, 0, width, height);
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		//glColorMask(GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		auto program = g_BasicShader.GetProgram();
		glUseProgram(program);

		/*	uint32_t texUnit = 0;
		glActiveTexture(GL_TEXTURE0 + texUnit);
		glBindTexture(GL_TEXTURE_2D, TexObj);
		auto texture_location = glGetUniformLocation(program, "u_Texture");
		glUniform1i(texture_location, texUnit);
		*/
		// UNIFORMS
		Esgi::Mat4 worldMatrix;
		worldMatrix.MakeScale(1.0f, 1.0f, 1.0f);

		//  Camera Matrix
		Esgi::Mat4 cameraMatrix;
		switch (CamType)
		{
		case 0:	//FPS
			cameraMatrix = FPSCamera(posX, posY, posZ, rotX, rotY);
			break;
		case 1:	//Orbit
			cameraMatrix = OrbitCamera(posX, posY, posZ, distance, rotX, rotY);
			break;
		}

		//

		auto world_location = glGetUniformLocation(program, "u_WorldMatrix");
		glUniformMatrix4fv(world_location, 1, GL_FALSE, worldMatrix.m);

		Esgi::Mat4 projectionMatrix;
		float w = glutGet(GLUT_WINDOW_WIDTH), h = glutGet(GLUT_WINDOW_HEIGHT);
		// ProjectionMatrix
		float aspectRatio = w / h;			// facteur d'aspect
		float fovy = 45.0f;					// degree d'ouverture
		float nearZ = 0.1f;
		float farZ = 10000.0f;
		projectionMatrix.Perspective(fovy, aspectRatio, nearZ, farZ);

		//projectionMatrix.MakeScale(1.0f / (0.5f*w), 1.0f / (0.5f*h), 1.0f);

		auto projection_location = glGetUniformLocation(program, "u_ProjectionMatrix");
		glUniformMatrix4fv(projection_location, 1, GL_FALSE, projectionMatrix.m);

		auto camera_location = glGetUniformLocation(program, "u_CameraMatrix");
		glUniformMatrix4fv(camera_location, 1, GL_FALSE, cameraMatrix.m);

		auto time_location = glGetUniformLocation(program, "u_Time");
		glUniform1f(time_location, TimeInSeconds);

		/*auto c_location = glGetUniformLocation(program, "color");
		glUniform4fv(c_location, 1, colore);*/

		// ATTRIBUTES
		auto normal_location = glGetAttribLocation(program, "a_Normal");
		auto position_location = glGetAttribLocation(program, "a_Position");
		auto color_location = glGetAttribLocation(program, "a_Color");
		//auto texcoords_location = glGetAttribLocation(program, "a_TexCoords");
		//glVertexAttrib3f(color_location, 0.0f, 1.0f, 0.0f);

		// Le fait de specifier la ligne suivante va modifier le fonctionnement interne de glVertexAttribPointer
		// lorsque GL_ARRAY_BUFFER != 0 cela indique que les donnees sont stockees sur le GPU
		glBindBuffer(GL_ARRAY_BUFFER, VBO0);

		//glBindVertexArray(VAO);

		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(0 * sizeof(float)));
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
		glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));
		// on interprete les 3 valeurs inconnues comme RGB alors que ce sont les normales
		//glVertexAttribPointer(texcoords_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));

		//glEnableVertexAttribArray(texcoords_location);
		glEnableVertexAttribArray(position_location);
		glEnableVertexAttribArray(normal_location);
		glEnableVertexAttribArray(color_location);
		//glEnableVertexAttribArray(texcoords_location);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_QUADS, p3D.size(), GL_UNSIGNED_SHORT, nullptr);

		//-----------

		glBindBuffer(GL_ARRAY_BUFFER, VBO1);

		//glBindVertexArray(VAO);

		glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(0 * sizeof(float)));
		glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(3 * sizeof(float)));
		glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));
		// on interprete les 3 valeurs inconnues comme RGB alors que ce sont les normales
		//glVertexAttribPointer(texcoords_location, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<const void *>(6 * sizeof(float)));

		//glEnableVertexAttribArray(texcoords_location);
		glEnableVertexAttribArray(position_location);
		glEnableVertexAttribArray(normal_location);
		glEnableVertexAttribArray(color_location);
		//glEnableVertexAttribArray(texcoords_location);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
		//glDrawElements(GL_TRIANGLES, tmpVectorPoints.size(), GL_UNSIGNED_SHORT, nullptr);
		glMultiDrawArrays(GL_LINES, firsts, count, nbCount);
		//----------------
		//glPointSize(10);
		glDisableVertexAttribArray(position_location);
		glDisableVertexAttribArray(normal_location);
		glDisableVertexAttribArray(color_location);
		//glDisableVertexAttribArray(texcoords_location);
		glUseProgram(0);


		//Repositionnement du curseur 
		//glutWarpPointer(width*0.5f, height*0.5f);
		glEnd();


		glutSwapBuffers();
	}
}

void mouse(int button, int state, int x, int y)
{
	if (!voronoi3d)
	{
		Point tmp;
		bool deletePoint = false;
		Point* p = nullptr;

		if (points.size() == maxPoints)
		{
			return;
		}

		// Si on appuie sur le bouton de gauche	
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			tmp.x = x - 400.0f;
			tmp.y = -y + 400.0f;

			for (int i = 0; i < points.size(); ++i)
			{
				if (tmp.x == points.at(i).x && tmp.y == points.at(i).y)
				{
					p = &points.at(i);
					deletePoint = true;
					break;
				}
			}

			if (deletePoint)
			{
				points.erase(std::find(points.begin(), points.end(), *p));
			}
			else
			{
				points.push_back(tmp);
			}
			//voronoi = false;
			triangulate();
			delaunayTriangulation();

			if (voronoi)
			{
				voronoiDiagram();
			}

		}
		else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
		{
			tmp.x = x - 400.0f;
			tmp.y = -y + 400.0f;
			//voronoi = false;

			for (int i = 0; i < points.size(); ++i)
			{
				if (tmp.x == points.at(i).x && tmp.y == points.at(i).y)
				{
					p = &points.at(i);
					deletePoint = true;
					break;
				}
			}

			if (deletePoint)
			{
				delaunayDeletePoint(p);
			}
			else
			{
				points.push_back(tmp);
				delaunayAddPoint();
			}

			if (voronoi)
			{
				voronoiDiagram();
			}
		}
	}
	else
	{
		//rotX = (float)(y-height*0.5f) * rotSpeed;
		//rotY = (float)(x-width*0.5f) * rotSpeed;
		rotX += (y - lastposY)* rotSpeed;
		rotY += (x - lastposX)* rotSpeed;
		lastposX = x;
		lastposY = y;
	}
}

void showMenu()
{
	glutDestroyMenu(menuPrincipal);

	colorSubMenu = glutCreateMenu(colorMenuCallBack);
	glutAddMenuEntry("Blanc", 11);
	glutAddMenuEntry("Jaune", 12);
	glutAddMenuEntry("Vert", 13);
	glutAddMenuEntry("Noir", 14);
	glutAddMenuEntry("Bleu", 15);
	glutAddMenuEntry("Orange", 16);
	glutAddMenuEntry("Violet", 17);

	voronoiColorSubMenu = glutCreateMenu(voronoiColorMenuCallBack);
	glutAddMenuEntry("Blanc", 21);
	glutAddMenuEntry("Jaune", 22);
	glutAddMenuEntry("Vert", 23);
	glutAddMenuEntry("Noir", 24);
	glutAddMenuEntry("Bleu", 25);
	glutAddMenuEntry("Orange", 26);
	glutAddMenuEntry("Violet", 27);

	menuPrincipal = glutCreateMenu(mainMenuCallback);

	glutAddSubMenu("Couleur", colorSubMenu);
	glutAddSubMenu("Voronoi", voronoiColorSubMenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void mainMenuCallback(int menuItem)
{	
	showMenu();
}

void colorMenuCallBack(int a)
{
	switch (a)
	{
	case 12:
		triangulationColor = std::make_tuple(1.0f, 1.0f, 0.0f);
		break;
	case 13:
		triangulationColor = std::make_tuple(0.0f, 1.0f, 0.0f);
		break;
	case 14:
		triangulationColor = std::make_tuple(0.0f, 0.0f, 0.0f);
		break;
	case 15:
		triangulationColor = std::make_tuple(0.0f, 0.0f, 1.0f);
		break;
	case 16:
		triangulationColor = std::make_tuple(1.0f, 0.5f, 0.0f);
		break;
	case 17:
		triangulationColor = std::make_tuple(1.0f, 0.0f, 1.0f);
		break;
	default:
		triangulationColor = std::make_tuple(1.0f, 1.0f, 1.0f);
		break;
	}
}

void voronoiColorMenuCallBack(int a)
{
	voronoi = true;

	switch (a)
	{
	case 22:
		voronoiColor = std::make_tuple(1.0f, 1.0f, 0.0f);
		break;
	case 23:
		voronoiColor = std::make_tuple(0.0f, 1.0f, 0.0f);
		break;
	case 24:
		voronoiColor = std::make_tuple(0.0f, 0.0f, 0.0f);
		break;
	case 25:
		voronoiColor = std::make_tuple(0.0f, 0.0f, 1.0f);
		break;
	case 26:
		voronoiColor = std::make_tuple(1.0f, 0.5f, 0.0f);
		break;
	case 27:
		voronoiColor = std::make_tuple(1.0f, 0.0f, 1.0f);
		break;
	default:
		voronoiColor = std::make_tuple(1.0f, 1.0f, 1.0f);
		break;
	}
}

void triangulate()
{
	//Reseting previous triangulation
	apexes.clear();
	edges.clear();
	triangles.clear();


	//Sorting points by coordinates
	//-----------------------------
	std::sort(points.begin(), points.end(), points_comparator());


	//Initializing 2-triangulation
	//----------------------------
	int i = 0;

	if (points.size() == 0)
	{
		return;
	}

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

		Triangle t = Triangle(&edges.at(j), &edges.at(edges.size() - 2), &edges.at(edges.size() - 1));

		if (!isTriangleDirect(t))
		{
			t = Triangle(&edges.at(j), &edges.at(edges.size() - 1), &edges.at(edges.size() - 2));
		}

		triangles.push_back(t);

		addTriangleToItsEdges(&triangles.back());

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
	seen_edges.reserve(maxPoints - 1);
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
			triangles.push_back(tr);

			//add triangle to edges
			addTriangleToItsEdges(&triangles.back());
		}

		//clear seen edges for next point
		seen_edges.clear();
		added_edges.clear();
	}
}

void delaunayTriangulation()
{
	std::vector<Edge*> edges_copy;
	edges_copy.reserve(maxPoints * (maxPoints - 1) / 2);

	for (int i = 0; i < edges.size(); ++i)
	{
		edges_copy.push_back(&edges.at(i));
	}

	Edge *a;
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
			a1 = t1->a3;
			a4 = t1->a1;
		}
		else
		{
			a1 = t1->a1;
			a4 = t1->a2;
		}

		if (t2->a1 == a)
		{
			a2 = t2->a3;
			a3 = t2->a2;
		}
		else if (t2->a2 == a)
		{
			a2 = t2->a1;
			a3 = t2->a3;
		}
		else
		{
			a2 = t2->a2;
			a3 = t2->a1;
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

		for (Edge edge : edges)
		{
			if (edge.td == t1 || edge.td == t2)
			{
				edge.td = nullptr;
			}
			if (edge.tg == t1 || edge.tg == t2)
			{
				edge.tg = nullptr;
			}
		}

		a->td = nullptr;
		a->tg = nullptr;

		addTriangleToItsEdges(t1);
		addTriangleToItsEdges(t2);

		if (std::find(edges_copy.begin(), edges_copy.end(), a1) == edges_copy.end())
		{
			edges_copy.push_back(a1);
		}
		if (std::find(edges_copy.begin(), edges_copy.end(), a2) == edges_copy.end())
		{
			edges_copy.push_back(a2);
		}
		if (std::find(edges_copy.begin(), edges_copy.end(), a3) == edges_copy.end())
		{
			edges_copy.push_back(a3);
		}
		if (std::find(edges_copy.begin(), edges_copy.end(), a4) == edges_copy.end())
		{
			edges_copy.push_back(a4);
		}
	}
}

void voronoiDiagram()
{
	voronoi_regions.clear();

	Circle c;
	Apex *s1, *s2, *s3;
	Apex *s4, *s5, *s6;
	std::map<Triangle*, Circle> circonscript_circles;
	std::map<Triangle*, Circle>::iterator it0;
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
			it0 = circonscript_circles.find(edges.at(i).td);
			c1 = it0->second;
			it1 = circonscript_circles.find(edges.at(i).tg);
			c2 = it1->second;

			bounded_edges.insert(std::pair<Edge*, Bounded_Edge>(&edges.at(i), Bounded_Edge(c1.center, c2.center)));
		}
		else
		{
			float dref, d;
			Apex s;

			if (edges.at(i).td == nullptr)
			{
				it1 = circonscript_circles.find(edges.at(i).tg);
				c1 = it1->second;

				middle.x = (edges.at(i).s1->p->x + edges.at(i).s2->p->x) / 2;
				middle.y = (edges.at(i).s1->p->y + edges.at(i).s2->p->y) / 2;

				std::tie(s4, s5, s6) = apexesOfTriangle(edges.at(i).tg);
				if (s4 != edges.at(i).s1 && s4 != edges.at(i).s2)
				{
					dref = (s4->p->x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (s4->p->y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;

					d = (c1.center.x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (c1.center.y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

					if (d != dref)
					{
						continue;
					}
				}
				else if (s5 != edges.at(i).s1 && s5 != edges.at(i).s2)
				{
					dref = (s5->p->x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (s5->p->y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;

					d = (c1.center.x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (c1.center.y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

					if (d != dref)
					{
						continue;
					}
				}
				else if (s6 != edges.at(i).s1 && s6 != edges.at(i).s2)
				{
					dref = (s6->p->x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (s6->p->y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;

					d = (c1.center.x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (c1.center.y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

					if (d != dref)
					{
						continue;
					}
				}
			}
			else
			{
				it1 = circonscript_circles.find(edges.at(i).td);
				c1 = it1->second;

				middle.x = (edges.at(i).s1->p->x + edges.at(i).s2->p->x) / 2;
				middle.y = (edges.at(i).s1->p->y + edges.at(i).s2->p->y) / 2;

				std::tie(s4, s5, s6) = apexesOfTriangle(edges.at(i).td);
				if (s4 != edges.at(i).s1 && s4 != edges.at(i).s2)
				{
					dref = (s4->p->x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (s4->p->y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;

					d = (c1.center.x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (c1.center.y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

					if (d != dref)
					{
						continue;
					}
				}
				else if (s5 != edges.at(i).s1 && s5 != edges.at(i).s2)
				{
					dref = (s5->p->x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (s5->p->y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;

					d = (c1.center.x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (c1.center.y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

					if (d != dref)
					{
						continue;
					}
				}
				else if (s6 != edges.at(i).s1 && s6 != edges.at(i).s2)
				{
					dref = (s6->p->x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (s6->p->y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					dref = (dref < 0) ? -1 : (dref > 0) ? 1 : 0;

					d = (c1.center.x - edges.at(i).s1->p->x) * (edges.at(i).s2->p->y - edges.at(i).s1->p->y)
						- (c1.center.y - edges.at(i).s1->p->y) * (edges.at(i).s2->p->x - edges.at(i).s1->p->x);
					d = (d < 0) ? -1 : (d > 0) ? 1 : 0;

					if (d != dref)
					{
						continue;
					}
				}
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
				it2 = bounded_edges.find(&edges.at(j));
				if (it2 == bounded_edges.end())
				{
					continue;
				}
				region.push_back(it2->second);
			}
		}

		voronoi_regions.insert(std::pair<Apex*, std::vector<Bounded_Edge>>(&apexes.at(i), region));
		region.clear();
	}
}

void delaunayAddPoint()
{
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
				addTriangleToItsEdges(&triangles.back());
			}
		}
	}
	else
	{
		bool isInTriangle = false, needToContinue = false;
		std::vector<Edge*> seen_edges;
		seen_edges.reserve(maxPoints * (maxPoints - 1) / 2);
		Edge* a;
		Circle c;
		Apex *s1, *s2, *s3;
		std::vector<Triangle>::iterator it1;
		std::vector<Edge>::iterator it2;
		Triangle td, tg;

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
				break;
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

			if (a->td != nullptr)
			{
				td = *a->td;
			}
			if (a->tg != nullptr)
			{
				tg = *a->tg;
			}

			if(a->td != nullptr)
			{
				std::tie(s1, s2, s3) = apexesOfTriangle(&td);
				c = circonscriptCircle(s1, s2, s3);

				if (sqrt(pow(c.center.x - apexes.back().p->x, 2) + pow(c.center.y - apexes.back().p->y, 2)) <= c.radius)
				{
					if (a == td.a1)
					{
						if (std::find(seen_edges.begin(), seen_edges.end(), td.a2) == seen_edges.end())
						{
							seen_edges.push_back(td.a2);
							//a->td->a2->td = nullptr;
						}
						if (std::find(seen_edges.begin(), seen_edges.end(), td.a3) == seen_edges.end())
						{
							seen_edges.push_back(td.a3);
							//a->td->a3->td = nullptr;
						}
					}
					else if (a == td.a2)
					{
						if (std::find(seen_edges.begin(), seen_edges.end(), td.a1) == seen_edges.end())
						{
							seen_edges.push_back(td.a1);
							//a->td->a1->td = nullptr;
						}
						if (std::find(seen_edges.begin(), seen_edges.end(), td.a3) == seen_edges.end())
						{
							seen_edges.push_back(td.a3);
							//a->td->a3->td = nullptr;
						}
					}
					else
					{
						if (std::find(seen_edges.begin(), seen_edges.end(), td.a1) == seen_edges.end())
						{
							seen_edges.push_back(td.a1);
							//a->td->a1->td = nullptr;
						}
						if (std::find(seen_edges.begin(), seen_edges.end(), td.a2) == seen_edges.end())
						{
							seen_edges.push_back(td.a2);
							//a->td->a2->td = nullptr;
						}
					}

					for (int i = 0; i < edges.size(); ++i)
					{
						if (&edges.at(i) == a)
						{
							continue;
						}
						if (edges.at(i).td == a->td)
						{
							edges.at(i).td = nullptr;
						}
						if (edges.at(i).tg == a->td)
						{
							edges.at(i).tg = nullptr;
						}
					}

					if (a->tg != nullptr)
					{
						if (a == tg.a1)
						{
							if (std::find(seen_edges.begin(), seen_edges.end(), tg.a2) == seen_edges.end())
							{
								seen_edges.push_back(tg.a2);
								//a->td->a2->td = nullptr;
							}
							if (std::find(seen_edges.begin(), seen_edges.end(), tg.a3) == seen_edges.end())
							{
								seen_edges.push_back(tg.a3);
								//a->td->a3->td = nullptr;
							}
						}
						else if (a == tg.a2)
						{
							if (std::find(seen_edges.begin(), seen_edges.end(), tg.a1) == seen_edges.end())
							{
								seen_edges.push_back(tg.a1);
								//a->td->a1->td = nullptr;
							}
							if (std::find(seen_edges.begin(), seen_edges.end(), tg.a3) == seen_edges.end())
							{
								seen_edges.push_back(tg.a3);
								//a->td->a3->td = nullptr;
							}
						}
						else
						{
							if (std::find(seen_edges.begin(), seen_edges.end(), tg.a1) == seen_edges.end())
							{
								seen_edges.push_back(tg.a1);
								//a->td->a1->td = nullptr;
							}
							if (std::find(seen_edges.begin(), seen_edges.end(), tg.a2) == seen_edges.end())
							{
								seen_edges.push_back(tg.a2);
								//a->td->a2->td = nullptr;
							}
						}

						for (int i = 0; i < edges.size(); ++i)
						{
							if (&edges.at(i) == a)
							{
								continue;
							}
							if (edges.at(i).td == a->tg)
							{
								edges.at(i).td = nullptr;
							}
							if (edges.at(i).tg == a->tg)
							{
								edges.at(i).tg = nullptr;
							}
						}
					}

					triangles.erase(std::remove(triangles.begin(), triangles.end(), *a->td), triangles.end());
					edges.erase(std::remove(edges.begin(), edges.end(), *a), edges.end());

					continue;
				}
			}
			if (a->tg != nullptr)
			{
				std::tie(s1, s2, s3) = apexesOfTriangle(&tg);
				c = circonscriptCircle(s1, s2, s3);

				if (sqrt(pow(c.center.x - apexes.back().p->x, 2) + pow(c.center.y - apexes.back().p->y, 2)) <= c.radius)
				{
					if (a == tg.a1)
					{
						if (std::find(seen_edges.begin(), seen_edges.end(), tg.a2) == seen_edges.end())
						{
							seen_edges.push_back(tg.a2);
							//a->tg->a2->tg = nullptr;
						}
						if (std::find(seen_edges.begin(), seen_edges.end(), tg.a3) == seen_edges.end())
						{
							seen_edges.push_back(tg.a3);
							//a->tg->a3->tg = nullptr;
						}
					}
					else if (a == tg.a2)
					{
						if (std::find(seen_edges.begin(), seen_edges.end(), tg.a1) == seen_edges.end())
						{
							seen_edges.push_back(tg.a1);
							//a->tg->a1->tg = nullptr;
						}
						if (std::find(seen_edges.begin(), seen_edges.end(), tg.a3) == seen_edges.end())
						{
							seen_edges.push_back(tg.a3);
							//a->tg->a3->tg = nullptr;
						}
					}
					else
					{
						if (std::find(seen_edges.begin(), seen_edges.end(), tg.a1) == seen_edges.end())
						{
							seen_edges.push_back(tg.a1);
							//a->tg->a1->tg = nullptr;
						}
						if (std::find(seen_edges.begin(), seen_edges.end(), tg.a2) == seen_edges.end())
						{
							seen_edges.push_back(tg.a2);
							//a->tg->a2->tg = nullptr;
						}
					}

					for (int i = 0; i < edges.size(); ++i)
					{
						if (&edges.at(i) == a)
						{
							continue;
						}
						if (edges.at(i).td == a->tg)
						{
							edges.at(i).td = nullptr;
						}
						if (edges.at(i).tg == a->tg)
						{
							edges.at(i).tg = nullptr;
						}
					}

					triangles.erase(std::remove(triangles.begin(), triangles.end(), *a->tg), triangles.end());
					edges.erase(std::remove(edges.begin(), edges.end(), *a), edges.end());

					continue;
				}
			}

			Edge cop = Edge(a->s1, a->s2);
			Edge* cop_p = &(*std::find(edges.begin(), edges.end(), cop));
			Edge *s4 = nullptr, *s5 = nullptr;
			bool s1Exists = false, s2Exists = false;

			for (int i = 0; i < edges.size(); ++i)
			{
				if (edges.at(i).s1 == cop_p->s1 || edges.at(i).s2 == cop_p->s1)
				{
					if (edges.at(i).s1 == &apexes.back() || edges.at(i).s2 == &apexes.back())
					{
						s4 = &edges.at(i);
						s1Exists = true;
					}
				}
				if (edges.at(i).s1 == cop_p->s2 || edges.at(i).s2 == cop_p->s2)
				{
					if (edges.at(i).s1 == &apexes.back() || edges.at(i).s2 == &apexes.back())
					{
						s5 = &edges.at(i);
						s2Exists = true;
					}
				}
			}

			if (!s1Exists)
			{
				edges.push_back(Edge(cop_p->s1, &apexes.back()));
				s4 = &edges.back();
			}

			if (!s2Exists)
			{
				edges.push_back(Edge(cop_p->s2, &apexes.back()));
				s5 = &edges.back();
			}

			apexes.back().a = &edges.back();

			Triangle tr = Triangle(cop_p, s5, s4);

			if (!isTriangleDirect(tr))
			{
				tr = Triangle(cop_p, s4, s5);
			}

			triangles.push_back(tr);
			addTriangleToItsEdges(&triangles.back());
		}
	}
}

void delaunayDeletePoint(Point* p)
{
	Apex s;

	for (int i = 0; i < apexes.size(); ++i)
	{
		if (apexes.at(i).p == p)
		{
			s = apexes.at(i);
			break;
		}
	}

	if (triangles.empty())
	{
		if (apexes.size() == 1)
		{
			apexes.clear();
			points.clear();
		}

		std::vector<Edge*> adjacent_edges = findAdjacentEdges(p);

		if (adjacent_edges.size() == 1)
		{
			edges.erase(std::find(edges.begin(), edges.end(), *adjacent_edges.at(0)));
			apexes.erase(std::find(apexes.begin(), apexes.end(), s));
		}
		else if (adjacent_edges.size() == 2)
		{
			Apex *s1, *s2;

			if (adjacent_edges.at(0)->s1 == &s)
			{
				s1 = adjacent_edges.at(0)->s2;
			}
			else
			{
				s1 = adjacent_edges.at(0)->s1;
			}

			if (adjacent_edges.at(1)->s1 == &s)
			{
				s2 = adjacent_edges.at(1)->s2;
			}
			else
			{
				s2 = adjacent_edges.at(1)->s1;
			}

			edges.erase(std::find(edges.begin(), edges.end(), *adjacent_edges.at(0)));
			edges.erase(std::find(edges.begin(), edges.end(), *adjacent_edges.at(1)));
			apexes.erase(std::find(apexes.begin(), apexes.end(), s));

			edges.push_back(Edge(s1, s2));
		}
	}
	else
	{
		std::vector<Edge*> adjacent_edges = findOrientedAdjacentEdges(p);
		std::vector<Triangle*> adjacent_triangles = findOrientedAdjacentTriangles(adjacent_edges);
		std::vector<Edge*> non_adjacent_edges = findNonAdjacentEdges(adjacent_edges, adjacent_triangles);

		std::map<Apex*, std::vector<Edge*>> adjacent_apexes;

		for (int i = 0; i < non_adjacent_edges.size(); ++i)
		{
			if (adjacent_apexes.find(non_adjacent_edges.at(i)->s1) == adjacent_apexes.end())//std::find(adjacent_apexes.begin(), adjacent_apexes.end(), non_adjacent_edges.at(i)->s1) == adjacent_apexes.end())
			{
				std::vector<Edge*> adj_edges;
				adj_edges.reserve(2);
				adj_edges.push_back(non_adjacent_edges.at(i));
				adjacent_apexes.insert(std::pair<Apex*, std::vector<Edge*>>(non_adjacent_edges.at(i)->s1, adj_edges));
			}
			else
			{
				adjacent_apexes.at(non_adjacent_edges.at(i)->s1).push_back(non_adjacent_edges.at(i));
			}

			if (adjacent_apexes.find(non_adjacent_edges.at(i)->s2) == adjacent_apexes.end())//std::find(adjacent_apexes.begin(), adjacent_apexes.end(), non_adjacent_edges.at(i)->s2) == adjacent_apexes.end())
			{
				std::vector<Edge*> adj_edges;
				adj_edges.reserve(2);
				adj_edges.push_back(non_adjacent_edges.at(i));
				adjacent_apexes.insert(std::pair<Apex*, std::vector<Edge*>>(non_adjacent_edges.at(i)->s2, adj_edges));
			}
			else
			{
				adjacent_apexes.at(non_adjacent_edges.at(i)->s2).push_back(non_adjacent_edges.at(i));
			}
		}

		apexes.erase(std::find(apexes.begin(), apexes.end(), s));

		for (int i = 0; i < adjacent_edges.size(); ++i)
		{
			edges.erase(std::find(edges.begin(), edges.end(), *adjacent_edges.at(i)));
		}

		for (int i = 0; i < adjacent_triangles.size(); ++i)
		{
			triangles.erase(std::find(triangles.begin(), triangles.end(), *adjacent_triangles.at(i)));
		}

		bool isConv, exists;
		Apex *s1, *s2;
		Edge a;
		Triangle tr;
		Circle c;
		
		if (non_adjacent_edges.back()->s1 == non_adjacent_edges.at(0)->s1
			|| non_adjacent_edges.back()->s1 == non_adjacent_edges.at(0)->s2
			|| non_adjacent_edges.back()->s2 == non_adjacent_edges.at(0)->s1
			|| non_adjacent_edges.back()->s2 == non_adjacent_edges.at(0)->s2)
		{

			while (non_adjacent_edges.size() > 3)
			{
				for (std::pair<Apex*, std::vector<Edge*>> pair : adjacent_apexes)
				{
					isConv = true;
					if (pair.second.at(0)->s1 == pair.first)
					{
						s1 = pair.second.at(0)->s2;
					}
					else
					{
						s1 = pair.second.at(0)->s1;
					}

					if (pair.second.at(1)->s1 == pair.first)
					{
						s2 = pair.second.at(1)->s2;
					}
					else
					{
						s2 = pair.second.at(1)->s1;
					}

					if (edgeExists(s1, s2))
					{
						continue;
					}

					a = Edge(s1, s2);
					tr = Triangle(&a, pair.second.at(0), pair.second.at(1));

					if (!isTriangleDirect(tr))
					{
						tr = Triangle(&a, pair.second.at(1), pair.second.at(0));
					}

					c = circonscriptCircle(pair.first, s1, s2);

					for (std::pair<Apex*, std::vector<Edge*>> pair2 : adjacent_apexes)
					{
						if (pair2.first == pair.first
							|| pair2.first == s1
							|| pair2.first == s2)
						{
							continue;
						}

						if (sqrt(pow(pair2.first->p->x - c.center.x , 2) + pow(pair2.first->p->y - c.center.y, 2)) < c.radius)
						{
							isConv = false;
							break;
						}
					}

					if (isConv)
					{
						for (Edge* ar : pair.second)
						{
							non_adjacent_edges.erase(std::find(non_adjacent_edges.begin(), non_adjacent_edges.end(), ar));
						}
						adjacent_apexes.erase(pair.first);
						break;
					}
				}

				edges.push_back(a);
				triangles.push_back(tr);
				non_adjacent_edges.push_back(&edges.back());
			}

			Triangle t = Triangle(non_adjacent_edges.at(0), non_adjacent_edges.at(1), non_adjacent_edges.at(2));

			if (!isTriangleDirect(t))
			{
				t = Triangle(non_adjacent_edges.at(0), non_adjacent_edges.at(2), non_adjacent_edges.at(1));
			}

			triangles.push_back(t);
			addTriangleToItsEdges(&triangles.back());
		}
		else
		{
			bool hasFoundApex = true;

			while (hasFoundApex)
			{
				hasFoundApex = false;

				for (std::pair<Apex*, std::vector<Edge*>> pair : adjacent_apexes)
				{
					isConv = true;

					if (pair.second.at(0)->s1 == pair.first)
					{
						s1 = pair.second.at(0)->s2;
					}
					else
					{
						s1 = pair.second.at(0)->s1;
					}

					if (pair.second.at(1)->s1 == pair.first)
					{
						s2 = pair.second.at(1)->s2;
					}
					else
					{
						s2 = pair.second.at(1)->s1;
					}
					
					if (edgeExists(s1, s2))
					{
						continue;
					}

					a = Edge(s1, s2);
					tr = Triangle(&a, pair.second.at(0), pair.second.at(1));

					if (!isTriangleDirect(tr))
					{
						tr = Triangle(&a, pair.second.at(1), pair.second.at(0));
					}

					c = circonscriptCircle(pair.first, s1, s2);

					for (std::pair<Apex*, std::vector<Edge*>> pair2 : adjacent_apexes)
					{
						if (pair2.first == pair.first
							|| pair2.first == s1
							|| pair2.first == s2)
						{
							continue;
						}

						if (sqrt(pow(pair2.first->p->x - c.center.x, 2) + pow(pair2.first->p->y - c.center.y, 2)) < c.radius)
						{
							isConv = false;
							break;
						}
					}

					if (isConv)
					{
						hasFoundApex = true;
						for (Edge* ar : pair.second)
						{
							non_adjacent_edges.erase(std::find(non_adjacent_edges.begin(), non_adjacent_edges.end(), ar));
						}
						adjacent_apexes.erase(pair.first);
						break;
					}
				}

				edges.push_back(a);
				triangles.push_back(tr);
				non_adjacent_edges.push_back(&edges.back());
			}
		}
	}
}

bool isTriangleDirect(Triangle tr)
{
	Apex *s1 = nullptr, *s2 = nullptr, *s3 = nullptr;

	std::tie(s1, s2, s3) = apexesOfTriangle(&tr);

	return determinant(s1, s2, s3) < 0;
}

bool isTriangleLeft(Triangle tr, Edge* e)
{
	Apex *s1 = nullptr, *s2 = nullptr, *s3 = nullptr;

	if (tr.a1 == e)
	{
		if (tr.a1->s1 == tr.a2->s1 || tr.a1->s2 == tr.a2->s1)
		{
			s1 = tr.a1->s1;
			s2 = tr.a1->s2;
			s3 = tr.a2->s2;
		}
		else if (tr.a1->s1 == tr.a2->s2 || tr.a1->s2 == tr.a2->s2)
		{
			s1 = tr.a1->s1;
			s2 = tr.a1->s2;
			s3 = tr.a2->s1;
		}
	}
	else if (tr.a2 == e)
	{
		if (tr.a2->s1 == tr.a3->s1 || tr.a2->s2 == tr.a3->s1)
		{
			s1 = tr.a2->s1;
			s2 = tr.a2->s2;
			s3 = tr.a3->s2;
		}
		else if (tr.a2->s1 == tr.a3->s2 || tr.a2->s2 == tr.a3->s2)
		{
			s1 = tr.a2->s1;
			s2 = tr.a2->s2;
			s3 = tr.a3->s1;
		}
	}
	else if (tr.a3 == e)
	{
		if (tr.a3->s1 == tr.a1->s1 || tr.a3->s2 == tr.a1->s1)
		{
			s1 = tr.a3->s1;
			s2 = tr.a3->s2;
			s3 = tr.a1->s2;
		}
		else if (tr.a3->s1 == tr.a1->s2 || tr.a3->s2 == tr.a1->s2)
		{
			s1 = tr.a3->s1;
			s2 = tr.a3->s2;
			s3 = tr.a1->s1;
		}
	}

	return determinant(s1, s2, s3) < 0;
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
	Apex *s1, *s2, *s3, *s4, *ts1, *ts2, *ts3;
	Circle circonscript;

	s1 = a->s1;
	s2 = a->s2;

	std::tie(ts1, ts2, ts3) = apexesOfTriangle(a->td);
	if (ts1 != s1 && ts1 != s2)
	{
		s3 = ts1;
	}
	else if (ts2 != s1 && ts2 != s2)
	{
		s3 = ts2;
	}
	else
	{
		s3 = ts3;
	}

	std::tie(ts1, ts2, ts3) = apexesOfTriangle(a->tg);
	if (ts1 != s1 && ts1 != s2)
	{
		s4 = ts1;
	}
	else if (ts2 != s1 && ts2 != s2)
	{
		s4 = ts2;
	}
	else
	{
		s4 = ts3;
	}

	//Pour td
	circonscript = circonscriptCircle(s1, s2, s3);

	if (sqrt(pow(s4->p->x - circonscript.center.x, 2) + pow(s4->p->y - circonscript.center.y, 2)) <= circonscript.radius)
	{
		return false;
	}

	//Pour tg
	circonscript = circonscriptCircle(s1, s2, s4);

	if (sqrt(pow(s3->p->x - circonscript.center.x, 2) + pow(s3->p->y - circonscript.center.y, 2)) <= circonscript.radius)
	{
		return false;
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

	float x1, x2, y1, y2;
	float a1, a2, b1, b2;

	x1 = (s1->p->x + s2->p->x) / 2;
	y1 = (s1->p->y + s2->p->y) / 2;
	x2 = (s1->p->x + s3->p->x) / 2;
	y2 = (s1->p->y + s3->p->y) / 2;

	a1 = (s1->p->y - s2->p->y) / (s1->p->x - s2->p->x);
	a2 = (s1->p->y - s3->p->y) / (s1->p->x - s3->p->x);

	b1 = y1 + 1 / a1 * x1;
	b2 = y2 + 1 / a2 * x2;

	c.center.x = (b2 - b1) / (1 / a2 - 1 / a1);
	c.center.y = -1 / a1 * c.center.x + b1;
	
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
		tref = (edges.at(k).td != nullptr) ? edges.at(k).td : edges.at(k).tg;

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

std::vector<Edge*> findAdjacentEdges(Point* p)
{
	std::vector<Edge*> adjacent_edges;
	adjacent_edges.reserve(edges.size());

	for (int i = 0; i < edges.size(); ++i)
	{
		if (edges.at(i).s1->p == p || edges.at(i).s2->p == p)
		{
			adjacent_edges.push_back(&edges.at(i));
		}
	}

	return adjacent_edges;
}

float orientedAngle(Edge* a1, Edge* a2, Point* p)
{
	Apex *s = nullptr, *s1 = nullptr, *s2 = nullptr;

	for (int i = 0; i < apexes.size(); ++i)
	{
		if (apexes.at(i).p == p)
		{
			s = &apexes.at(i);
			continue;
		}
		if (apexes.at(i).p == a1->s1->p)
		{
			s1 = &apexes.at(i);
			continue;
		}
		if (apexes.at(i).p == a1->s2->p)
		{
			s1 = &apexes.at(i);
			continue;
		}
		if (apexes.at(i).p == a2->s1->p)
		{
			s2 = &apexes.at(i);
			continue;
		}
		if (apexes.at(i).p == a2->s2->p)
		{
			s2 = &apexes.at(i);
			continue;
		}
	}

	float angle = atan2(s2->p->y - s->p->y, s2->p->x - s->p->x) - atan2(s1->p->y - s->p->y, s1->p->x - s->p->x);
	if (angle < 0)
	{
		angle += 2 * PI;
	}

	return angle;
}

std::vector<Edge*> findOrientedAdjacentEdges(Point* p)
{
	std::map<Edge*, float> oriented_angles;
	std::vector<Edge*> adjacent_edges = findAdjacentEdges(p);
	adjacent_edges.reserve(edges.size());
	Edge* aref = adjacent_edges.at(0);

	for (int i = 1; i < adjacent_edges.size(); ++i)
	{
		oriented_angles.insert(std::pair<Edge*, float>(adjacent_edges.at(i), orientedAngle(aref, adjacent_edges.at(i), p)));
	}

	typedef std::function<bool(std::pair<Edge*, float>, std::pair<Edge*, float>)> Comparator;

	Comparator compFunctor = [](std::pair<Edge*, float> elem1, std::pair<Edge*, float> elem2)
	{
		return elem1.second < elem2.second;
	};

	std::set<std::pair<Edge*, float>, Comparator> setOfAngles(oriented_angles.begin(), oriented_angles.end(), compFunctor);

	adjacent_edges.clear();
	adjacent_edges.push_back(aref);
	for (std::pair<Edge*, float> element : setOfAngles)
	{
		adjacent_edges.push_back(element.first);
	}

	return adjacent_edges;
}

std::vector<Triangle*> findOrientedAdjacentTriangles(std::vector<Edge*> adjacent_edges)
{
	std::vector<Triangle*> adjacent_triangles;
	adjacent_triangles.reserve(adjacent_edges.size());

	std::vector<Edge*>::iterator it1;
	std::vector<Edge*>::iterator it2;
	std::vector<Edge*>::iterator it3;

	for (int i = 0; i < adjacent_edges.size(); ++i)
	{
		for (int j = 0; j < triangles.size(); ++j)
		{
			it1 = std::find(adjacent_edges.begin(), adjacent_edges.end(), triangles.at(j).a1);
			it2 = std::find(adjacent_edges.begin(), adjacent_edges.end(), triangles.at(j).a2);
			it3 = std::find(adjacent_edges.begin(), adjacent_edges.end(), triangles.at(j).a3);

			if ((it1 == adjacent_edges.end() && it2 == adjacent_edges.end())
				|| (it1 == adjacent_edges.end() && it3 == adjacent_edges.end())
				|| (it2 == adjacent_edges.end() && it3 == adjacent_edges.end()))
			{
				continue;
			}

			if (std::find(adjacent_triangles.begin(), adjacent_triangles.end(), &triangles.at(j)) == adjacent_triangles.end())
			{
				adjacent_triangles.push_back(&triangles.at(j));
			}
			break;
		}
	}

	return adjacent_triangles;
}

std::vector<Edge*> findNonAdjacentEdges(std::vector<Edge*> adjacent_edges, std::vector<Triangle*> adjacent_triangles)
{
	std::vector<Edge*> non_adjacent_edges;
	non_adjacent_edges.reserve(edges.size());

	for (int i = 0; i < triangles.size(); ++i)
	{
		if (std::find(adjacent_edges.begin(), adjacent_edges.end(), triangles.at(i).a1) == adjacent_edges.end())
		{
			non_adjacent_edges.push_back(triangles.at(i).a1);
			continue;
		}

		if (std::find(adjacent_edges.begin(), adjacent_edges.end(), triangles.at(i).a2) == adjacent_edges.end())
		{
			non_adjacent_edges.push_back(triangles.at(i).a2);
			continue;
		}
			
		non_adjacent_edges.push_back(triangles.at(i).a3);
	}

	return non_adjacent_edges;
}

void addTriangleToItsEdges(Triangle* t)
{
	if (isTriangleLeft(*t, t->a1))
	{
		t->a1->tg = t;
	}
	else
	{
		t->a1->td = t;
	}

	if (isTriangleLeft(*t, t->a2))
	{
		t->a2->tg = t;
	}
	else
	{
		t->a2->td = t;
	}

	if (isTriangleLeft(*t, t->a3))
	{
		t->a3->tg = t;
	}
	else
	{
		t->a3->td = t;
	}
}

bool edgeExists(Apex* s1, Apex* s2)
{
	for (Edge edge : edges)
	{
		if ((edge.s1 == s1 && edge.s2 == s2)
			|| (edge.s1 == s2 && edge.s2 == s1))
		{
			return true;
		}
	}

	return false;
}
