#include <iostream>
#include "Graph.h"
#include "GraphCom.h"
using namespace std;
int main() {

    GraphCom gc;
    freopen("a.txt","r",stdin);
    int u,v,x;
    while(cin >>u>>v>>x) {
        gc.addEdge(u, v, 0);
    }

    gc.initialSuperNode();
    gc.processBatch(1,1,20);
    return 0;
}
