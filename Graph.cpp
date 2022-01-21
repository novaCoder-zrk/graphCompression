//
// Created by zhang on 2022/1/19.
//

#include "Graph.h"
void Graph::addOrderedEdge(int src, int dst){
    vertex.insert(src);
    vertex.insert(dst);
    int before = adjList[src].size();
    adjList[src].insert(dst);
    if(adjList[src].size() > before)
        cntEdge++;

}
void Graph::addUnorderedEdge(int src, int dst){
    vertex.insert(src);
    vertex.insert(dst);
    int before = adjList[src].size();
    adjList[src].insert(dst);
    adjList[dst].insert(src);

    if(adjList[src].size() > before)
        cntEdge += 2;
}
Graph::Graph() {
    cntEdge = 0;
}
set<int> Graph:: getNeighbors(int u){
    return adjList[u];
}
bool Graph::isAdj(int src,int dst){
    return  adjList[src].find(dst) != adjList[src].end();
}

int Graph::maxVertex() {
    auto it  = vertex.end();
    it--;
    return *it;
}