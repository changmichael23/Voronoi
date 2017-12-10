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