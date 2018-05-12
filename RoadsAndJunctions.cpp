// 方針
// 新しく接続点は作らずにcityだけでクラスカルを用いて最小全域木を求める
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#define all(v) (v).begin(),(v).end()

using namespace std;

class UnionFind{
    public:
        vector<int> rank,p;
        UnionFind(int n) : rank(n,0), p(n){
            for(int i=0;i<n;i++) p[i]=i;
        }
        int root(int x){
            if(p[x]==x) return x;
            else return p[x] = root(p[x]);
        }
        void unite(int x,int y){
            x=root(x);
            y=root(y);
            if(x==y) return;
            if(rank[x]<rank[y]) p[x]=y;
            else{
                p[y]=x;
                if(rank[x]==rank[y]) rank[x]++;
            }
        }
        inline bool same(int x,int y){
            return root(x)==root(y);
        }
};

class City{
    public:
        int x,y;
        City(){}
        City(int x,int y) : x(x),y(y){}
};

class Edge{
    public:
        int a,b; 
        double cost;
        Edge(){}
        Edge(int a,int b,double cost) : a(a),b(b),cost(cost){}
        bool operator<(const Edge& e) const{ return cost<e.cost; }
};

inline double dist(const City& c1,const City& c2){
    double dx=c1.x-c2.x;
    double dy=c1.y-c2.y;
    return sqrt(dx*dx+dy*dy);
}

class RoadsAndJunctions {
public:
    int NC;
    int NJ;
    vector<City> cities;

    vector<int> buildJunctions(int S, vector<int> cities_, double junctionCost, double failureProbability) {
        NC = cities_.size() / 2;
        for(int i=0;i<NC;i++) cities.emplace_back(City(cities_[2*i],cities_[2*i+1]));

        return vector<int>({});
    }

    // 返り値 : 建築したいroadのリスト.要素数は
    vector<int> buildRoads(vector <int> junctionStatus) {
        NJ = junctionStatus.size();
        vector<Edge> es((NC*NC-NC)/2);
        int idx=0;
        for(int i=0;i<NC;i++){
            for(int j=i+1;j<NC;j++){
                es[idx++]=Edge(i,j,dist(cities[i],cities[j]));
            }
        }
        sort(all(es));

        vector<int> ret;
        UnionFind uf(NC);
        for(int i=0;i<es.size();i++){
            if(uf.same(es[i].a,es[i].b)) continue;
            uf.unite(es[i].a,es[i].b);
            ret.emplace_back(es[i].a);
            ret.emplace_back(es[i].b);
        }
        return ret;
    }
};
// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
    for (int i = 0; i < v.size(); ++i)
        cin >> v[i];
}

int main() {
    RoadsAndJunctions rj;
    int S, C;
    cin >> S >> C;
    vector<int> cities(C);
    getVector(cities);
    double junctionCost, failureProbability;
    cin >> junctionCost >> failureProbability;

    vector<int> ret = rj.buildJunctions(S, cities, junctionCost, failureProbability);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;
    cout.flush();
    
    int J;
    cin >> J;
    vector<int> junctionStatus(J);
    getVector(junctionStatus);

    ret = rj.buildRoads(junctionStatus);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;
    cout.flush();
}
