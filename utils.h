//
// Created by nevo4 on 06/08/2026.
//

#ifndef FAST_COMPEPTATIVE_UTILS_H
#define FAST_COMPEPTATIVE_UTILS_H
#include "typ.h"

void dfs_tree(const ll s, const ll e);
void build_dfs_tree();
void dfs(const ll s);
void clean_dfs(ll ndfs);
void bfs(ll s);
void build_dfs();
void build_bfs();
void clean_bfs(const ll nbfs);
void bellman_ford(ll bfn, ll x);
void show_dis(ll bfn);
void fast_bellman_ford(ll bfn, ll x);
void build_fast_bf();
void build_bf();
void daikstra(ll n, ll x);
void show_daikstra(ll n);
void build_daikstra();
void floyd_warshall(const ll n);
void build_war();
void show_war(ll n);

void Edmons_Karp_FF(ll s, ll t);
void scaling_FF(ll s, ll t);
ll calculate_max_flow(ll s);
vvll uniq_FF(ll s, ll t);
int test();
void init_graph(ll n);
void add_edge(ll u, ll v, ll cap);
#endif //FAST_COMPEPTATIVE_UTILS_H
