#include "Fonctions.h"

#define _USE_MATH_DEFINES
#include <math.h>

template <class TPoint>
glm::vec2 makeVector(TPoint p1, TPoint p2)
{
	int x = p2.x - p1.x;
	int y = p2.y - p1.y;
	return glm::vec2(x, y);
}

template <class TPoint>
int normVector(TPoint a, TPoint b)
{
	int x = pow(b.x - a.x, 2);
	int y = pow(b.y - a.y, 2);
	return sqrt(x + y);
}

int normVector(glm::vec2 vector)
{
	int x = pow(vector.x, 2);
	int y = pow(vector.y, 2);
	return sqrt(x + y);
}

template <class TPoint>
double angleVec(glm::vec2 vector, TPoint p1, TPoint p2)
{
	glm::vec2 vectorPoint = makeVector(p1, p2);
	int norm_vector_points = normVector(p1, p2);
	int norm_vector = normVector(vector);

	double dot = glm::dot(vectorPoint, vector);

	int determinant = vectorPoint.x * vector.y - vectorPoint.y * vector.x;
	double result = dot / (norm_vector_points * norm_vector);

	if (result < -1.0) result = -1.0;
	else if (result > 1.0) result = 1.0;
	double angle = acos(result);

	if (determinant == 0) // colinear
		return INFINITY;
	return angle * 180 / M_PI;
}

std::vector<Point2D> jarvisMarch(std::vector<Point2D> vecpoints)
{

	int n = vecpoints.size();
	std::vector<Point2D> points;

	int iBas = 0;
	Point2D pBas = (Point2D)vecpoints[0];
	for (int i = 1; i < vecpoints.size(); i++) {
		Point2D p = ((Point2D)vecpoints[i]);
		if (p.y > pBas.y || (p.y == pBas.y && p.x > pBas.x)) {
			pBas = p;
			iBas = i;
		}
	}
	points.push_back(pBas);

	int first_index = iBas;
	int i = first_index;
	int j = -1, inew = -1;

	glm::vec2 vectorDirector = makeVector(Point2D(0, 0), Point2D(0, 1));
	do {
		if (points.size() > vecpoints.size()) {
			return points;
		}
		if (i == 0) { j = 1; }
		else { j = 0; }

		double min_angle = angleVec(vectorDirector, vecpoints[i], vecpoints[j]);
		int max_length = normVector(vecpoints[i], vecpoints[j]);
		inew = j;
		for (j = inew + 1; j < n; j++)
		{
			if (j != i)
			{
				double current_angle = angleVec(vectorDirector, vecpoints[i], vecpoints[j]);
				if (min_angle > current_angle || (current_angle == min_angle && max_length < normVector(points[i], points[j])))
				{
					min_angle = current_angle;
					max_length = normVector(vecpoints[i], vecpoints[j]);
					inew = j;
				}
			}
		}
		vectorDirector = makeVector(vecpoints[i], vecpoints[inew]);
		i = inew;
		points.push_back(vecpoints[inew]);


	} while (i != first_index);

	return points;
}

Point2D barycentre(std::vector<Point2D> vecpoints) 
{
	float barX = 0, barY = 0;
	for (int i = 0; i < vecpoints.size(); i++) {
		barX += vecpoints[i].x;
		barY += vecpoints[i].y;
	}
	barX /= vecpoints.size();
	barY /= vecpoints.size();

	Point2D ret = Point2D(barX, barY);
	return ret;
}

double dotProduct(glm::vec2 vecA, glm::vec2 vecB)
{
	return glm::dot(vecA, vecB);
}

template<class TPoint>
double fullAngle(glm::vec2 vector, TPoint p1, TPoint p2)
{
	glm::vec2 vectorPoint = makeVector(p1, p2);
	int norm_vector_points = normVector(p1, p2);
	int norm_vector = normVector(vector);

	double dot = dotProduct(vectorPoint, vector);

	int determinant = vectorPoint.x * vector.y - vectorPoint.y * vector.x;
	double result = dot / (norm_vector_points * norm_vector);

	if (result < -1.0) result = -1.0;
	else if (result > 1.0) result = 1.0;
	double angle = acos(result);
	angle = atan2(determinant, dot);
	if (determinant == 0)
		return INFINITY;
	if (angle < 0)
		return 360 - (angle * 180 / M_PI)*-1;
	else
		return angle * 180 / M_PI;
}

bool isConvexPoint(Point2D p, Point2D prevPoint, Point2D nextPoint)
{
	glm::vec2 vector1 = glm::vec2(prevPoint.x - p.x, prevPoint.y - p.y);
	glm::vec2 vector2 = glm::vec2(nextPoint.x - p.x, nextPoint.y - p.y);

	double a = fullAngle(vector1, p, nextPoint);
	if (a >= 180)
		return true;
	return false;
}

std::vector<Point2D> findAndSuppressConcavePoints(std::vector<Point2D> vecpoints)
{
	Point2D saveBegin;
	int initial;
	if (vecpoints.size() > 0)
		initial = 0;
	else
		return std::vector<Point2D>();


	std::vector<Point2D> L = vecpoints;
	int pivot = 0;
	int next = vecpoints.size() > 1 ? 1 : 0;
	int previous = vecpoints.size() - 1;
	bool go = false;
	do {
		if (isConvexPoint(vecpoints[pivot], vecpoints[previous], vecpoints[next]))
		{
			previous = pivot;
			pivot = next;
			next = next + 1 < vecpoints.size() ? next + 1 : 0;
			go = true;
		}
		else
		{
			initial = previous;
			if (std::find(L.begin(), L.end(), vecpoints[pivot]) != L.end()) {
				L.erase(std::remove(L.begin(), L.end(), vecpoints[pivot]), L.end());
			}
			if (std::find(vecpoints.begin(), vecpoints.end(), vecpoints[pivot]) != vecpoints.end()) {
				vecpoints.erase(std::remove(vecpoints.begin(), vecpoints.end(), vecpoints[pivot]), vecpoints.end());
			}
			pivot = initial;
			next = pivot + 1 < vecpoints.size() ? pivot + 1 : 0;
			previous = pivot - 1 >= 0 ? pivot - 1 : vecpoints.size() - 1;

			if (pivot >= vecpoints.size())
			{
				initial = 0;
				pivot = 0;
				next = 1;
				previous = vecpoints.size() - 1;
			}

			go = false;
		}
	} while (pivot != initial || go == false);

	L.push_back(L[0]);

	return L;
}

std::vector<Point2D> grahamScan(std::vector<Point2D> vecpoints)
{
	int n = vecpoints.size();
	int foundPoints = 0;
	std::vector<Point2D> points;

	if (n < 3)
		return points;

	Point2D bar = barycentre(vecpoints);

	double min_angle = 999;
	int smallerDotProdIndex = 0;
	float distanceToBar = 99999;
	

	do
	{
		min_angle = 999;
		smallerDotProdIndex = 0;
		for (int i = 0; i < vecpoints.size(); i++)
		{
			glm::vec2 a = glm::vec2(1, 0);
			glm::vec2 b = glm::vec2(vecpoints[i].x - bar.x, vecpoints[i].y - bar.y);
			double currentAngle = fullAngle(a, bar, vecpoints[i]);

			
			if (currentAngle <= min_angle)
			{
				distanceToBar = glm::distance(glm::vec2(vecpoints[i].x, vecpoints[i].y), glm::vec2(bar.x, bar.y));
				min_angle = currentAngle;
				smallerDotProdIndex = i;
			}

		}
		points.push_back(vecpoints[smallerDotProdIndex]);
		vecpoints.erase(vecpoints.begin() + smallerDotProdIndex);
		foundPoints++;

	} while (foundPoints < n);
	points = findAndSuppressConcavePoints(points);
	//points.push_back(saveBegin);

	return points;
}