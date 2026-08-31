//
// Created by nevo4 on 06/08/2026.
//


#include <complex>

#include "typ.h"
int ma() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    ll n;
    cin >> n >> ws;
    std::string name;
    std::getline(std::cin, name);
    typedef long long C;
    typedef complex<C> P;
    #define X real()
    #define Y imag()
    P v = {3,1};
    P u = {2,2};

    P s = v+u;
    P s2 = v-u;
    cout <<abs(s2) << arg(v) << (conj(v)*u).Y;
    // 1/2 sum p_i X p_i+1 = AREA
    // pick a+b/2+1 AREA on integar gread
    return 0;
}