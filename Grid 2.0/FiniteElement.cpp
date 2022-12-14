#include "FiniteElement.h"
#include "Geometry.h"
#include <iostream>
#include <algorithm>

constexpr double eps = 0.00001;

Point::Point() : x(0), y(0)
{

}

Point::Point(double x, double y) : x(x), y(y)
{

}

bool Point::operator==(const Point& other)
{
    return fabs(this->x - other.x) < eps && fabs(this->y - other.y) < eps;
}

Edge::Edge() : a(0, 0), b(0, 0), aIndex(0), bIndex(0)
{

}

Edge::Edge(Point a, Point b) : a(a), b(b), aIndex(0), bIndex(0)
{

}

Edge::Edge(int a, int b) : a(0, 0), b(0, 0), aIndex(a), bIndex(b)
{
    if (a > b)
    {
        std::swap(a, b);
    }
}

bool Edge::operator==(const Edge& other)
{
    return this->a == other.a && this->b == other.b || this->a == other.b && this->b == other.a;
}

void Edge::setIndices(int a, int b)
{
    aIndex = a;
    bIndex = b;

    if (aIndex > bIndex)
    {
        std::swap(aIndex, bIndex);
    }
}

int Edge::getIndexA()
{
    return aIndex;
}

int Edge::getIndexB()
{
    return bIndex;
}

bool Edge::operator<(const Edge& other) const
{
    if (this->aIndex < other.aIndex)
    {
        return true;
    }
    if (this->aIndex > other.aIndex)
    {
        return false;
    }

    if (this->bIndex < other.bIndex)
    {
        return true;
    }

    return false;
}

FiniteElement::FiniteElement() : points(4), indices(4)
{

}

FiniteElement::FiniteElement(Node leftbottom, Node rightbottom, Node lefttop, Node righttop) : points(4), indices(4)
{
    points[0] = leftbottom;
    points[1] = rightbottom;
    points[2] = lefttop;
    points[3] = righttop;
}

FiniteElement::FiniteElement(int leftbottom, int rightbottom, int lefttop, int righttop) : points(4), indices(4)
{
    indices[0] = leftbottom;
    indices[1] = rightbottom;
    indices[2] = lefttop;
    indices[3] = righttop;
}

bool FiniteElement::operator==(const FiniteElement& other)
{
    return indices == other.indices;
}

std::vector<int> FiniteElement::getIndices()
{
    return indices;
}

Node FiniteElement::leftBottom()
{
    return points[Points::leftbottom];
}

Node FiniteElement::rightBottom()
{
    return points[Points::rightbottom];
}

Node FiniteElement::leftTop()
{
    return points[Points::lefttop];
}

Node FiniteElement::rightTop()
{
    return points[Points::righttop];
}



int FiniteElement::leftBottomIndex()
{
    return indices[Points::leftbottom];
}

int FiniteElement::rightBottomIndex()
{
    return indices[Points::rightbottom];
}

int FiniteElement::leftTopIndex()
{
    return indices[Points::lefttop];
}

int FiniteElement::rightTopIndex()
{
    return indices[Points::righttop];
}








Area::Area() : cornerNodes(4)
{

}

Area::Area(Node leftbottom, Node rightbottom, Node lefttop, Node righttop) : cornerNodes(4)
{
    cornerNodes[0] = leftbottom;
    cornerNodes[1] = rightbottom;
    cornerNodes[2] = lefttop;
    cornerNodes[3] = righttop;
}

void Area::setHorizontalNodes(int n, double coef)
{
    double length = pow(rightTop().y - leftTop().y, 2) + pow(rightTop().x - leftTop().x, 2);
    length = sqrt(length);

    double h;
    if (fabs(fabs(coef) - 1) < eps)
    {
        h = length / n;
    }
    else
    {
        h = length * ((1 - coef) / (1 - pow(coef, n)));
    }


    topNodes.push_back(leftTop());
    double newx = leftTop().x, newy = leftTop().y;
    for (size_t i = 0; i < n; i++)
    {
        newx += (rightTop().x - leftTop().x) / length * h;
        newy += (rightTop().y - leftTop().y) / length * h;
        h = h * coef;
        topNodes.push_back(Node(newx, newy));
    }


    for (size_t i = 0; i < topNodes.size(); i++)
    {
        Node retNode;
        Node imgNode(topNodes[i].x, std::min(leftBottom().y, rightBottom().y));
        
        if (Geometry::findIntersection(topNodes[i], imgNode, leftBottom(), rightBottom(), retNode))
        {
            bottomNodes.push_back(retNode);
        }
        else
        {
            std::cerr << "can't find projection on bottom\n";
            return;
        }

    }
}

void Area::setVerticalNodes(int n, double coef)
{
    double length = pow(leftTop().y - leftBottom().y, 2) + pow(leftTop().x - leftBottom().x, 2);
    length = sqrt(length);

    double h;
    if (fabs(fabs(coef) - 1) < eps)
    {
        h = length / n;
    }
    else
    {
        h = length * ((1 - coef) / (1 - pow(coef, n)));
    }


    leftNodes.push_back(leftBottom());
    double newx = leftBottom().x, newy = leftBottom().y;
    for (size_t i = 0; i < n; i++)
    {
        newx += (leftTop().x - leftBottom().x) / length * h;
        newy += (leftTop().y - leftBottom().y) / length * h;
        h = h * coef;
        leftNodes.push_back(Node(newx, newy));
    }


    length = pow(rightTop().y - rightBottom().y, 2) + pow(rightTop().x - rightBottom().x, 2);
    length = sqrt(length);

    h;
    if (fabs(fabs(coef) - 1) < eps)
    {
        h = length / n;
    }
    else
    {
        h = length * ((1 - coef) / (1 - pow(coef, n)));
    }


    rightNodes.push_back(rightBottom());
    newx = rightBottom().x;
    newy = rightBottom().y;
    for (size_t i = 0; i < n; i++)
    {
        newx += (rightTop().x - rightBottom().x) / length * h;
        newy += (rightTop().y - rightBottom().y) / length * h;
        h = h * coef;
        rightNodes.push_back(Node(newx, newy));
    }
}

void Area::setVerticalNodes(const std::vector<Node>& rightNodesFromLeftArea)
{
    std::vector<Node> ourLeftNodes;

    for (size_t i = 0; i < rightNodesFromLeftArea.size(); i++)
    {
        if (rightNodesFromLeftArea[i].y > leftBottom().y - eps && rightNodesFromLeftArea[i].y < leftTop().y + eps)
        {
            ourLeftNodes.push_back(rightNodesFromLeftArea[i]);
        }
    }

    if (!(fabs(ourLeftNodes.back().x - leftTop().x) < eps && fabs(ourLeftNodes.back().y - leftTop().y) < eps))
    {
        ourLeftNodes.back() = leftTop();
    }

    leftNodes = ourLeftNodes;

    //double length = pow(rightTop().y - rightBottom().y, 2) + pow(rightTop().x - rightBottom().x, 2);
    //length = sqrt(length);
    //int n = leftNodes.size() - 1;
    //double h = length / n;

    //rightNodes.push_back(rightBottom());
    //double newx = rightBottom().x;
    //double newy = rightBottom().y;
    //for (size_t i = 0; i < n; i++)
    //{
    //    newx += (rightTop().x - rightBottom().x) / length * h;
    //    newy += (rightTop().y - rightBottom().y) / length * h;
    //    rightNodes.push_back(Node(newx, newy));
    //}

    for (size_t i = 0; i < leftNodes.size(); i++)
    {
        Node retNode;
        Node imgNode(std::max(leftBottom().x, rightBottom().x), leftNodes[i].y);

        if (Geometry::findIntersection(leftNodes[i], imgNode, rightTop(), rightBottom(), retNode))
        {
            rightNodes.push_back(retNode);
        }
        else
        {
            std::cerr << "can't find projection on right\n";
            return;
        }

    }

}

void Area::fixLeftArea(Area& right)
{
    Node goodNode = right.leftTop();

    std::vector<double> distances(rightNodes.size());

    for (size_t i = 0; i < rightNodes.size(); i++)
    {
        distances[i] = pow(goodNode.x - rightNodes[i].x, 2) + pow(goodNode.y - rightNodes[i].y, 2);
        distances[i] = sqrt(distances[i]);
    }

    auto it = std::min_element(distances.begin(), distances.end());
    rightNodes[it - distances.begin()] = goodNode;
}

void Area::fixRightArea(Area& left)
{
    int nExtraNodes = rightNodes.size() - left.getRightNodes().size();

    for (size_t i = 0; i < left.getRightNodes().size(); i++)
    {
        leftNodes[i] = left.getRightNodes()[i];
    }

    Node upper(leftTop());
    Node lower(left.rightTop());

    double length = pow(upper.y - lower.y, 2) + pow(upper.x - lower.x, 2);
    length = sqrt(length);
    int n = nExtraNodes;
    double h = length / n;

    double newx = lower.x;
    double newy = lower.y;
    for (size_t i = 0; i < n; i++)
    {
        newx += (upper.x - lower.x) / length * h;
        newy += (upper.y - lower.y) / length * h;
        leftNodes[left.getRightNodes().size() + i] = Node(newx, newy);
    }
}

void Area::assembleNodes()
{
    for (size_t i = 0; i < leftNodes.size(); i++)
    {
        for (size_t j = 0; j < bottomNodes.size(); j++)
        {
            Node node;
            if (Geometry::findIntersection(leftNodes[i], rightNodes[i], bottomNodes[j], topNodes[j], node))
            {
                nodes.push_back(node);
            }
            else
            {
                std::cerr << "can't find intersection.\n";
                return;
            }
        }
    }
}

void Area::assembleFes()
{
    int nHorizontalPoints = bottomNodes.size();
    int nVerticalPoints = leftNodes.size();
    int nFesx = nHorizontalPoints - 1;
    int nFesy = nVerticalPoints - 1;
    int nFes = nFesx * nFesy;
    for (size_t i = 0; i < nFes; i++)
    {
        int firstNode = i + i / nFesx;
        int secondNode = i + i / nFesx + 1;
        int thirdNode = i + nFesx + i / nFesx + 1;
        int forthNode = i + nFesx + i / nFesx + 1 + 1;

        fes.push_back(FiniteElement(firstNode, secondNode, thirdNode, forthNode));
    }
}

Node Area::leftBottom()
{
    return cornerNodes[Points::leftbottom];
}

Node Area::rightBottom()
{
    return cornerNodes[Points::rightbottom];
}

Node Area::leftTop()
{
    return cornerNodes[Points::lefttop];
}

Node Area::rightTop()
{
    return cornerNodes[Points::righttop];
}

std::vector<Node> Area::getTopNodes()
{
    return topNodes;
}

std::vector<Node> Area::getBottomNodes()
{
    return bottomNodes;
}

std::vector<Node> Area::getLeftNodes()
{
    return leftNodes;
}

std::vector<Node> Area::getRightNodes()
{
    return rightNodes;
}

