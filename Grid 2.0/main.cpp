#include <vector>
#include "Grid.h"
#include "FiniteElement.h"
#include "EdgeNumeration.h"
#include <fstream>
#include <iostream>

using Node = Point;
int main()
{
    std::vector<Node> nodes;
    std::vector<FiniteElement> fes;

    Grid grid;
    grid.assembleGrid("grid.txt", nodes, fes);
    grid.print("nodes.txt", "fes.txt");

    std::vector<Edge> edges;

    EdgeNumeration edge;
    edge.numerateEdges(nodes, fes);
    edge.printEdges("edges.txt");

    int n = 30;
    std::vector<int> edgeFes = edge.getConnectedFes(n);
    std::vector<int> edgePoints = edge.getPointsIndices(n);

    std::cout << "(" << n << ")fes: ";
    for (size_t i = 0; i < edgeFes.size(); i++)
    {
        std::cout << edgeFes[i] << " ";
    }
    std::cout << "\n";

    std::cout << "(" << n << ")points: ";
    for (size_t i = 0; i < edgePoints.size(); i++)
    {
        std::cout << edgePoints[i] << " ";
    }
    std::cout << "\n";
    int a = 31, b = 44;
    int e = edge.getEdgeNumber(a, b);
    std::cout << "(" << a << ", " << b << ")edge: " << e << "\n";

    return 0;
}

