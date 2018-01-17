#pragma once
#include <vector>
#include "Point.hpp"

#ifndef PATCH_HPP
#define PATCH_HPP

extern int precision;
extern RotateMode rm;

struct Patch
{
	int n, m;
	std::vector<Point> Controlpoints;
	std::vector<Point> gridPoints;

	Patch()
	{
		n = 0;
		m = 0;
	}

	Patch(int _n, int _m)
	{
		n = _n;
		m = _m;

		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				Point p = Point(i, .0f, j);
				Controlpoints.push_back(p);
			}
		}
	}

	void MovePoint(int i, float step)
	{
		Controlpoints.at(i).y += step;
	}

	void Rotate(int i, float step)
	{
		if (rm == quaternion)
		{

		}
		else if (rm == matrix)
		{
			
		}
	}

	void Translate(int i, float step)
	{
		if (i == 0)
		{
			for (Point p : Controlpoints)
			{
				p.x += step;
			}
		}
		else if (i == 1)
		{
			for (Point p : Controlpoints)
			{
				p.y += step;
			}
		}
		else if (i == 2)
		{
			for (Point p : Controlpoints)
			{
				p.z += step;
			}
		}
	}

	void Resize(float step)
	{
		for (Point p : Controlpoints)
		{
			p.x *= step;
			p.y *= step;
			p.z *= step;
		}
	}

	void generateCurve()
	{
		gridPoints.reserve(pow(precision + 1, 2));

		for (int j = 0; j <= precision; ++j)
		{
			for (int i = 0; i <= precision; ++i)
			{
				gridPoints.push_back(bezierPatch(i / (float)precision, j / (float)precision));
			}
		}
	}

	Point bezierPatch(float u, float v)
	{
		Point* uCurve = new Point[n];
		Point* cP = new Point[n * m];

		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				cP[j] = Controlpoints.at(i * m + j);
			}
			uCurve[i] = bezierCurve(cP, u, m);
		}

		return bezierCurve(uCurve, v, n);
	}

	Point bezierCurve(Point* P, float t, int dim)
	{
		Point p = Point(0.f, 0.f, 0.f);

		for (int i = 0; i < dim; ++i)
		{
			p.x += BernsteinPoly(i, t, dim) * P[i].x;
			p.y += BernsteinPoly(i, t, dim) * P[i].y;
			p.z += BernsteinPoly(i, t, dim) * P[i].z;
		}

		return p;
	}

	float BernsteinPoly(int i, float t, int dim)
	{
		float binomial = Factorial(dim) / (Factorial(i) * Factorial(dim - i));

		return binomial * pow(t, i) * pow(1 - t, dim - i);
	}

	int Factorial(int x)
	{
		return (x == 1 || x == 0) ? 1 : x * Factorial(x - 1);
	}
};

#endif