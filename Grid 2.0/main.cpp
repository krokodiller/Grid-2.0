#include <vector>
#include "Grid.h"
#include "FiniteElement.h"
#include "EdgeNumeration.h"
#include <fstream>

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
    edge.numerateEdges(nodes, fes, edges);
    edge.printEdges("edges.txt");

    std::vector<int> temp = edge.EdgesByFE(5);

    return 0;
}

