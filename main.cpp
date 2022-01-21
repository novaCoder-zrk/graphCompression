#include <iostream>
#include "Graph.h"
#include "GraphCom.h"
int main() {
    GraphCom gc;
    gc.addEdge(1,2,0);
    gc.addEdge(2,3,0);


    gc.initialSuperNode();
    gc.dividing(6,2);
    return 0;
}
