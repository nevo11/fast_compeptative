//
// Created by nevo4 on 06/08/2026.
//

#include "utils.h"

#include <unordered_set>

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

vvll dff_edge;
vvll dff_edge_weight;
vvll dff_edge_weight_cap;
vb Edmons_Karp_visitedb;
qll nex_Edmons_Karp;
vll father_path;
pair<ll,vll> Edmons_Karp_path(ll s_1, ll t) {
    while (!nex_Edmons_Karp.empty()) {
        nex_Edmons_Karp.pop();
    }
    fill(Edmons_Karp_visitedb.begin(), Edmons_Karp_visitedb.end(), false);
    fill(father_path.begin(), father_path.end(), -1);


    nex_Edmons_Karp.push(s_1);
    Edmons_Karp_visitedb[s_1] = true; // mark visited at discovery (push), not at pop
    // process node s
    while (!nex_Edmons_Karp.empty()){
        const ll s = nex_Edmons_Karp.front();
        nex_Edmons_Karp.pop();
        if (s == t) {
            vll ret = {t};
            ll fath = father_path[t];
            ll mn = dff_edge_weight_cap[fath][t] - dff_edge_weight[fath][t] + dff_edge_weight[t][fath];
            while (fath != s_1) {
                ret.push_back(fath);
                ll las = fath;
                fath = father_path[fath];
                mn = min(mn, dff_edge_weight_cap[fath][las] - dff_edge_weight[fath][las] + dff_edge_weight[las][fath]);
            }
            ret.push_back(fath);
            reverse(ret.begin(), ret.end());
            return {mn,ret};
        }
        for (auto u: dff_edge[s]) {
            if (!Edmons_Karp_visitedb[u] && dff_edge_weight_cap[s][u] - dff_edge_weight[s][u] + dff_edge_weight[u][s] > 0) {
                Edmons_Karp_visitedb[u] = true;
                father_path[u] = s;
                nex_Edmons_Karp.push(u);
            }
        }
    }
    return {0,{}};
}

ll residual_cap(ll u, ll v) {
    return dff_edge_weight_cap[u][v] - dff_edge_weight[u][v] + dff_edge_weight[v][u];
}

void Edmons_Karp_FF(ll s, ll t) {
    if (s == t) return; // no augmenting path is meaningful when source == sink
    for (auto &sd:dff_edge_weight) {
        for (auto &sdtm:sd) {
            sdtm = 0;
        }
    }
    pair<ll,vll> path = Edmons_Karp_path(s, t);
    while (path.first != 0) {
        ll las =-1;
        for (auto h : path.second) {
            if (las!=-1) {
                dff_edge_weight[las][h] += path.first;
                ll min_e = min(dff_edge_weight[las][h],dff_edge_weight[h][las]);
                dff_edge_weight[las][h] -= min_e;
                dff_edge_weight[h][las] -= min_e;
            }
            las = h;
        }
        path = Edmons_Karp_path(s, t);
    }
}

vb was;
vll req_path(const ll s, ll t, const ll del) {
    if (was[s]) {
        return {};
    }
    was[s] = true;
    if (s == t) {
        return {t};
    }
    for (const ll u: dff_edge[s]) {
        if (dff_edge_weight_cap[s][u] - dff_edge_weight[s][u] + dff_edge_weight[u][s] >= del) {
            vll f = req_path(u, t, del);
            if ( f.size() >0) {
                f.push_back(s);
                return f;
            }
        }
    }
    return {};
}

pair<ll,vll> scaling_path(ll C, ll st, ll t) {
    while (C > 0) {
        was.assign(dff_edge.size(), false);
        vll pa = req_path(st, t, C);
        if (pa.size() != 0) {
            reverse(pa.begin(), pa.end());
            return {C,pa};
        }
        C = C / 2;
    }
    return {C,{}};
}
void scaling_FF(ll s, ll t) {
    if (s == t) return; // avoid trivial s==t "path" causing an infinite augmenting loop
    ll C = 1;
    ll max_c = 0;
    for (auto cc:dff_edge_weight_cap) {
        for (auto c:cc) {
            max_c = max(max_c, c);
        }
    }
    while (C * 2 <= max_c) {
        C = C * 2;
    }
    for (auto &cd:dff_edge_weight) {
        for (auto &cdff:cd) {
            cdff = 0;
        }
    }
    pair<ll,vll> path = scaling_path(C, s, t);
    while (path.first != 0) {
        ll las =-1;
        for (auto h : path.second) {
            if (las!=-1) {
                dff_edge_weight[las][h] += path.first;
                ll min_e = min(dff_edge_weight[las][h],dff_edge_weight[h][las]);
                dff_edge_weight[las][h] -= min_e;
                dff_edge_weight[h][las] -= min_e;
            }
            las = h;
        }
        path = scaling_path(path.first, s, t);
    }
}


void init_graph(ll n) {
    dff_edge.assign(n, vll());
    dff_edge_weight.assign(n, vll(n, 0));
    dff_edge_weight_cap.assign(n, vll(n, 0));
    Edmons_Karp_visitedb.assign(n, false);
    was.assign(n, false);
    father_path.clear();
    father_path.assign(n, -1);
    while(!nex_Edmons_Karp.empty()) nex_Edmons_Karp.pop();
}

void add_edge(ll u, ll v, ll cap) {
    dff_edge[u].push_back(v);
    dff_edge[v].push_back(u); // Residual graph reverse edge support
    dff_edge_weight_cap[u][v] += cap;
}

ll calculate_max_flow(ll s) {
    ll flow = 0;
    for (auto subflow: dff_edge_weight[s]) {
        flow += subflow;
    }
    return flow;
}

vvll uniq_FF(ll s, ll t) {
    // if (s == t) {
    //     return {{t}};
    // }
    vvll uniq_flows;
    ll c =0;
    for (auto edge: dff_edge[s]) {
        if (dff_edge_weight[s][edge] == 1) {
            dff_edge_weight[s][edge] = 0;
            vll uniq_flow;
            uniq_flow.push_back(s);
            ll ne = edge;
            while (ne != t) {
                ll lastn = ne;
                for (auto edge_2: dff_edge[ne]){
                    if (dff_edge_weight[ne][edge_2] == 1) {
                        dff_edge_weight[ne][edge_2] = 0;
                        uniq_flow.push_back(ne);
                        ne = edge_2;
                        break;
                    }
                }
                if (lastn == ne) {
                    break;
                }
            }
            if (ne == t) {
                uniq_flow.push_back(t);
                uniq_flows.push_back(uniq_flow);
            }
        }
    }
    return uniq_flows;
}
// --- Test Driver ---

void run_test(const string& test_name, int n, ll s, ll t, const vector<tuple<int, int, ll>>& edges, ll expected_flow) {
    cout << "========================================" << endl;
    cout << "Test Case: " << test_name << endl;

    // Test Edmonds-Karp
    init_graph(n);
    for (auto& [u, v, cap] : edges) add_edge(u, v, cap);
    Edmons_Karp_FF(s, t);
    ll ek_flow = calculate_max_flow(s);

    // Test Capacity Scaling
    init_graph(n);
    for (auto& [u, v, cap] : edges) add_edge(u, v, cap);
    scaling_FF(s, t);
    ll scaling_flow = calculate_max_flow(s);

    cout << "Expected Max Flow     : " << expected_flow << endl;
    cout << "Edmonds-Karp Result   : " << ek_flow << " [" << (ek_flow == expected_flow ? "PASSED" : "FAILED") << "]" << endl;
    cout << "Capacity Scaling Result: " << scaling_flow << " [" << (scaling_flow == expected_flow ? "PASSED" : "FAILED") << "]" << endl;
    cout << "========================================" << endl << endl;
}

int test() {
    // Test Case 1: Standard Textbook Flow Network (6 nodes)
    // 0: Source (s), 5: Sink (t)
    vector<tuple<int, int, ll>> test1_edges = {
        {0, 1, 16}, {0, 2, 13},
        {1, 2, 10}, {2, 1, 4},
        {1, 3, 12}, {3, 2, 9},
        {2, 4, 14}, {4, 3, 7},
        {3, 5, 20}, {4, 5, 4}
    };
    run_test("Standard 6-node Network", 6, 0, 5, test1_edges, 23);

    // Test Case 2: Simple Diamond Graph (4 nodes)
    // 0: Source, 3: Sink
    vector<tuple<int, int, ll>> test2_edges = {
        {0, 1, 1000000}, {0, 2, 1000000},
        {1, 2, 1},
        {1, 3, 1000000}, {2, 3, 1000000}
    };
    run_test("Diamond Graph with High Capacity", 4, 0, 3, test2_edges, 2000000);

    // Test Case 3: No path from s to t at all -> flow should be 0
    vector<tuple<int, int, ll>> test3_edges = {
        {0, 1, 10}, {2, 3, 10}
    };
    run_test("Disconnected Source/Sink", 4, 0, 3, test3_edges, 0);

    // Test Case 4: Single direct edge s -> t
    vector<tuple<int, int, ll>> test4_edges = {
        {0, 1, 7}
    };
    run_test("Single Direct Edge", 2, 0, 1, test4_edges, 7);

    // Test Case 5: Classic case requiring the residual (reverse) edge
    // to be used to reach the true max flow (bottleneck forces cancellation).
    vector<tuple<int, int, ll>> test5_edges = {
        {0, 1, 1}, {0, 2, 1},
        {1, 2, 1},
        {1, 3, 1}, {2, 3, 1}
    };
    run_test("Requires Residual Cancellation", 4, 0, 3, test5_edges, 2);

    // Test Case 6: Parallel-ish multi-hop chain, flow limited by weakest link
    vector<tuple<int, int, ll>> test6_edges = {
        {0, 1, 5}, {1, 2, 3}, {2, 3, 8}, {0, 3, 2}
    };
    run_test("Bottleneck Chain Plus Direct Edge", 4, 0, 3, test6_edges, 5);

    // Test Case 7: s == t edge case (trivial, flow is 0 by definition here
    // since calculate_max_flow sums outgoing flow from s, and no augmenting
    // path search should be needed).
    vector<tuple<int, int, ll>> test7_edges = {
        {0, 1, 5}
    };
    run_test("Source Equals Sink", 2, 0, 0, test7_edges, 0);

    // Test Case 8: Larger random DAG-ish graph, cross-checking EK vs scaling
    // against each other (both must agree, exact value computed by hand below).
    // Layered graph 0 -> {1,2} -> {3,4} -> 5
    vector<tuple<int, int, ll>> test8_edges = {
        {0, 1, 10}, {0, 2, 10},
        {1, 3, 4}, {1, 4, 8},
        {2, 3, 9}, {2, 4, 6},
        {3, 5, 10}, {4, 5, 10}
    };
    run_test("Layered Graph", 6, 0, 5, test8_edges, 20);

    return 0;
}