#include <iostream>
#include "Graph.h"
#include "GraphCom.h"
using namespace std;
int main() {

    GraphCom gc;
    freopen("a.txt","r",stdin);
    int N = 0;
    cin >> N;
    for (int i = 0; i < 16; i++) {
        int u,v;
        cin>>u>>v;
        gc.addEdge(u, v, 0);
    }

    gc.initialSuperNode();
    gc.processBatch(6,2,3);
    return 0;
}
