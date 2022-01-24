//
// Created by zhang on 2022/1/19.
//

#ifndef GRAPHCOMPRESSION_GRAPH_H
#define GRAPHCOMPRESSION_GRAPH_H
#include <map>
#include <set>

using namespace std;
class Graph {
public:
    Graph();
    void addOrderedEdge(int src,int dst);
    void addUnorderedEdge(int src,int dst);
    set<int> getNeighbors(int u);
    bool isAdj(int src,int dst);
    int maxVertex();
    int getEdgeNum();
private:
    map<int,set<int>> adjList;
    set<int> vertex;
    long cntEdge;
};


#endif //GRAPHCOMPRESSION_GRAPH_H
