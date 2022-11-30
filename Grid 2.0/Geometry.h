#pragma once
#include "FiniteElement.h"
class Geometry
{
public:
    static bool isBetween(double x1, double x2, double x3, double y1, double y2, double y3);
    static bool findIntersection(Point a1, Point a2, Point b1, Point b2, Point& ret);

};

