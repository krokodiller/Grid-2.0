#pragma once
#include <vector>

class Point
{
public:
    Point();
    Point(double x, double y);
    bool operator==(const Point &other);

    double x, y;
};

class Edge
{
public:
    Edge();
    Edge(Point a, Point b);
    Edge(int a, int b);
    bool operator==(const Edge& other);
    bool operator<(const Edge& other) const;

    void setIndices(int a, int b);

    int getIndexA();
    int getIndexB();


private:
    int aIndex, bIndex;
    Point a, b;
};

using Node = Point;
class FiniteElement
{
public:
    FiniteElement();
    FiniteElement(Node leftbottom, Node rightbottom, Node lefttop, Node righttop);
    FiniteElement(int leftbottom, int rightbottom, int lefttop, int righttop);
    bool operator==(const FiniteElement& other);

    std::vector<int> getIndices();

    Node leftBottom();
    Node rightBottom();
    Node leftTop();
    Node rightTop();

    int leftBottomIndex();
    int rightBottomIndex();
    int leftTopIndex();
    int rightTopIndex();

private:
    std::vector<Node> points;
    std::vector<int> indices;
};

using Node = Point;
class Area
{
public:
    Area();
    Area(Node leftbottom, Node rightbottom, Node lefttop, Node righttop);
    Node leftBottom();
    Node rightBottom();
    Node leftTop();
    Node rightTop();

    std::vector<Node> getTopNodes();
    std::vector<Node> getBottomNodes();
    std::vector<Node> getLeftNodes();
    std::vector<Node> getRightNodes();

    void setHorizontalNodes(int n, double coef);
    // for left most and right most area
    void setVerticalNodes(int n, double coef);
    // for anything else
    void setVerticalNodes(const std::vector<Node>& rightNodesFromLeftArea);

    void fixLeftArea(Area& right);

    void fixRightArea(Area& left);

    void assembleNodes();

    void assembleFes();

    std::vector<Node> nodes;
    std::vector<FiniteElement> fes;

private:
    std::vector<Node> cornerNodes;
    std::vector<Node> topNodes;
    std::vector<Node> bottomNodes;
    std::vector<Node> leftNodes;
    std::vector<Node> rightNodes;
    
    
};


enum Points
{
    leftbottom,
    rightbottom,
    lefttop,
    righttop
};