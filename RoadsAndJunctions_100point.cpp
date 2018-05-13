// 方針
// 全体にN^2個のjunctionを作ろうという方針
// [0,S]にN個の接続点を作るためには[1/(N+1),2/(N+1),...,N/(N+1)]にjunctionを打てば良い
// N = 10で試す. これは N^2 = 100の点を作成し,クラスカルに10^4かかることを想定すると10^7でちょっと余裕があるかな?くらい
// N = 50くらいにしてもいいかもしれない
// 追記 : 制約で間接点は20個までしか作れなかったので実際は100個の点のうち,20個を選択している

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <set>
#include <numeric>

#define all(v) (v).begin(),(v).end()
#define rep(i,n) for(int i=0;i<(int)(n);i++)
using ll = long long;
constexpr int dx[] = {1,0,-1,0};
constexpr int dy[] = {0,1,0,-1};

using namespace std;


template<typename T> ostream& operator<<(ostream& os,const vector<T>& vec){ os << "["; for(const auto& v : vec){ os << v << ","; } os << "]"; return os; }
template<typename T> ostream& operator<<(ostream& os,const vector<T>&& vec){ os << "["; for(const auto&& v : vec){ os << v << ","; } os << "]"; return os; }

class UnionFind{/*{{{*/
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
};/*}}}*/

class City{/*{{{*/
    public:
        int x,y;
        City(){}
        City(int x,int y) : x(x),y(y){}
        friend ostream& operator<<(ostream& os,const City& c){
            os << "(" << c.x << "," << c.y << ")";
            return os;
        }
};/*}}}*/
using Junction = City;

class Edge{/*{{{*/
    public:
        int a,b; 
        double cost;
        Edge(){}
        Edge(int a,int b,double cost) : a(a),b(b),cost(cost){}
        bool operator<(const Edge& e) const{ return cost<e.cost; }
};/*}}}*/

inline double dist(const City& c1,const City& c2){/*{{{*/
    double dx=c1.x-c2.x;
    double dy=c1.y-c2.y;
    return sqrt(dx*dx+dy*dy);
}/*}}}*/

vector<Edge> last_es;

double Kruskal(const vector<City>& cities,const vector<Junction>& junctions={},const vector<int>& junctionStatus = {},const double precost = 0){/*{{{*/
    //cerr << "Kruskal : " << junctions << " " << junctionStatus << endl;
    int NC = cities.size();
    int NJ = junctions.size();
    int N = NC + accumulate(junctionStatus.begin(),junctionStatus.end(),0);

    vector<Edge> &es = last_es;
    es.clear();
    es.resize(N*(N-1)/2);
    int idx=0;
    // city & city
    for(int i=0;i<NC;i++){
        for(int j=i+1;j<NC;j++){
            es[idx++] = Edge(i,j,dist(cities[i],cities[j]));
        }
    }
    // city & junction
    for(int i=0;i<NC;i++){
        for(int j=0;j<NJ;j++){
            if(junctionStatus[j] == 0) continue;
            es[idx++] = Edge(i,j+NC,dist(cities[i],junctions[j]));
        }
    }
    // junction & junction
    for(int i=0;i<NJ;i++){
        if(junctionStatus[i] == 0) continue;
        for(int j=i+1;j<NJ;j++){
            if(junctionStatus[j] == 0) continue;
            es[idx++] = Edge(i+NC,j+NC,dist(junctions[i],junctions[j]));
        }
    }
    sort(all(es));

    double road_length = precost;
    UnionFind uf(NC+NJ);
    for(int i=0;i<es.size();i++){
        if(uf.same(es[i].a,es[i].b)) continue;
        uf.unite(es[i].a,es[i].b);
        City a = (es[i].a<NC) ? cities[es[i].a] : junctions[es[i].a-NC];
        City b = (es[i].b<NC) ? cities[es[i].b] : junctions[es[i].b-NC];
        road_length += dist(a,b);
    }
    return road_length;
}/*}}}*/
vector<int> gen_road_by_kruskal(const vector<City>& cities,const vector<Junction>& junctions={},const vector<int>& junctionStatus = {},const double precost = 0){/*{{{*/
    //cerr << "gen road: " << junctions << " " << junctionStatus << endl;
    int NC = cities.size();
    int NJ = junctions.size();
    int N = NC + accumulate(junctionStatus.begin(),junctionStatus.end(),0);

    vector<Edge> &es = last_es;
    es.clear();
    es.resize(N*(N-1)/2);
    int idx=0;
    // city & city
    for(int i=0;i<NC;i++){
        for(int j=i+1;j<NC;j++){
            es[idx++] = Edge(i,j,dist(cities[i],cities[j]));
        }
    }
    // city & junction
    for(int i=0;i<NC;i++){
        for(int j=0;j<NJ;j++){
            if(junctionStatus[j] == 0) continue;
            es[idx++] = Edge(i,j+NC,dist(cities[i],junctions[j]));
        }
    }
    // junction & junction
    for(int i=0;i<NJ;i++){
        if(junctionStatus[i] == 0) continue;
        for(int j=i+1;j<NJ;j++){
            if(junctionStatus[j] == 0) continue;
            es[idx++] = Edge(i+NC,j+NC,dist(junctions[i],junctions[j]));
        }
    }
    sort(all(es));

    vector<int> ret;
    UnionFind uf(NC+NJ);
    for(int i=0;i<es.size();i++){
        if(uf.same(es[i].a,es[i].b)) continue;
        uf.unite(es[i].a,es[i].b);
        ret.push_back(es[i].a);
        ret.push_back(es[i].b);
    }
    return ret;
}/*}}}*/

class RoadsAndJunctions {
public:
    int NC;
    int NJ;
    double junctionCost;
    vector<City> cities;
    vector<Junction> junctions;
    set<pair<int,int>> city_place;

    vector<int> buildJunctions(int S, vector<int> cities_, double junctionCost_, double failureProbability){
        const int expectedValue = 1.0 / failureProbability;
        junctionCost = junctionCost_;
        NC = cities_.size() / 2;
        for(int i=0;i<NC;i++) cities.emplace_back(City(cities_[2*i],cities_[2*i+1]));
        for(int i=0;i<NC;i++) city_place.insert(make_pair(cities[i].x,cities[i].y));

        ll midx=0,midy=0;
        for(int i=0;i<NC;i++){
            midx+=cities[i].x;
            midy+=cities[i].y;
        }
        midx/=NC;
        midy/=NC;

        double NoJunctionScore= Kruskal(cities);

        const int DN = 10;
        set<pair<int,int>> st;
        for(int i=0;i<DN;i++){
            int p1 = S*(i+1)/(DN+1);
            for(int j=0;j<DN;j++){
                int p2 = S*(j+1)/(DN+1); 
                st.insert(make_pair(p1,p2));
            }
        }

        set<pair<int,int>> junctionCandidate;    // 候補間接点 (実際は確率で失敗するので安全のために周囲いくつかを建設する)
        for(const pair<int,int>& p : st){
            vector<Junction> j{Junction(p.first,p.second)};
            vector<int> s{1};
            double tmp = Kruskal(cities,j,s,junctionCost*9);
            if(tmp<NoJunctionScore) junctionCandidate.insert(p);
        }

        if(junctionCandidate.empty()){
            return vector<int>();
        }

        set<pair<int,int>> junction_set;         // 実間接点 (実際に作成する間接点)
        if(junctionCandidate.size() < 20){
            junction_set.insert(junctionCandidate.begin(),junctionCandidate.end());
            int rem = 20 - junctionCandidate.size();
            int d = 0;
            int cnt = 0;
            while(rem){
                int dx = d / 3 - 1;
                int dy = d % 3 - 1;
                for(const auto& p : junctionCandidate){
                    junction_set.insert(make_pair(p.first + dx , p.second + dy));
                    if(!--rem) break;
                }
                if(++cnt==9) break;
            }
        }else{
            int rem=20;
            for(const auto& p : junctionCandidate){
                junction_set.insert(p);
                if(!--rem) break;
            }
        }

        vector<int> ret;
        for(const auto& junc : junction_set){
            if(city_place.count(junc)) continue;
            ret.emplace_back(junc.first);
            ret.emplace_back(junc.second);
            junctions.emplace_back(Junction(junc.first,junc.second));
        }
        return ret;
    }

    vector<int> buildRoads(vector <int> junctionStatus) {
        NJ = junctionStatus.size();
        //cerr << "JUNCTIONS " << junctions << endl;
        //cerr << "junctionStatus " << junctionStatus << endl;
        return gen_road_by_kruskal(cities,junctions,junctionStatus,junctionCost*NJ);
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
