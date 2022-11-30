#include "Grid.h"
#include <fstream>
#include <algorithm>
#include <map>

constexpr double eps = 0.00001;

void Grid::assembleGrid(std::string name, std::vector<Node>& nodes, std::vector<FiniteElement>& fes)
{
    input(name);

    for (size_t i = 0; i < areas.size(); i++)
    {
        areas[i].setHorizontalNodes(topInfo[i].numberOfPoints, topInfo[i].coef);
    }

    for (size_t i = 0, j = 0; i < areas.size(); i++)
    {
        if (i == 0 || i == areas.size() - 1)
        {
            areas[i].setVerticalNodes(sidesInfo[j].numberOfPoints, sidesInfo[j].coef);
            j++;
        }
        else
        {
            areas[i].setVerticalNodes(areas[i - 1].getRightNodes());
        }
    }

    areas.begin()->fixLeftArea(*(areas.begin() + 1));

    (areas.end() - 1)->fixRightArea(*(areas.end() - 2));

    for (size_t i = 0; i < areas.size(); i++)
    {
        areas[i].assembleNodes();
        areas[i].assembleFes();
    }

    assembleGlobalGrid();

    nodes = this->nodes;
    fes = this->fes;

    std::ofstream f("nodes.txt");

    for (size_t i = 0; i < areas.size(); i++)
    {
        for (size_t j = 0; j < areas[i].nodes.size(); j++)
        {
            f << areas[i].nodes[j].x << " " << areas[i].nodes[j].y << "\n";
        }
    }

    f.close();

    f.open("fes.txt");

    for (size_t i = 0; i < areas.size(); i++)
    {
        for (size_t j = 0; j < areas[i].fes.size(); j++)
        {
            f << areas[i].fes[j].leftBottomIndex() << " " << areas[i].fes[j].rightBottomIndex() << " " <<
                areas[i].fes[j].leftTopIndex() << " " << areas[i].fes[j].rightTopIndex() << "\n";
        }
    }

    f.close();
}

void Grid::input(std::string name)
{
    std::ifstream f(name);
    int nPoints, nAreas, nTopEdgesInfo, nSideEdgesInfo;
    
    f >> nPoints;
    for (size_t i = 0; i < nPoints; i++)
    {
        double x, y;
        f >> x >> y;
        points.push_back(Point(x, y));
    }

    f >> nAreas;
    for (size_t i = 0; i < nAreas; i++)
    {
        int leftbottom, lefttop, righttop, rightbottom;
        f >> leftbottom >> lefttop >> righttop >> rightbottom;
        areas.push_back(Area(points[leftbottom], points[rightbottom], points[lefttop], points[righttop]));
    }

    f >> nTopEdgesInfo;
    for (size_t i = 0; i < nTopEdgesInfo; i++)
    {
        EdgeInfo info;
        f >> info.numberOfPoints >> info.coef;
        if (info.coef < 0)
        {
            info.coef = 1 / fabs(info.coef);
        }
        topInfo.push_back(info);
    }

    f >> nSideEdgesInfo;
    for (size_t i = 0; i < nSideEdgesInfo; i++)
    {
        EdgeInfo info;
        f >> info.numberOfPoints >> info.coef;
        if (info.coef < 0)
        {
            info.coef = 1 / fabs(info.coef);
        }
        sidesInfo.push_back(info);
    }
}

void Grid::print(std::string nameNodes, std::string nameFes)
{
    std::ofstream f(nameNodes);
    
    for (size_t i = 0; i < nodes.size(); i++)
    {
        f << nodes[i].x << " " << nodes[i].y << "\n";
    }

    f.close();

    f.open(nameFes);

    for (size_t i = 0; i < fes.size(); i++)
    {
        f << fes[i].leftBottomIndex() << " " << fes[i].rightBottomIndex() << " " <<
            fes[i].leftTopIndex() << " " << fes[i].rightTopIndex() << "\n";
    }

    f.close();
}

void Grid::assembleGlobalGrid()
{
    std::vector<std::vector<Node>::iterator> nodeIts;

    std::vector<int> yNNodes;

    std::vector<std::map<int, int>> mapping(areas.size());

    for (size_t i = 0; i < areas.size(); i++)
    {
        nodeIts.push_back(areas[i].nodes.begin());
        yNNodes.push_back(areas[i].getLeftNodes().size());
    }

    auto max = std::max_element(yNNodes.begin(), yNNodes.end());


    for (size_t h = 0; h < *max; h++)
    {
        for (size_t i = 0; i < areas.size(); i++)
        {
            if (nodes.empty())
            {
                nodes.push_back(*nodeIts[i]);
                mapping[i].insert(std::make_pair(int(nodeIts[i] - areas[i].nodes.begin()), int(nodes.size() - 1)));
            }
            if (nodeIts[i] == areas[i].nodes.end())
            {
                continue;
            }
            if (!(nodes.back() == *nodeIts[i]))
            {
                nodes.push_back(*nodeIts[i]);
                mapping[i].insert(std::make_pair(int(nodeIts[i] - areas[i].nodes.begin()), int(nodes.size() - 1)));
            }
            do
            {
                if (nodeIts[i] == areas[i].nodes.end())
                {
                    break;
                }
                mapping[i].insert(std::make_pair(int(nodeIts[i] - areas[i].nodes.begin()), int(nodes.size() - 1)));
                nodeIts[i]++;
                if (nodes.back() == *nodeIts[i])
                {
                    continue;
                }
                nodes.push_back(*nodeIts[i]);
                mapping[i].insert(std::make_pair(int(nodeIts[i] - areas[i].nodes.begin()), int(nodes.size() - 1)));
            } while (fabs(nodeIts[i]->x - areas[i].rightBottom().x) > eps);
            if (nodeIts[i] != areas[i].nodes.end())
            {
                nodeIts[i]++;
            }
        }
    }

    std::vector<std::vector<FiniteElement>::iterator> fesIts;


    for (size_t i = 0; i < areas.size(); i++)
    {
        fesIts.push_back(areas[i].fes.begin());
    }

    for (size_t h = 0; h < *max; h++)
    {
        for (size_t i = 0; i < areas.size(); i++)
        {
            if (fesIts[i] != areas[i].fes.end())
            {
                FiniteElement tempfe(mapping[i][fesIts[i]->leftBottomIndex()], mapping[i][fesIts[i]->rightBottomIndex()],
                    mapping[i][fesIts[i]->leftTopIndex()], mapping[i][fesIts[i]->rightTopIndex()]);
                fes.push_back(tempfe);
                fesIts[i]++;
            }
            //if (fesIts[i] == areas[i].fes.end())
            //{
            //    continue;
            //}
            //if (!(fes.back() == *fesIts[i]))
            //{
            //    FiniteElement tempfe(mapping[i][fesIts[i]->leftBottomIndex()], mapping[i][fesIts[i]->rightBottomIndex()],
            //        mapping[i][fesIts[i]->leftTopIndex()], mapping[i][fesIts[i]->rightTopIndex()]);
            //    fes.push_back(tempfe);
            //}

            while ((fesIts[i] - areas[i].fes.begin()) % (areas[i].getBottomNodes().size() - 1))
            {
                if (std::next(fesIts[i]) == areas[i].fes.end())
                {
                    break;
                }
                //if (fes.back() == *fesIts[i])
                //{
                //    continue;
                //}
                FiniteElement tempfe(mapping[i][fesIts[i]->leftBottomIndex()], mapping[i][fesIts[i]->rightBottomIndex()],
                    mapping[i][fesIts[i]->leftTopIndex()], mapping[i][fesIts[i]->rightTopIndex()]);
                fes.push_back(tempfe);
                fesIts[i]++;
            }
            //if (fesIts[i] != areas[i].fes.end())
            //{
            //    fesIts[i]++;
            //}
        }
    }

}