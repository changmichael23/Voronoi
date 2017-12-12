#include "Point2D.h"

using namespace std;

Point2D::Point2D(float x, float y) : x(x), y(y) {}
Point2D::Point2D() : x(0), y(0) {}

float Point2D::distance(Point2D point)
{
	float res;
	res = sqrt((this->x - point.x)*(this->x - point.x) + (this->y - point.y)*(this->y - point.y));
	return res;
}

Point2D Point2D::operator+(const Point2D & point)
{
	Point2D ret;
	this->x += point.x;
	this->y += point.y;
	ret = Point2D(x, y);
	return ret;
}

Point2D Point2D::operator-(const Point2D & point)
{
	Point2D ret;
	this->x -= point.x;
	this->y -= point.y;
	ret = Point2D(x, y);
	return ret;
}

std::ostream& operator<<(std::ostream& os, const Point2D &point)
{
	return os << "X : " << point.x << "Y : " << point.y;
}

bool Point2D::operator<=(const Point2D& point) const
{
	if (x <= point.x || (x == point.x && y <= point.y))
		return true;
	return false;
}

bool Point2D::operator<(const Point2D& point) const
{
	if (x < point.x || (x == point.x && y < point.y))
		return true;
	return false;
}

bool Point2D::operator==(const Point2D & point) const
{
	return (x == point.x || y == point.y);
}

bool Point2D::operator!=(const Point2D & point) const
{
	return (x != point.x || y != point.y);
}
