#pragma once
#include <algorithm>
#include <iterator>
#include "Point2D.h"
#include <vector>
#include "glm.hpp"

template <class TPoint>
glm::vec2 makeVector(TPoint p1, TPoint p2);

template <class TPoint>
int normVector(TPoint a, TPoint b);
int normVector(glm::vec2 vector);

template <class TPoint>
double angleVec(glm::vec2 vector, TPoint p1, TPoint p2);

template<class TPoint>
double fullAngle(glm::vec2 vector, TPoint p1, TPoint p2);

std::vector<Point2D> jarvisMarch(std::vector<Point2D> vecpoints);

Point2D barycentre(std::vector<Point2D> vecpoints);

bool isConvexPoint(Point2D p, Point2D prevPoint, Point2D nextPoint);

std::vector<Point2D> findAndSuppressConcavePoints(std::vector<Point2D> vecpoints);

std::vector<Point2D> grahamScan(std::vector<Point2D> vecpoints);
