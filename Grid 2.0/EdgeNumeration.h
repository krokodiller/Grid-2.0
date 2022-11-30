#pragma once
#include "FiniteElement.h"
#include "Grid.h"
#include <map>
class EdgeNumeration
{
public:
    void numerateEdges(std::vector<Node>& nodes, std::vector<FiniteElement>& fes, std::vector<Edge>& edges);

    void printEdges(std::string name);

    std::vector<int> EdgesByFE(int FE);
private:
    std::vector<Edge> edges;
};

