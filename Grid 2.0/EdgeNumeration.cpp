#include "EdgeNumeration.h"
#include "Grid.h"
#include <set>
#include <algorithm>
#include <fstream>
#include <iostream>

void EdgeNumeration::numerateEdges(std::vector<Node>& nodes, std::vector<FiniteElement>& fes)
{
    std::map<Edge, std::vector<int>> locEdges;
    for (int i = 0; i < fes.size(); i++)
    {
        Edge e(fes[i].leftBottomIndex(), fes[i].rightBottomIndex());
        auto it = locEdges.find(e);
        if (it != locEdges.end())
        {
            it->second.push_back(i);
        }
        else
        {
            locEdges.emplace(e, std::initializer_list<int>{i});
        }

        e.setIndices(fes[i].rightBottomIndex(), fes[i].rightTopIndex());
        it = locEdges.find(e);
        if (it != locEdges.end())
        {
            it->second.push_back(i);
        }
        else
        {
            locEdges.emplace(e, std::initializer_list<int>{i});
        }

        e.setIndices(fes[i].rightTopIndex(), fes[i].leftTopIndex());
        it = locEdges.find(e);
        if (it != locEdges.end())
        {
            it->second.push_back(i);
        }
        else
        {
            locEdges.emplace(e, std::initializer_list<int>{i});
        }

        e.setIndices(fes[i].leftTopIndex(), fes[i].leftBottomIndex());
        it = locEdges.find(e);
        if (it != locEdges.end())
        {
            it->second.push_back(i);
        }
        else
        {
            locEdges.emplace(e, std::initializer_list<int>{i});
        }
    }

    for (auto it = locEdges.begin(); it != locEdges.end(); it++)
    {
        edges.emplace_back(it->first, it->second);
    }
}

void EdgeNumeration::printEdges(std::string name)
{
    std::ofstream f(name);
    for (size_t i = 0; i < edges.size(); i++)
    {
        f << edges[i].edge.getIndexA() << " " << edges[i].edge.getIndexB() << "\n";
    }
    f.close();
}

std::vector<int> EdgeNumeration::getPointsIndices(int number)
{
    if (number >= edges.size())
    {
        std::cerr << "incorrect index.\n";
        return std::vector<int>();
    }
    std::vector<int> ret = { edges[number].edge.getIndexA(), edges[number].edge.getIndexB() };
    return ret;
}

std::vector<int> EdgeNumeration::getConnectedFes(int number)
{
    if (number >= edges.size())
    {
        std::cerr << "incorrect index.\n";
        return std::vector<int>();
    }
    return edges[number].feIndices;
}

int EdgeNumeration::getEdgeNumber(int indexA, int indexB)
{
    auto find = std::find_if(edges.begin(), edges.end(), [indexA, indexB](EdgeInformation e) {
        return e.edge.getIndexA() == indexA && e.edge.getIndexB() == indexB;
        });
    if (find != edges.end())
    {
        return (find - edges.begin());
    }
    else
    {
        std::cerr << "no edge with these indices.\n";
        return -1;
    }

}

