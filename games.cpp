//
// Created by nevo4 on 18/08/2026.
//

#include "games.h"
//
// Created by nevo4 on 17/08/2026.
//
#include "typ.h"
#define N 1000000
#include <bitset>
#include <iostream>
#include <random>
vll nim_;


pll nim_move(ll n) {
    ll s=0;
    for (auto ni:nim_) {
        s = (s^ni);
    }
    if (s == 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> pile_dist(0, n-1);
        ll i = pile_dist(gen);
        while (nim_[i] == 0) {
            i = pile_dist(gen);
        }
        std::uniform_int_distribution<int> stick_dis(1, nim_[i]);
        return {i, stick_dis(gen)};
    }
    for (ll i =0; i<n;i++) {
        if ((nim_[i]^s) <nim_[i]) {
            return {i,nim_[i]-(nim_[i]^s)};
        }
    }
}
void nimgame(bool show, ll n) {
    ll n_tag = n;
    while (n_tag>0) {
        if (show) {
            for (auto ni:nim_) {
                cout <<ni << " ";
            }
            cout<<"\n";
        }
        pll z = nim_move(n);
        if (z.second == nim_[z.first]) {
            n_tag--;
        }
        nim_[z.first] -= z.second;
    }

}


unordered_map<ll,ll>counting;
priority_queue<pll> edge;
vector<unordered_set<ll>> gragh;
vector<unordered_set<ll>> rev_gragh;
qll nex_counting;

void countt(ll n) {
    for (ll i =1; i<=n;i++) {
        if (gragh[i].empty()) {
            counting[i]=0;
            ll s = i;
            nex_counting.push(s);
            // process node s
            while (!nex_counting.empty()){
                s = nex_counting.front();
                nex_counting.pop();
                // visitedb_counting[s] = true;
                for (auto u: rev_gragh[s]) {
                    if (counting.contains(u)) {
                        if (counting[u] < counting[s]+1) {
                            counting[u] = counting[s]+1;
                            nex_counting.push(u);
                        }
                    }
                    else {
                        counting[u] = counting[s]+1;
                        nex_counting.push(u);
                    }
                }
            }
        }
    }
    for (ll i =1; i<=n;i++) {
        edge.push({-counting[i],i});
    }
}
unordered_map<ull,ull> grundy(const ll n) {
    counting.clear();
    countt(n);
    unordered_map<ull,ull> grund;
    while (!edge.empty()) {
        pll a = edge.top();
        edge.pop();
        if (a.first==0) {
            grund[a.second] =0;
        }
        else {
            ll f = 0;
            bitset<12> m(string("111111111111"));
            for (auto gra:gragh[a.second]) {
                m[grund[gra]] = false;
            }
            grund[a.second] = m._Find_first();
        }
    }
    return grund;
    // for (ll i =1; i<=n;i++) {
    //     if (gragh[i].empty()) {
    //         grund[i] = 0;
    //         for (auto rg :rev_gragh[i]) {
    //             if (gragh[rg].size() == 1) {
    //                 grund[rg] = 1;
    //             }
    //             else {
    //                 edge.push({counting[rg],rg});
    //             }
    //         }
    //     }
    // }
}


void countt_move(ll n, priority_queue<pll> edge_move, unordered_map<ll,ll>counting_move, vector<unordered_set<ll>> gragh_move, vector<unordered_set<ll>> rev_gragh_move) {
    for (ll i =1; i<=n;i++) {
        if (gragh_move[i].empty()) {
            counting[i]=0;
            ll s = i;
            nex_counting.push(s);
            // process node s
            while (!nex_counting.empty()){
                s = nex_counting.front();
                nex_counting.pop();
                // visitedb_counting[s] = true;
                for (auto u: rev_gragh_move[s]) {
                    if (counting_move.contains(u)) {
                        if (counting_move[u] < counting_move[s]+1) {
                            counting_move[u] = counting_move[s]+1;
                            nex_counting.push(u);
                        }
                    }
                    else {
                        counting_move[u] = counting_move[s]+1;
                        nex_counting.push(u);
                    }
                }
            }
        }
    }
    for (ll i =1; i<=n;i++) {
        edge_move.push({-counting_move[i],i});
    }
}
unordered_map<ull,ull> grundy_(const ll n, priority_queue<pll> edge_move, unordered_map<ll,ll> counting_move, vector<unordered_set<ll>> gragh_move, vector<unordered_set<ll>> rev_gragh_move) {
    counting.clear();
    countt_move(n, edge_move, counting_move, gragh_move,  rev_gragh_move);
    unordered_map<ull,ull> grund;
    while (!edge_move.empty()) {
        pll a = edge_move.top();
        edge_move.pop();
        if (a.first==0) {
            grund[a.second] =0;
        }
        else {
            ll f = 0;
            bitset<12> m(string("111111111111"));
            for (auto gra:gragh_move[a.second]) {
                m[grund[gra]] = false;
            }
            grund[a.second] = m._Find_first();
        }
    }
    return grund;
    // for (ll i =1; i<=n;i++) {
    //     if (gragh[i].empty()) {
    //         grund[i] = 0;
    //         for (auto rg :rev_gragh[i]) {
    //             if (gragh[rg].size() == 1) {
    //                 grund[rg] = 1;
    //             }
    //             else {
    //                 edge.push({counting[rg],rg});
    //             }
    //         }
    //     }
    // }
}
// pll grundy_move(ll k, vector<tuple<ll, priority_queue<pll>, unordered_map<ll,ll>, vector<unordered_set<ll>>, vector<unordered_set<ll>>>> all_mightiy) {
//     ll s=0;
//     vector<unordered_map<ull,ull>> grundys;
//     for (int h=0;h<k;h++) {
//
//         grundys.push_back(grundy_(std::apply(all_mightiy[h]));
//     }
//     for (auto ni:nim_) {
//         s = (s^ni);
//     }
//     if (s == 0) {
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<int> pile_dist(0, k-1);
//         ll i = pile_dist(gen);
//         while (nim_[i] == 0) {
//             i = pile_dist(gen);
//         }
//         std::uniform_int_distribution<int> stick_dis(1, nim_[i]);
//         return {i, stick_dis(gen)};
//     }
//     for (ll i =0; i<k;i++) {
//         if ((nim_[i]^s) <nim_[i]) {
//             return {i,nim_[i]-(nim_[i]^s)};
//         }
//     }
// }

int exemple() {
    nim_ = {5,12,10};
    nimgame(true, 3);
    cout << "\n";
    gragh ={
        {},
        {},
        {1},
        {2},
        {1,2},
        {4,2},
        {5,2},
    };
    rev_gragh ={
        {},
        {2,4},
        {3,4,5,6},
        {},
        {5},
        {6},
        {},
    };
    auto p = grundy(6);
    for (auto [key,val]:p) {
        cout << key <<": " << val<<"\n";
    }
    cout << "\n";
    gragh ={
        {},
        {},
        {1},
        {},
        {3},
        {2},
        {5},
        {5,6,3},
        {},
        {2,5,8},
        {3,7},
        {10},
        {2,5,9},
        {12},
        {12,13,3,7,10},
        {12,13,14,11},
        {},
        {16,2,5,9,12},
        {16,17,13},
        {16,17,18,3,7,10,14},
        {16,17,18,19,15,11}
    };
    rev_gragh.clear();
    for (int i =0;i<=20;i++) {
        rev_gragh.push_back({});
    }
    for (int i =1;i<=20;i++) {
        for (const auto ppp: gragh[i]) {
            rev_gragh[ppp].insert(i);
        }
    }
    auto p2 = grundy(20);
    for (auto [key,val]:p2) {
        cout << key <<": " << val<<"\n";
    }
    cout << "\n";
    return 0;
}
