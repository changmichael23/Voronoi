#include <string>
#include <vector>
#include <ctime>
#include "Colore.hpp"
#include "freeglut.h"
int sizeCube = 3;

struct Point3D {
	float x;
	float y;
	float z;
	float n1;
	float n2;
	float n3;
	Point3D()
	{
		x = 0;
		y = 0;
		z = 0;
		n1 = 0;
		n2 = 0;
		n3 = 0;
	}

	Point3D(float a, float b, float c, float d = 0.0f, float e = 0.0f, float f = 0.0f)
	{
		x = a;
		y = b;
		z = c;
		n1 = d;
		n2 = e;
		n3 = f;
	}

	bool operator==(const Point3D &p)
	{
		if (x == p.x && y == p.y && z == p.z)
		{
			return true;
		}
		return false;
	}

	std::string toString()
	{
		return "x = " + std::to_string(x) + ", y = " + std::to_string(y) + ", z =" + std::to_string(z);
	}
};

float * structToTab(std::vector<Point3D> newPoints)
{
	float* tabP = new float[newPoints.size() * 6];
	int j = 0;
	for (int i = 0; i < newPoints.size() * 6; i += 6)
	{
		tabP[i] = newPoints[j].x;
		tabP[i + 1] = newPoints[j].y;
		tabP[i + 2] = newPoints[j].z;

		tabP[i + 3] = newPoints[j].n1;
		tabP[i + 4] = newPoints[j].n2;
		tabP[i + 5] = newPoints[j].n3;
		j++;
	}

	return tabP;
}
float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
std::vector<Point3D> createRandomPoints(int n)
{
	std::vector<Point3D> tmp;

	for (int i = 0; i < 7; i++)
	{
		tmp.push_back(Point3D(RandomFloat(-500,500), RandomFloat(-500, 500), RandomFloat(-500, 500), 0, 0, 0));
		
	}
	return tmp;
}

std::vector<Point3D> transformPointsToCube(std::vector<Point3D> p)
{
	std::vector<Point3D> tmp;
	int j = 0;
	for (int i = 0; i < p.size(); i++)
	{
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, -1.0f, 0.0f, 0.0f));

		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, -1.0f, 0.0f));

		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point3D(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point3D(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f));

		j += 8;
	}

	return tmp;

}

std::vector<Point3D> createVoronoiExtCube()
{
	std::vector<Point3D> tmp;
	int sizeCubeLocal = 400;
	Point3D tmpP = Point3D(0, 0, -250, 0, 0, 0);

		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, -1.0f, 0.0f, 0.0f));

		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, -1.0f, 0.0f));

		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal-0.5f, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point3D(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal - 0.5f, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal - 0.5f, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point3D(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal - 0.5f, 0.0f, 0.0f, +1.0f));


	return tmp;

}

std::vector<Point3D> createVoronoi2DFaces(std::vector<int> *indVector)
{
	std::vector<std::vector<Point3D>> tmp;

	std::vector<Point3D> t;
	t.push_back(Point3D(0, 100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(0, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(100, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(100, 100, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	indVector->push_back(4);
	//t.clear();

	t.push_back(Point3D(0, 100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(0, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(-100, 0, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(-100, 100, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	indVector->push_back(4);
	//t.clear();

	t.push_back(Point3D(-100, 0, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(-50, 25, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(-50, -100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(-100, -100, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	indVector->push_back(4);
	//t.clear();


	//t.push_back(Point3D(-50, 25, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point3D(0, 50, 150, 0.0f, 0.0f, +1.0f));
	////t.push_back(Point3D(50, 50, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point3D(25, 50, 150, 0.0f, 0.0f, +1.0f));
	////t.push_back(Point3D(50, -25, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point3D(25, -25, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point3D(-50, -25, 150, 0.0f, 0.0f, +1.0f));
	//tmp.push_back(t);
	////t.clear();
	//indVector->push_back(5);

	//
	//t.push_back(Point3D(25, 50, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point3D(50, 50, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point3D(50, -25, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point3D(25, -25, 150, 0.0f, 0.0f, +1.0f));

	//indVector->push_back(4);



	t.push_back(Point3D(50, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(100, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(100, -100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(50, -100, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	//t.clear();
	indVector->push_back(4);


	t.push_back(Point3D(50, -25, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(50, -100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(-50, -100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point3D(-50, -25, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	//t.clear();
	indVector->push_back(4);


	return t;
}

// Create new faces from previous Point3D with new Point3Ds based on final Point3D and delta, updates indexVector
std::vector<Point3D> newIterationPoint(std::vector<Point3D> voronoiIns, std::vector<Point3D> * newTabPoint3DsTmp, std::vector<int> *indVector, float ind = 1, Point3D finalPoint3D = Point3D(0, 0, -300.0f, 0, 0, 0))
{
	//Point3D finalPoint3D = Point3D(0, 0, -300.0f, 0, 0, 0);
	std::vector<Point3D> newTabPoint3Ds;

	newTabPoint3DsTmp->clear();
	//push new Point3Ds
	for (int i = 0; i < voronoiIns.size(); i++)
	{
		std::vector<float> vectorToFinal;
		vectorToFinal.push_back(finalPoint3D.x - voronoiIns[i].x);
		vectorToFinal.push_back(finalPoint3D.y - voronoiIns[i].y);
		vectorToFinal.push_back(finalPoint3D.z - voronoiIns[i].z);

		Point3D tmp = Point3D(Point3D(voronoiIns[i].x + vectorToFinal[0] * ind, voronoiIns[i].y + vectorToFinal[1] * ind, voronoiIns[i].z + vectorToFinal[2] * ind, 0.0f, 0.0f, 1.0f));
		newTabPoint3Ds.push_back(tmp);
		newTabPoint3DsTmp->push_back(tmp);
	}
	indVector->push_back(voronoiIns.size());


	for (int i = 0; i < voronoiIns.size(); i++)
	{
		int j = (i + 1) % voronoiIns.size();
		newTabPoint3Ds.push_back(newTabPoint3DsTmp->at(i));
		newTabPoint3Ds.push_back(Point3D(voronoiIns[i].x, voronoiIns[i].y, voronoiIns[i].z, 0.0f, 0.0f, 1.0f));
		newTabPoint3Ds.push_back(Point3D(voronoiIns[j].x, voronoiIns[j].y, voronoiIns[j].z, 0.0f, 0.0f, 1.0f));
		newTabPoint3Ds.push_back(newTabPoint3DsTmp->at(j));
		

		indVector->push_back(4);
	}


	return newTabPoint3Ds;
}

std::vector<Point3D> combineVector(std::vector<Point3D> t, std::vector<Point3D> t1)
{
	std::vector<Point3D> tmp;
	for (int i = 0; i < t.size(); i++)
	{
		tmp.push_back(t[i]);
	}
	for (int i = 0; i < t1.size(); i++)
	{
		tmp.push_back(t1[i]);
	}

	return tmp;
}

int* firstsFromVectorIndex(std::vector<int>* indVector)
{
	int *tmp = new int[indVector->size()];
	tmp[0] = 0;
	for (int i = 1; i < indVector->size(); i++)
	{

		tmp[i] = indVector->at(i - 1) + tmp[i - 1];
	}

	return tmp;
}

int* countFromVectorIndex(std::vector<int>* indVector)
{
	int *tmp = new int[indVector->size()];
	for (int i = 0; i < indVector->size(); i++)
	{
		tmp[i] = indVector->at(i);
	}


	return tmp;
}

int nbCountFromVectorIndex(std::vector<int>* indVector)
{
	return indVector->size();
}

std::vector<Point3D> algo3d(std::vector<std::vector<Point3D>> voronoiIns, std::vector<int> *indVector, float delta = 0.1, Point3D finalPoinet3D = Point3D(0, 0, 0, 0, 0, 0))
{
	std::vector<Point3D> localBuffer;
	float localDelta = delta;
	std::vector<std::vector<Point3D>> currentTabPoint3DsTmp;
	std::vector<Point3D> * newTabPoint3DsTmp = new std::vector<Point3D>();
	

	for (int i = 0; i < voronoiIns.size(); i++)
	{
		for (int k = 0; k < voronoiIns[i].size(); k++)
		{
			int j = (k + 1) % voronoiIns[i].size();
			localBuffer.push_back(Point3D(voronoiIns[i][k].x, voronoiIns[i][k].y, voronoiIns[i][k].z, 0.0f, 0.0f, 1.0f));
			localBuffer.push_back(Point3D(voronoiIns[i][j].x, voronoiIns[i][j].y, voronoiIns[i][j].z, 0.0f, 0.0f, 1.0f));

			indVector->push_back(2);
		}
	}

		while (localDelta< 1.01 && localDelta >=0)
		{
				Point3D finalPoint3D = Point3D(RandomFloat(-10, 10) / localDelta, RandomFloat(-10, 10) / localDelta, 0, 0.0f, 0.0f, 1.0f);
				for (int i = 0; i < voronoiIns.size(); i++)
				{

					if (localDelta == delta)
					{
						currentTabPoint3DsTmp.push_back(voronoiIns[i]);
					}
					localBuffer = combineVector(localBuffer, newIterationPoint(currentTabPoint3DsTmp[i], newTabPoint3DsTmp, indVector, localDelta, finalPoint3D));
					currentTabPoint3DsTmp[i] = *newTabPoint3DsTmp;

				}

				localDelta = 1.0 / (1 / localDelta - 1);
			}
			
		

	return localBuffer;
}

GLushort* createInd(int n)
{
	GLushort* tmp = new GLushort[n];
	for (int i = 0; i < n; i++)
	{
		tmp[i] = i;
	}
	return tmp;
}