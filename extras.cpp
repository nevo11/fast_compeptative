//
// Created by nevo4 on 01/09/2026.
//
#include "typ.h"
#include "extras.h"

template <typename T>
class UnionFindRank {
public:
    unordered_map<T,T> parent;
    unordered_map<T,T> rank;
    UnionFindRank(vector<T> v) {
        for (T vi:v) {
            parent[vi] = vi;
            rank[vi] = 0;
        }
    }
    T find(T v) {
        if (v == parent[v]) {
            return v;
        }
        parent[v] = find(parent[v]);
        return parent[v];
    }
    void union_sets(T a, T b) {
        a = find(a);
        b = find(b);
        if (a!=b) {
            if (rank[a] > rank[b] ) {
                swap(a,b);
            }
            parent[b] = a;
            if (rank[a] == rank[b]) {
                rank[a]++;
            }
        }
    }
};

template <typename T>
class UnionFindSize {
public:
    unordered_map<T,T> parent;
    unordered_map<T,T> size;
    UnionFindSize(vector<T> v) {
        for (T vi:v) {
            parent[vi] = vi;
            size[vi] = 1;
        }
    }
    T find(T v) {
        if (v == parent[v]) {
            return v;
        }
        parent[v] = find(parent[v]);
        return parent[v];
    }
    void union_sets(T a, T b) {
        a = find(a);
        b = find(b);
        if (a!=b) {
            if (size[a] < size[b] ) {
                swap(a,b);
            }
            parent[b] = a;
            size[a] += size[b];
        }
    }
};



template <typename T>
class UnionFindRand {
public:
    unordered_map<T,T> parent;
    unordered_map<T,T> rand_index;
    UnionFindRand(vector<T> v) {
        for (T vi:v) {
            parent[vi] = vi;
            rand_index[vi] = rand();
        }
    }
    T find(T v) {
        if (v == parent[v]) {
            return v;
        }
        parent[v] = find(parent[v]);
        return parent[v];
    }
    void union_sets(T a, T b) {
        a = find(a);
        b = find(b);
        if (a!=b) {
            if (rand_index[a] < rand_index[b] ) {
                swap(a,b);
            }
            parent[b] = a;
        }
    }
};
vvpll edges_wk;
vector<pair<ll,pll>> edgwk;
vpll kruskal(ll n) {
    sort(edgwk.begin(),edgwk.end());
    vll v;
    for (ll i=1;i<=n;i++) {
        v.push_back(i);
    }
    auto ufr = new UnionFindRank<ll>(v);
    vpll sol;
    ll s=0;
    for (auto ed: edgwk) {
        auto a =ed.second.first;
        auto b =ed.second.second;
        if (ufr->find(b) != ufr->find(a)) {
            sol.push_back({a,b});
            s+= ed.first;
            ufr->union_sets(a, b);
        }
    }
    cout << s <<"\n";
    return sol;
}


priority_queue<pair<ll,pll>> pq_jernik_prime;
vpll jernik_prime(ll n) {
    vb has_jernik_prime(n+1, false);
    vpll sol;
    ll summ=0;
    for (auto edgs_a: edges_wk[1]) {
        pq_jernik_prime.push({-edgs_a.second,{1,edgs_a.first}});
    }
    has_jernik_prime[1] = true;
    while (!pq_jernik_prime.empty()) {
        auto s = pq_jernik_prime.top();
        pq_jernik_prime.pop();
        auto a =s.second.first;
        auto b =s.second.second;
        if (!has_jernik_prime[b]) {
            sol.push_back({a,b});
            summ+= -s.first;
            has_jernik_prime[b] = true;
            for (auto edgs_a: edges_wk[b]) {
                pq_jernik_prime.push({-edgs_a.second,{b,edgs_a.first}});
            }
        }
    }
    cout <<summ<<"\n";
    return sol;
}
void build_kru() {
    edges_wk.clear();
    edges_wk.push_back({});
    edges_wk.push_back({{2,3},{5,5}});
    edges_wk.push_back({{1,3},{5,6},{3,5}});
    edges_wk.push_back({{4,9},{6,3},{2,5}});
    edges_wk.push_back({{6,7},{3,9}});
    edges_wk.push_back({{1,5},{2,6},{6,2}});
    edges_wk.push_back({{5,2},{3,3},{4,7}});
    edgwk.clear();
    edgwk.push_back({3,{1,2}});
    edgwk.push_back({6,{2,5}});
    edgwk.push_back({5,{1,5}});
    edgwk.push_back({2,{5,6}});
    edgwk.push_back({3,{3,6}});
    edgwk.push_back({5,{2,3}});
    edgwk.push_back({9,{3,4}});
    edgwk.push_back({7,{4,6}});
}

vvll edge_top;
vvll edge_top_rev;
vi color;
vll sol_top;
bool req_dfs_top1(const ll s) {
    if (color[s] == 1) {
        return false;
    }
    if (color[s] == 2) {
        return true;
    }
    color[s] = 1;
    bool h = true;
    for (const ll et :edge_top[s]) {
        h = h && req_dfs_top1(et);
    }
    color[s] = 2;
    sol_top.push_back(s);
    return h;
}
vll top1(const ll s) {
    req_dfs_top1(s);
    reverse(sol_top.begin(),sol_top.end());
    return sol_top;
}
vll paths;
vll toppath(ll s, ll n) {
    for (ll i=0;i<=n;i++) {
        paths.push_back(0);
    }
    paths[s] = 1;
    for (const ll st: sol_top) {
        for (const ll revst: edge_top_rev[st]) {
            paths[st] += paths[revst];
        }
    }
    return paths;
}

void build_top() {
    edge_top.clear();
    edge_top.push_back({0});
    edge_top.push_back({2,4});
    edge_top.push_back({3});
    edge_top.push_back({6});
    edge_top.push_back({5});
    edge_top.push_back({2, 3});
    edge_top.push_back({});
    edge_top_rev.clear();
    edge_top_rev.push_back({});
    edge_top_rev.push_back({});
    edge_top_rev.push_back({1,5});
    edge_top_rev.push_back({2,5});
    edge_top_rev.push_back({1});
    edge_top_rev.push_back({4});
    edge_top_rev.push_back({3});
    sol_top.clear();
    color.clear();
    for (ll i=0;i<7;i++) {
        color.push_back(0);
    }
}

vll succ;
vector<unordered_map<ll,ll>> succ_;
ll calc_suc(ll k, ll x) {
    if (!(k && k-1)) {
        return succ_[x][k];
    }
    ll u =1;
    while (u < k) {
        u *=2;
    }
    while (u >0) {
        if (u<=k) {
            x = succ_[x][u];
            k -= u;
        }
        else {
            u = u/2;
        }
    }
    return x;
}


void pre_calc_suc(const ll n) {
    ll i =1;
    const unordered_map<ll,ll> tmp1;
    succ_.push_back(tmp1);
    while (i<=n) {
        for (ll j =1; j<=n;j++) {
            if (i==1) {
                unordered_map<ll,ll> tmp;
                tmp[1] =succ[j];
                succ_.push_back(tmp);
            }
            else {
                succ_[j][i] = succ_[succ_[j][i/2]][i/2];
            }
        }
        i*=2;
    }
}

void build_suc() {
    succ.clear();
    succ.push_back({});
    succ.push_back({3});
    succ.push_back({5});
    succ.push_back({7});
    succ.push_back({6});
    succ.push_back({2});
    succ.push_back({2});
    succ.push_back({1});
    succ.push_back({6});
    succ.push_back({3});

}

vvll kosaraju_edges;
vvll kosaraju_edges_rev;
vpll kosaraju_time;
vpll kosaraju_ptime;
ll t;
vb kosaraju_was;
vvll kosaraju_sol;
void dfs_kosaraju( ll s) {
    if (kosaraju_was[s]) {
        return;
    }
    kosaraju_was[s]= true;
    t++;
    ll t1 = t;
    for (ll ke: kosaraju_edges[s]) {
        dfs_kosaraju(ke);
    }
    t++;
    kosaraju_time.push_back({t1,t});
    kosaraju_ptime.push_back({t, s});
}
ll i_kosaraju;
void dfs_kosaraju_2( ll s, ll i) {
    if (kosaraju_was[s]) {
        return;
    }
    if (i ==0) {
        i_kosaraju++;
        kosaraju_sol.push_back({});
    }
    kosaraju_sol[i_kosaraju].push_back(s);
    kosaraju_was[s]= true;
    for (ll ke: kosaraju_edges_rev[s]) {
        dfs_kosaraju_2(ke, i_kosaraju);
    }
}
vvll kosaraju(ll n) {
    t=0;
    for (ll i =0; i<=n;i++) {
        kosaraju_was.push_back(false);
    }
    kosaraju_time.clear();
    kosaraju_ptime.clear();
    dfs_kosaraju(1);
    reverse(kosaraju_ptime.begin(),kosaraju_ptime.end());
    kosaraju_was.clear();
    for (ll i =0; i<=n;i++) {
        kosaraju_was.push_back(false);
    }

    for (auto rev: kosaraju_ptime) {
        dfs_kosaraju_2(rev.second,0);
    }
}
void build_kosaraju() {
    kosaraju_edges.clear();
    kosaraju_edges.push_back({});
    kosaraju_edges.push_back({2,4});
    kosaraju_edges.push_back({1,5});
    kosaraju_edges.push_back({2,7});
    kosaraju_edges.push_back({});
    kosaraju_edges.push_back({4});
    kosaraju_edges.push_back({5,3});
    kosaraju_edges.push_back({6});
}


// vvll kosaraju_sat() {
//     t=0;
//     for (ll i =0; i<=n;i++) {
//         kosaraju_was.push_back(false);
//     }
//     kosaraju_time.clear();
//     kosaraju_ptime.clear();
//     dfs_kosaraju(1);
//     reverse(kosaraju_ptime.begin(),kosaraju_ptime.end());
//     kosaraju_was.clear();
//     for (ll i =0; i<=n;i++) {
//         kosaraju_was.push_back(false);
//     }
//
//     for (auto rev: kosaraju_ptime) {
//         dfs_kosaraju_2(rev.second,0);
//     }
// }
void sat2() {
    vpll a = {
        {1,2},
        {1,-2},
        {-1,3},
        {-1,-3}
    };
    ll n =3;
    unordered_map<ll,vll> edges;
    for (pll o:a) {
        if (edges.contains(-o.first)) {
            edges[-o.first] = {o.second};
        }
        else {
            edges[-o.first].push_back(o.second);
        }
        if (edges.contains(-o.second)) {
            edges[-o.second] = {o.first};
        }
        else {
            edges[-o.second].push_back(o.first);
        }
        //kosaraju_sat();

    }

}
class TRIE {
public:
    TRIE* childs[26]{};
    ll c;
    TRIE() {
        c=1;
        for (auto & child : childs){
            child = nullptr;
        }
    }
    void add(const string &s, const ll i) {
        if (s.length()==i) {
            return;
        }
        if  (childs[s[i]-'a']==nullptr) {
            const auto tr = new TRIE();
            childs[s[i]-'a'] = tr;
        }
        else {
            c++;
        }
        childs[s[i]-'a']->add(s, i+1);
    }
    bool search(const string &s, const ll i) {
        if (s.length()==i) {
            return true;
        }
        if  (childs[s[i]-'a']==nullptr) {
            return false;
        }
        if (!childs[s[i]-'a']->search(s, i+1)) {
            return false;
        }
        return true;
    }
    string longestprefix(const string &s, const ll i) {
        if (s.length()==i) {
            return "";
        }
        if  (childs[s[i]-'a']==nullptr) {
            return "";
        }
        string r = s[i] + childs[s[i] - 'a']->longestprefix(s, i + 1);
        return r;
    }
};
ll power_mdular_expention_hashing(ll base, ll exp, const ll mod) {
    ll res = 1;
    if (mod == -1) {
        while (exp > 0) {
            if (exp % 2 == 1) res = (res * base);
            base = base * base;
            exp /= 2;
        }
        return res;
    }
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}
ll hashing(const string& s, const ll A, const ll B) {
    ll so=0;
    ull j=s.length()-1;
    for (const auto c:s) {
        so =(so+((c)*power_mdular_expention_hashing(A,j,B))%B)%B;
        j--;
    }
    return so;
}
vll hhash;
vll phash;
void preprosshash(string s, ll n, ll A, ll B) {
    hhash.push_back(s[0]);
    for (ll i=1;i<n;i++) {
        hhash.push_back(((hhash[i-1]*A)%B+ s[i])%B);
    }
    phash.push_back(1);
    for (ll i=1;i<n;i++) {
        phash.push_back((phash[i-1]*A)%B);
    }
}
ll subhashing(ll a, ll b, ll B) {
    if (a ==0) {
        return hhash[b];
    }
    const ll s = (hhash[b]-hhash[a-1] * phash[b-a+1]) %B;
    if (s<0) {
        return s+B;
    }
    return s;
}

bool orderlexicographic(string s1,string s2, ll n) {
    ll maxn = min(s1.length(), s2.length());
    ll max = maxn;
    ll min = 0;
    ll m = (max+min)/2;
    while (true) {
        if (s1[m] == s2[m] && s1[m+1]!=s2[m+1]) {
            return s1[m+1]< s2[m+1];
        }
        if (s1[m] == s2[m]) {
            min = m;
        }
        else {
            max = m;
        }
        m = (min+max)/2;
        if (min +1 == max) {
            if (s1[m] == s2[m] && s1[m+1]==s2[m+1]) {
                if (max != maxn) {
                    m = max;
                }
                else {
                    return s1.length()<s2.length();
                }
            }
        }
    }

}

vll z_algo(string s, ll n) {
    vll k;
    k.push_back(0);
    if (s[0] == s[1]) {
        k.push_back(1);
    }
    else {
        k.push_back(0);
    }
    ll i =2;
    ll x =-1;
    ll y =0;
    while (i<n) {
        ll j =0;
        if (x!= -1 && s[j] == s[i+j] && i+k[i-x]<y) {
            k.push_back(k[i-x]);
            i++;
            continue;
        }
        while (s[j] == s[i+j]) {
            j++;
        }
        if (j!=0 && j+i-1>y) {
            y= j+i-1;
            x = i;
        }
        k.push_back(j);
        i++;
    }
    return k;
}

vll good_z_algo(const string &s) {
    const ull n = s.size();
    vll z(n);
    ll x=0,y=0;
    for ( ll i =1;i<n;i++) {
        z[i] = max(0ll, min(z[i-x], y-i +1));
        while (i+z[i] <n&&s[z[i]]==s[i+z[i]]) {
            x=i;
            y=i+z[i];
            z[i]++;
        }
    }
    return z;
}

class srds {
public:
    vvll arr;
    vll val;
    srds(const vll& ar, ull k) {
        if (k ==-1) {
            const ull n = ar.size();
            ll m = sqrt(ar.size());
            k = n/m;
            ll l = n-m*k;
            for (ll i=0; i<k;i++) {
                arr.push_back({});
                ll su =0;
                for (ll j=0;j<m;j++) {
                    arr[i].push_back(ar[i*m+j]);
                    su += ar[i*m+j];
                }
                val.push_back(su);
            }
            if (l!=0) {
                arr.push_back({});
                ll su =0;
                for (ll j=0;j<l;j++) {
                    arr[k].push_back(ar[k*m+j]);
                    su += ar[k*m+j];
                }
                val.push_back(su);
            }
        }
    }

};