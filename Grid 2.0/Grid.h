#pragma once
#include <string>
#include <vector>
#include "FiniteElement.h"

struct EdgeInfo
{
    int numberOfPoints = 0;
    double coef = 1;
};

using Node = Point;
class Grid
{
public:
    void assembleGrid(std::string name, std::vector<Node>& nodes, std::vector<FiniteElement>& fes);

    void print(std::string nameNodes, std::string nameFes);
private:
    void input(std::string name);

    void assembleGlobalGrid();



    std::vector<Point> points;
    std::vector<Area> areas;
    std::vector<EdgeInfo> topInfo;
    std::vector<EdgeInfo> sidesInfo;

    std::vector<Node> nodes;
    std::vector<FiniteElement> fes;
};

