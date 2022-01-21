//
// Created by zhang on 2022/1/19.
//

#ifndef GRAPHCOMPRESSION_GRAPHCOM_H
#define GRAPHCOMPRESSION_GRAPHCOM_H
#include "Graph.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <functional>
#include <string>
#include <unordered_map>
class GraphCom {
public:
    GraphCom(){
        T =1;
    }
    void initialGraph(Graph inputG);//初始化图结构

    void addEdge(int src,int dst,bool ordered);//向图中加边

    void initialSuperNode();//初始化supernode
    int find(int u);//找到u所在的supernode
    bool unite(int u,int v);//等价类合并 以u为标志
    void mergeSuperNode(int su,int sv);//merge supernode 以su为标志

    void dividing(int m,int r);//将SuperNodes 利用LSH 分配到不同的buckets中

    void merge(int iter);
    double save(int sa,int sb); //计算A B合并后的saving
    unordered_map<int,int> getSuperDeg(int sv);//得到与sv的所有邻接点的
    long getCost(int sv);//计算supernode sv的cost
    long getMergeCost(int sa,int sb);
    void processBatch(int m,int r,int t1);//进行迭代处理
    void encode();
private:
    int T;//the number of iterations
    Graph g;//存储整张图
    vector<int> node2Super; //记录每一个顶点所属的超点
    map<int,set<int>> supernodes;
    unordered_map<size_t,set<int>> buckets;//通过LSH 将超点分到桶中
    map<int,int>deg;//每个supernode的度
    Graph cm;
    Graph cp;
    Graph P;
};


#endif //GRAPHCOMPRESSION_GRAPHCOM_H
