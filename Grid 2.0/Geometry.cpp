#include "Geometry.h"

constexpr double eps = 0.00001;

bool Geometry::isBetween(double x1, double x2, double x3, double y1, double y2, double y3)
{
    double crossproduct = (y3 - y1) * (x2 - x1) - (x3 - x1) * (y2 - y1);

    if (fabs(crossproduct) > eps)
    {
        return false;
    }

    double dotproduct = (x3 - x1) * (x2 - x1) + (y3 - y1) * (y2 - y1);
    if (dotproduct < 0)
    {
        return false;
    }

    double squaredLength = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
    if (dotproduct > squaredLength)
    {
        return false;
    }

    return true;
}

bool Geometry::findIntersection(Point a1, Point a2, Point b1, Point b2, Point& ret)
{
    double& x1 = a1.x,
        x2 = a2.x,
        x3 = b1.x,
        x4 = b2.x,
        y1 = a1.y,
        y2 = a2.y,
        y3 = b1.y,
        y4 = b2.y;
    double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) /
        ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    double u = ((x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2)) /
        ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    if (fabs(x1 - x2) < eps && fabs(y1 - y2) < eps)
    {
        if (isBetween(x3, x4, x1, y3, y4, y1))
        {
            ret.x = x1;
            ret.y = y1;
            return true;
        }

    }

    if (fabs(x3 - x4) < eps && fabs(y3 - y4) < eps)
    {
        if (isBetween(x1, x2, x3, y1, y2, y3))
        {
            ret.x = x3;
            ret.y = y3;
            return true;
        }
        return false;
    }

    if (t + eps >= 0 && t - eps <= 1 && u + eps >= 0 && u - eps <= 1)
    {
        double px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) /
            ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

        double py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) /
            ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

        ret.x = px;
        ret.y = py;
        return true;
    }
    else
    {
        return false;
    }


}

