#include <string>
#include <vector>
#include <ctime>
int sizeCube = 3;

struct Point {
	float x;
	float y;
	float z;
	float n1;
	float n2;
	float n3;
	Point()
	{
		x = 0;
		y = 0;
		z = 0;
		n1 = 0;
		n2 = 0;
		n3 = 0;
	}

	Point(float a, float b, float c, float d = 0.0f, float e = 0.0f, float f = 0.0f)
	{
		x = a;
		y = b;
		z = c;
		n1 = d;
		n2 = e;
		n3 = f;
	}

	bool operator==(const Point &p)
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

float * structToTab(std::vector<Point> newPoints)
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
std::vector<Point> createRandomPoints(int n)
{
	std::vector<Point> tmp;

	for (int i = 0; i < 7; i++)
	{
		tmp.push_back(Point(RandomFloat(-500,500), RandomFloat(-500, 500), RandomFloat(-500, 500), 0, 0, 0));
		
	}
	return tmp;
}

std::vector<Point> transformPointsToCube(std::vector<Point> p)
{
	std::vector<Point> tmp;
	int j = 0;
	for (int i = 0; i < p.size(); i++)
	{
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, -1.0f, 0.0f, 0.0f));

		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, -1.0f, 0.0f));

		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z - sizeCube, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point(p[i].x + sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y - sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point(p[i].x - sizeCube, p[i].y + sizeCube, p[i].z + sizeCube, 0.0f, 0.0f, +1.0f));

		j += 8;
	}

	return tmp;

}

std::vector<Point> createVoronoiExtCube()
{
	std::vector<Point> tmp;
	int sizeCubeLocal = 100;
	Point tmpP = Point(0, 0, sizeCubeLocal / 2, 0, 0, 0);

		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, +1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, -1.0f, 0.0f, 0.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, -1.0f, 0.0f, 0.0f));

		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, +1.0f, 0.0f));
		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, -1.0f, 0.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, -1.0f, 0.0f));

		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, 0.0f, -1.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z - sizeCubeLocal, 0.0f, 0.0f, -1.0f));
		/*tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point(tmpP.x + sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y - sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, 0.0f, +1.0f));
		tmp.push_back(Point(tmpP.x - sizeCubeLocal, tmpP.y + sizeCubeLocal, tmpP.z + sizeCubeLocal, 0.0f, 0.0f, +1.0f));*/


	return tmp;

}

std::vector<Point> createVoronoi2DFaces(std::vector<int> *indVector)
{
	std::vector<std::vector<Point>> tmp;

	std::vector<Point> t;
	t.push_back(Point(0, 100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(0, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(100, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(100, 100, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	indVector->push_back(4);
	//t.clear();

	t.push_back(Point(0, 100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(0, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(-100, 0, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(-100, 100, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	indVector->push_back(4);
	//t.clear();

	t.push_back(Point(-100, 0, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(-50, 25, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(-50, -100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(-100, -100, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	indVector->push_back(4);
	//t.clear();


	t.push_back(Point(-50, 25, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(0, 50, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point(50, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(25, 50, 150, 0.0f, 0.0f, +1.0f));
	//t.push_back(Point(50, -25, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(25, -25, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(-50, -25, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	//t.clear();
	indVector->push_back(5);

	
	t.push_back(Point(25, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(50, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(50, -25, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(25, -25, 150, 0.0f, 0.0f, +1.0f));

	indVector->push_back(4);



	t.push_back(Point(50, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(100, 50, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(100, -100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(50, -100, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	//t.clear();
	indVector->push_back(4);


	t.push_back(Point(50, -25, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(50, -100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(-50, -100, 150, 0.0f, 0.0f, +1.0f));
	t.push_back(Point(-50, -25, 150, 0.0f, 0.0f, +1.0f));
	tmp.push_back(t);
	//t.clear();
	indVector->push_back(4);


	return t;
}

// Create new faces from previous point with new points based on final point and delta, updates indexVector
std::vector<Point> newIterationPoint(std::vector<Point> voronoiIns, std::vector<Point> * newTabPointsTmp, std::vector<int> *indVector, float ind = 1, Point finalPoint = Point(0, 0, 0, 0, 0, 0))
{
	std::vector<Point> newTabPoints;

	newTabPointsTmp->clear();
	//push new Points
	for (int i = 0; i < voronoiIns.size(); i++)
	{
		std::vector<float> vectorToFinal;
		vectorToFinal.push_back(finalPoint.x - voronoiIns[i].x);
		vectorToFinal.push_back(finalPoint.y - voronoiIns[i].y);
		vectorToFinal.push_back(finalPoint.z - voronoiIns[i].z);

		Point tmp = Point(Point(voronoiIns[i].x + vectorToFinal[0] * ind, voronoiIns[i].y + vectorToFinal[1] * ind, voronoiIns[i].z + vectorToFinal[2] * ind, 0.0f, 0.0f, 1.0f));
		newTabPoints.push_back(tmp);
		newTabPointsTmp->push_back(tmp);
	}
	indVector->push_back(voronoiIns.size());


	for (int i = 0; i < voronoiIns.size(); i++)
	{
		int j = (i + 1) % voronoiIns.size();
		newTabPoints.push_back(Point(voronoiIns[i].x, voronoiIns[i].y, voronoiIns[i].z, 0.0f, 0.0f, 1.0f));
		newTabPoints.push_back(Point(voronoiIns[j].x, voronoiIns[j].y, voronoiIns[j].z, 0.0f, 0.0f, 1.0f));
		newTabPoints.push_back(newTabPointsTmp->at(j));
		newTabPoints.push_back(newTabPointsTmp->at(i));

		indVector->push_back(4);
	}


	return newTabPoints;
}

std::vector<Point> combineVector(std::vector<Point> t, std::vector<Point> t1)
{
	std::vector<Point> tmp;
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

std::vector<Point> algo3d(std::vector<std::vector<Point>> voronoiIns, std::vector<int> *indVector, float delta = 0.1, Point finalPoint = Point(0, 0, 0, 0, 0, 0))
{
	std::vector<Point> localBuffer;
	float localDelta = delta;
	std::vector<std::vector<Point>> currentTabPointsTmp;
	std::vector<Point> * newTabPointsTmp = new std::vector<Point>();
	
		while (localDelta< 1.01 && localDelta >=0)
		{
				finalPoint = Point(RandomFloat(-1, 1) / localDelta, RandomFloat(-1, 1) / localDelta, 0, 0.0f, 0.0f, 1.0f);
				for (int i = 0; i < voronoiIns.size(); i++)
				{

					if (localDelta == delta)
					{
						currentTabPointsTmp.push_back(voronoiIns[i]);
					}
					localBuffer = combineVector(localBuffer, newIterationPoint(currentTabPointsTmp[i], newTabPointsTmp, indVector, localDelta, finalPoint));
					currentTabPointsTmp[i] = *newTabPointsTmp;

				}

				localDelta = 1.0 / (1 / localDelta - 1);
			}
			
		

	return localBuffer;
}