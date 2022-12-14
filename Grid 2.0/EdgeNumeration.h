#pragma once
#include "FiniteElement.h"
#include "Grid.h"
#include <map>

struct EdgeInformation
{
    Edge edge;
    std::vector<int> feIndices;
    EdgeInformation(Edge e, std::vector<int> fes) { edge = e; feIndices = fes; }
    bool operator<(const EdgeInformation& other) { return this->edge < other.edge; }
};

class EdgeNumeration
{
public:
    void numerateEdges(std::vector<Node>& nodes, std::vector<FiniteElement>& fes);

    void printEdges(std::string name);

    std::vector<int> getPointsIndices(int number);

    std::vector<int> getConnectedFes(int number);

    int getEdgeNumber(int indexA, int indexB);

private:
    std::vector<EdgeInformation> edges;
};

