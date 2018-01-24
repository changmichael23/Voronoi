#pragma once
#include <vector>
#include "Point.hpp"

#ifndef PATCH_HPP
#define PATCH_HPP

extern int precision;
extern RotateMode rm;

float COS(float angle)
{
	return cos(angle * 2 * 0.00872664625);
}
float SIN(float angle)
{
	return sin(angle * 2 * 0.00872664625);
}

struct Patch
{
	int n, m;
	std::vector<Point> controlPoints;
	std::vector<Point> gridPoints;
	std::vector<Point> subdivisionPoints;

	Patch()
	{
		n = 0;
		m = 0;
		
		gridPoints.reserve(pow(precision + 1, 2));
	}

	Patch(int _n, int _m)
	{
		n = _n;
		m = _m;

		for (int i = 0; i <= n; ++i)
		{
			for (int j = 0; j <= m; ++j)
			{
				Point p = Point(i, .0f, j);
				controlPoints.push_back(p);
			}
		}

		gridPoints.reserve(pow(precision + 1, 2));
	}

	void MovePoint(int i, float step)
	{
		controlPoints.at(i).y += step;
	}

	void Rotate(int a, float step)
	{
		float x = 0.f, y = 0.f, z = 0.f;

		if (rm == quaternion)
		{
			if (a == 0)
			{

			}
			else if (a == 1)
			{

			}
			else if (a == 2)
			{

			}
		}
		else if (rm == matrix)
		{
			if (a == 0)
			{
				for (Point &p : gridPoints)
				{
					x = p.x;
					y = p.y;
					z = p.z;
					
					p.y = y * COS(step) - z * SIN(step);
					p.z = y * SIN(step) + z * COS(step);
				}
			}
			else if (a == 1)
			{
				for (Point &p : gridPoints)
				{
					x = p.x;
					y = p.y;
					z = p.z;

					p.x = x * COS(step) + z * SIN(step);
					p.z = -x * SIN(step) + z * COS(step);
				}
			}
			else if (a == 2)
			{
				for (Point &p : gridPoints)
				{
					x = p.x;
					y = p.y;
					z = p.z;

					p.x = x * COS(step) - y * SIN(step);
					p.y = x * SIN(step) + y * COS(step);
				}
			}
		}
	}

	void Translate(int i, float step)
	{
		if (i == 0)
		{
			for (Point &p : gridPoints)
			{
				p.x += step;
			}
		}
		else if (i == 1)
		{
			for (Point &p : gridPoints)
			{
				p.y += step;
			}
		}
		else if (i == 2)
		{
			for (Point &p : gridPoints)
			{
				p.z += step;
			}
		}
	}

	void Resize(float step)
	{
		for (Point &p : gridPoints)
		{
			p.x *= step;
			p.y *= step;
			p.z *= step;
		}
	}

	void GenerateCurve()
	{
		gridPoints.clear();

		for (int j = 0; j <= precision; ++j)
		{
			for (int i = 0; i <= precision; ++i)
			{
				gridPoints.push_back(BezierPatch(i / (float)precision, j / (float)precision));
				CalculateNormale(i / (float)precision, j / (float)precision);
			}
		}
	}

	Point BezierPatch(float u, float v)
	{
		Point Suv = Point(), Si = Point();

		for (int i = 0; i <= n; ++i)
		{
			Si = .0f;
			for (int j = 0; j <= m; ++j)
			{
				Si +=  controlPoints[i * (m + 1) + j] * BernsteinPoly(j, v, m);
			}
			Suv += Si * BernsteinPoly(i, u, n);
		}

		return Suv;
	}

	float BernsteinPoly(int i, float t, int dim)
	{
		return Binomial(i, dim) * pow(t, i) * pow(1 - t, dim - i);
	}

	int Binomial(int x, int n)
	{
		return Factorial(n) / (Factorial(x) * Factorial(n - x));
	}

	int Factorial(int x)
	{
		return (x == 1 || x == 0) ? 1 : x * Factorial(x - 1);
	}

	void CalculateNormale(float u, float v)
	{
		Point Du = Point(), Dv = Point(), Si = Point();

		for (int i = 0; i <= n; ++i)
		{
			Si = .0f;
			for (int j = 0; j <= m; ++j)
			{
				Si += controlPoints[i * (m + 1) + j] * BernsteinPoly(j, v, m);
			}
			Du += Si * DerivateBernsteinPoly(i, u, n);
		}

		for (int i = 0; i <= n; ++i)
		{
			Si = .0f;
			for (int j = 0; j <= m; ++j)
			{
				Si += controlPoints[i * (m + 1) + j] * DerivateBernsteinPoly(j, v, m);
			}
			Dv += Si * BernsteinPoly(i, u, n);
		}

		Point cp = CrossProduct(Du, Dv);

		gridPoints.back().n1 = cp.x;
		gridPoints.back().n2 = cp.y;
		gridPoints.back().n3 = cp.z;
	}

	float DerivateBernsteinPoly(int i, float t, int dim)
	{
		if (i == 0)
		{
			return - dim * pow(1 - t, dim - 1);
		}
		else if (i == dim)
		{
			return dim * pow(t, dim - 1);
		}
		else
		{
			return i * Binomial(i, dim) * pow(t, i - 1) * pow(1 - t, dim - i)
				- (dim - i) * Binomial(i, dim) * pow(t, i) * pow(1 - t, dim - i - 1);
		}
	}

	Point CrossProduct(Point p1, Point p2)
	{
		Point cp = Point();

		cp.x = p1.y * p2.z - p1.z * p2.y;
		cp.y = p1.z * p2.x - p1.x * p2.z;
		cp.z = p1.x * p2.y - p1.y * p2.x;

		return cp;
	}

	void Subdivise()
	{
		int sm;
		subdivisionPoints.reserve(4 * precision * precision);

		std::vector<Point> centrePoints;
		std::vector<Point> edgePoints;

		std::cout << "Saisir la \"smoothness\" de la subdivision (standard = 4) : ";
		std::cin >> sm;
		
		for (int i = 0; i < precision; ++i)
		{
			for (int j = 0; j < precision; ++j)
			{
				Point centre = Point();
				centre.x = (controlPoints.at(i * m + j).x + controlPoints.at((i + 1)  * m + j).x) / 2;
				centre.x = (controlPoints.at(i * m + j).x + controlPoints.at((i + 1)  * m + j).x) / 2;	
			}
		}
	}
};

#endif