//
// Created by zhang on 2022/1/19.
//

#include "Graph.h"
void Graph::addOrderedEdge(int src, int dst){
    vertex.insert(src);
    vertex.insert(dst);
    adjList[src].insert(dst);

}
void Graph::addUnorderedEdge(int src, int dst){
    vertex.insert(src);
    vertex.insert(dst);
    adjList[src].insert(dst);
    adjList[dst].insert(src);
}
Graph::Graph() {

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