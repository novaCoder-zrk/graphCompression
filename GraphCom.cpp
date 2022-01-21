//
// Created by zhang on 2022/1/19.
//

#include "GraphCom.h"
void GraphCom::initialGraph(Graph inputG) {
    g = inputG;
}

void GraphCom::addEdge(int src, int dst, bool ordered) {
    if(ordered){
        if(deg.count(src) == 0)
            deg[src] = 1;
        else deg[src]++;
        g.addOrderedEdge(src,dst);
    }
    else {
        g.addUnorderedEdge(src, dst);
        if(deg.count(src) == 0)
            deg[src] = 1;
        else deg[src]++;
        if(deg.count(dst) == 0)
            deg[dst] = 1;
        else deg[dst]++;
    }
}
void GraphCom::initialSuperNode() {
    int max = g.maxVertex();
    node2Super = vector<int>(max+1);
    node2Super[0] = 0;
    for(int i = 1;i < node2Super.size();i++){
        node2Super[i] = i;
        supernodes[i].insert(i);
    }

}
int GraphCom::find(int u){
    if(node2Super[u] == u)
        return u;
    return node2Super[u] = find(node2Super[u]);
}

bool GraphCom::unite(int u,int v){
     u = find(u);
     v = find(v);
    if(u == v) return false;
    node2Super[v] = u;
    return true;
}
void GraphCom::mergeSuperNode(int su, int sv) {
   supernodes[su].insert(supernodes[sv].begin(),supernodes[sv].end());
   supernodes.erase(sv);
}
void GraphCom::dividing(int m, int r) {
    buckets.clear();
    int max = g.maxVertex();
    int pi[m][max];
    //生成m个随机全排列，每个全排列是从 1-max的
    for(int k = 0;k < m;k++){
        pi[k][0] = 1;
        for(int i = 1;i < max;i++){
            pi[k][i] = i+1;
            int random = rand()%(i+1);
            pi[k][i] = pi[k][random];
            pi[k][random] = i+1;
        }
    }

    //计算每个superNode的signature
    for(auto sn:supernodes){
        vector<int> shingle(m,1e9);
        for(int i = 0;i < m;i++){
            //计算每一个排列的 min
            for(auto v:sn.second){
                shingle[i] = min(shingle[i],pi[i][v-1]);
                for (int u:g.getNeighbors(v))
                    shingle[i] = min(shingle[i],pi[i][u-1]);
            }
        }
        //按照signature 划分
        int b = m/r;
        for(int i = 0;i < b;i++) {
            string code = to_string(i);
            //long long code = i;
            for(int j = 0;j < r;j++) {
                //code = (code << 24) | shingle[i * r + j];
                code = code + " " + to_string(shingle[i * r + j]);
            }
            size_t key = hash<string>()(code);
            buckets[key].insert(sn.first);
        }
    }
}

void GraphCom::merge(int iter) {

    double threshold = (iter <= T)?(1.0/(1+iter)) : 0 ;

    for(auto bk:buckets){
        //遍历每一个buckets
        if(bk.second.size() <= 1 ) continue;
        map<int,map<int,int>> node2super;//记录每个点和supernode中的邻接点的数量
        map<int,set<int>>     super2node;//记录每个supernode的临界点
        for(int sn:bk.second){
            //遍历bk中每一个supernode
            for(int v:supernodes[sn]) {

                for(int u:g.getNeighbors(v)) {
                    node2super[u][sn]++;//u 是supernode 中节点的邻接点，u与sn中的点相邻，计数+1
                    super2node[sn].insert(u);//v是超点中的点  u 是supernode sn的邻接点
                }
            }
        }


        vector<int> Q(bk.second.begin(),bk.second.end());
        while(Q.size() >1){
            //在Q中随机选取
            int random = rand()%(Q.size()+1);
            int A = Q[random];
            Q.erase(Q.begin()+random);
            //计算superJaccard

            //A所在的超点是以sa为标志的
            int sa = find(A);
            map<int,int> minSum;
            map<int,int> maxSum;

            for(int u:super2node[sa]) {
                for (auto sm:node2super[u]) {
                    int sv = sm.first;//找到u所邻接到的超点
                    if (sv == sa) continue;
                    if (minSum.count(sv) == 0) {
                        minSum[sv] = 0;
                    }
                    minSum[sv] = minSum[sv] + min(node2super[u][sa], node2super[u][sv]);
                }
            }
            //计算max 用减法就行
            for(int sv:Q){
                maxSum[sv] = deg[sa] + deg[sv] - minSum[sv];
            }
            double maxSuperJaccar = -1.0;
            int sb = -1;
            for(int i = 0;i < Q.size();i++){
                int sv = Q[i];
                double superJaccard = minSum[sv]*1.0 / maxSum[sv];
                if(superJaccard > maxSuperJaccar){
                    maxSuperJaccar = superJaccard;
                    sb = sv;//记录下 jaccar最大的b
                }
            }
            double saving = save(sa,sb);

            //merge A B :B <- A
            if(saving >= threshold){
                // B 在Q中 如果满足了合并的条件 将以sb为合并后的标志
                unite(sb,sa);
                mergeSuperNode(sb,sa);
                deg[sb] = deg[sb] + deg[sa];
                deg.erase(sa);
            }

        }

    }

}
unordered_map<int,int> GraphCom::getSuperDeg(int sv) {
    unordered_map<int,int> superdeg;
    for(int v:supernodes[sv]){
        for(int u:g.getNeighbors(v)){
            int s = find(u);
            if(superdeg.count(s) == 0)
                superdeg[s] = 0;
            superdeg[s]++;
        }
    }
    return superdeg;
}
long GraphCom::getCost(int sv){
    //计算cost要计算 sv的所有邻接的超点的真正的边的数量 和pi 取min
    unordered_map<int,int> superdeg = getSuperDeg(sv);
    long cost = 0;
    for(auto s:superdeg){
        int  sn = s.first;
        //sn 是和sv邻接的supernode
        long pi,edgeCount;
        if(sn == sv){
            pi = supernodes[sv].size();
            pi = pi*(pi-1);
            edgeCount = superdeg[sn];
            pi /= 2;
            edgeCount /= 2;
        }
        else{
            pi = supernodes[sv].size() * supernodes[sn].size();
            edgeCount = superdeg[sn];
        }
        cost += min(pi-edgeCount +1 ,edgeCount);

    }
    return cost;
}

long GraphCom::getMergeCost(int sa,int sb) {
    //计算superdeg
    unordered_map<int,int>superdeg,sd;
    superdeg = getSuperDeg(sa);
    sd = getSuperDeg(sb);
    for(auto s:sd){
        superdeg[s.first] += s.second;
    }
    superdeg[sa] += sd[sb];
    superdeg.erase(sb);
    //
    long cost = 0;
    for(auto s:superdeg){
        int  sn = s.first;
        //sn 是和sv邻接的supernode
        long pi,edgeCount;
        if(sn == sa){
            pi = supernodes[sa].size();
            pi = pi*(pi-1);
            edgeCount = superdeg[sn];
            pi /= 2;
            edgeCount /= 2;
        }
        else{
            pi = supernodes[sa].size() * supernodes[sn].size();
            edgeCount = superdeg[sn];
        }
        cost += min(pi-edgeCount +1 ,edgeCount);

    }
    return cost;
}

double GraphCom::save(int sa,int sb){
    long before = getCost(sa) + getCost(sb);
    long after = getMergeCost(sa,sb);
    double ans = 1 - (after*1.0 / before);
    return ans;
}
void GraphCom::encode(){
    for(auto sn:supernodes){
        //得到点 sn 与所有超点的邻接度数
        unordered_map<int,int> superDeg = getSuperDeg(sn.first);
        for(auto s:superDeg){
            //遍历 sn的所有邻接的超点s 计算pi 和 countedege
            long edgeCount,pi;
            if(s.first == sn.first){
                //判断是否需要添加 自环
                pi = supernodes[s.first].size();
                pi = pi*(pi-1);
                edgeCount = superDeg[s.first];

            }
            else{
                pi = supernodes[s.first].size() * supernodes[sn.first].size();
                edgeCount = superDeg[s.first];
            }
            if(pi-edgeCount + 1 < edgeCount){
                //构造超边P 和 cm
                P.addUnorderedEdge(sn.first,s.first);
                for(int u:supernodes[sn.first]){
                    for(int v:supernodes[s.first]){
                        if(u == v) continue;
                        if(!g.isAdj(u,v))
                            cm.addUnorderedEdge(u,v);
                    }
                }
            }
            else{
                //构造 cp
                for(int u:supernodes[sn.first]){
                    for(int v:supernodes[s.first]) {
                        if(g.isAdj(u,v))
                            cp.addUnorderedEdge(u,v);
                    }
                }
            }

        }
    }
}
void GraphCom::processBatch(int m,int r,int t1) {
    T = t1;
    for(int iter = 1;iter <= T;iter++){
        dividing(m,r);
        merge(iter);
    }
    encode();
}