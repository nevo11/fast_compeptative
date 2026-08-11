//
// Created by nevo4 on 06/08/2026.
//

#include "utils.h"
#include "typ.h"
#define INF 9223372036854775807
vvll dt_adj;
void dfs_tree(const ll s, const ll e) {
    for (const auto u:dt_adj[s]) {
        if (u != e) {
            dfs_tree(u,s);
        }
    }
}
void build_dfs_tree() {
    dt_adj.clear();
    dt_adj.push_back({});
    dt_adj.push_back({2,3,4});
    dt_adj.push_back({5,6,1});
    dt_adj.push_back({1});
    dt_adj.push_back({7,1});
    dt_adj.push_back({2});
    dt_adj.push_back({8,2});
    dt_adj.push_back({4});
    dt_adj.push_back({6});
}



vb visited;
vvll adjs;

void dfs(const ll s) {
    if (visited[s]) return;
    visited[s] = true;
    // process node s
    for (const auto u: adjs[s]) {
        dfs(u);
    }
}
void clean_dfs(ll ndfs) {
    visited.clear();
    for (int i =0;i<ndfs;i++) {
        visited.push_back( false);
    }
}

vb visitedb;
vvll adjb;
qll nex;
void bfs(ll s) {
    nex.push(s);
    // process node s
    while (!nex.empty()){
        s = nex.front();
        nex.pop();
        if (visitedb[s]) {
            continue;
        }
        visitedb[s] = true;
        for (auto u: adjb[s]) {
            nex.push(u);
        }
    }
}
void build_dfs() {
    adjs.push_back({1,3}); // f
    adjs.push_back({0,2}); // t
    adjs.push_back({1,3}); // f
    adjs.push_back({2,4,0}); //t
    adjs.push_back({3,5});
    adjs.push_back({4,6});
    adjs.push_back({5,7});
    adjs.push_back({6,8});
    adjs.push_back({7});
    adjs.push_back({8});
}

void build_bfs() {
    adjb.push_back({1});
    adjb.push_back({0,2});
    adjb.push_back({1,3});
    adjb.push_back({2,4});
    adjb.push_back({3,5});
    adjb.push_back({4,6});
    adjb.push_back({5,7});
    adjb.push_back({6,8});
    adjb.push_back({7,9});
    adjb.push_back({9});
}
void clean_bfs(const ll nbfs) {
    visitedb.clear();
    for (int i =0;i<nbfs;i++) {
        visitedb.push_back( false);
    }
}

vll distan;
typedef tuple<ll,ll,ll> edg;
vector<edg> edges ;
void bellman_ford(ll bfn, ll x) {
    distan.clear();
    for (int i =0;i<=bfn;i++) {
        distan.push_back(INF);
    }
    distan[x] =0;
    for (int i =1;i<bfn;i++) {
        for (auto e: edges) {
            ll a,b,w;
            tie(a,b,w) = e;
            if (distan[a] != INF) {
                distan[b] = min(distan[b], distan[a]+w);
            }
        }
    }
}
void show_dis(ll bfn) {
    for (int i=1;i<=bfn;i++) {
        cout << "i = " << i << " distance[i] = " << distan[i] << "\n";
    }
    cout << "\n";
}
queue<ll> qedg;
vvll adjbf;
vvll adjbf_w;
void fast_bellman_ford(ll bfn, ll x) {
    for (int i =1;i<=bfn;i++) {
        distan.push_back(INF);
    }
    distan[x] = 0;
    qedg.push(x);
    while (!qedg.empty()){
        ll ob = qedg.front();
        qedg.pop();
        for (const auto e: adjbf[ob]) {
            if (const ll w = adjbf_w[ob][e]; distan[ob]+w < distan[e]) {
                qedg.push(ob);
                distan[e] = distan[ob]+w;
            }
        }
    }
}

void build_fast_bf() {
    adjbf.clear();
    adjbf.push_back({});
    adjbf.push_back({3,4,2});
    adjbf.push_back({1,4,5});
    adjbf.push_back({1,4});
    adjbf.push_back({1,2,3,5});
    adjbf.push_back({2,4});
    adjbf_w.clear();
    adjbf_w.push_back({-1,-1,-1,-1,-1,-1});
    adjbf_w.push_back({-1,-1, 5, 3, 7,-1});
    adjbf_w.push_back({-1, 5,-1,-1, 3, 2});
    adjbf_w.push_back({-1, 3,-1,-1, 1,-1});
    adjbf_w.push_back({-1, 7, 3, 1,-1, 2});
    adjbf_w.push_back({-1,-1, 2,-1, 2,-1});

}
void build_bf() {
    edges.clear();
    edges.push_back({1,3,3});
    edges.push_back({1,4,7});
    edges.push_back({1,2,5});
    edges.push_back({2,4,3});
    edges.push_back({2,5,2});
    edges.push_back({3,4,1});
    edges.push_back({4,5,2});

    edges.push_back({3,1,3});
    edges.push_back({4,1,7});
    edges.push_back({2,1,5});
    edges.push_back({4,2,3});
    edges.push_back({5,2,2});
    edges.push_back({4,3,1});
    edges.push_back({5,4,2});
}

priority_queue<pll> qedijkstar;
vb processed_dikstra;
vll distan_dikstra;
vector<vector<pair<ll,ll>>> adj_dikstar;
void daikstra(ll n, ll x) {
    distan_dikstra.clear();
    processed_dikstra.clear();
    for (int i = 0; i <= n; i++) {
        distan_dikstra.push_back( INF);
        processed_dikstra.push_back(false);
    }
    distan_dikstra[x] = 0;
    qedijkstar.push({0,x});
    while (!qedijkstar.empty()) {
        const ll a = qedijkstar.top().second;
        qedijkstar.pop();
        if (processed_dikstra[a]) {
            continue;
        }
        processed_dikstra[a] = true;
        for (auto u : adj_dikstar[a]) {
            ll b = u.first, w = u.second;
            if (distan_dikstra[a]+w < distan_dikstra[b]) {
                distan_dikstra[b] = distan_dikstra[a]+w;
                qedijkstar.push({-distan_dikstra[b],b});
            }
        }
    }
}
void show_daikstra(ll n) {
    for (int i=1;i<=n;i++) {
        cout << "i = " << i << " distance[i] = " << distan_dikstra[i] << "\n";
    }
    cout << "\n";
}
void build_daikstra() {
    adj_dikstar.clear();
    adj_dikstar.push_back({});
    adj_dikstar.push_back({{2,1},{4,9},{5,1}});
    adj_dikstar.push_back({{1,1},{3,2}});
    adj_dikstar.push_back({{4,6},{2,2}});
    adj_dikstar.push_back({{3,6},{1,9},{5,2}});
    adj_dikstar.push_back({{1,1},{4,2}});
}


vvll diswar;
void floyd_warshall(const ll n) {
    for ( ll i =1; i<=n;i++) {
        for (ll j =1; j<=n;j++) {
            for ( ll k =j+1; k<=n;k++) {
                if (diswar[i][j]!=INF && diswar[i][k]!=INF) {
                    ll mm = min(diswar[i][j]+diswar[i][k],diswar[j][k]);
                    diswar[j][k] = mm;
                    diswar[k][j] = mm;
                }
            }
        }
    }
}
void build_war() {
    diswar.clear();
    diswar.push_back({});
    diswar.push_back({INF, 0   ,5 ,INF,   9,1});
    diswar.push_back({INF, 5   ,0, 2,    INF,INF});
    diswar.push_back({INF, INF ,2, 0,    7,INF});
    diswar.push_back({INF, 9   ,INF, 7,    0,2});
    diswar.push_back({INF, 1   ,INF, INF,2,0});
}

void show_war(ll n) {
    cout << "    ";
    for (ll i = 1; i<=n;i++) {
        cout << i << " ";
    }
    cout << "\n";
    for (ll i = 0; i<=n;i++) {
        cout << "__";
    }
    cout << "\n";
    for (ll i = 1; i<=n;i++) {
        cout << i <<" | ";
        for (ll j = 1; j<=n;j++) {
            cout << diswar[i][j] << " ";
        }
        cout << "\n";
    }
}