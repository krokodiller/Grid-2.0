#include "EdgeNumeration.h"
#include "Grid.h"
#include <set>
#include <algorithm>
#include <fstream>

void EdgeNumeration::numerateEdges(std::vector<Node>& nodes, std::vector<FiniteElement>& fes, std::vector<Edge> &edges)
{
    std::set<Edge> setEdges;
    for (size_t i = 0; i < fes.size(); i++)
    {
        Edge edge(fes[i].leftBottomIndex(), fes[i].rightBottomIndex());
        if (!setEdges.count(edge))
        {
            edge.addFENumbers(i);
            setEdges.insert(edge);
        }
        else
        {
            edge.addFENumbers(i);
        }

        edge.setIndices(fes[i].rightBottomIndex(), fes[i].rightTopIndex());
        if (!setEdges.count(edge))
        {
            edge.addFENumbers(i);
            setEdges.insert(edge);
        }
        else
        {
            edge.addFENumbers(i);
        }

        edge.setIndices(fes[i].rightTopIndex(), fes[i].leftTopIndex());
        if (!setEdges.count(edge))
        {
            edge.addFENumbers(i);
            setEdges.insert(edge);
        }
        else
        {
            edge.addFENumbers(i);
        }

        edge.setIndices(fes[i].leftTopIndex(), fes[i].leftBottomIndex());
        if (!setEdges.count(edge))
        {
            edge.addFENumbers(i);
            setEdges.insert(edge);
        }
        else
        {
            edge.addFENumbers(i);
        }
    }
    
    this->edges.resize(setEdges.size());
    std::copy(setEdges.begin(), setEdges.end(), this->edges.begin());

    edges = this->edges;
}

std::vector<int> EdgeNumeration::EdgesByFE(int FE)
{
    std::vector<int> indices;
    for (size_t i = 0; i < edges.size(); i++)
    {
        for (size_t j = 0; j < edges[i].getFENumbers().size(); j++)
        {
            if (edges[i].getFENumbers()[j] == FE)
            {
                indices.push_back(i);
            }
        }
    }
    return indices;
}


void EdgeNumeration::printEdges(std::string name)
{
    std::ofstream f(name);
    for (auto it = edges.begin(); it != edges.end(); it++)
    {
        f << it->getIndexA() << " " << it->getIndexB() << "\n";
    }
    f.close();
}