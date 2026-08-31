//
// Created by nevo4 on 29/08/2026.
//

#ifndef FAST_COMPEPTATIVE_GEOMTRY_H
#define FAST_COMPEPTATIVE_GEOMTRY_H
template <typename T>
T horn(const T a, const T b , const T c);
template <typename T>
T pol_area(const vector<pair<T,T>> &p, const T n) ;


template <typename P_>
bool eqP(const P_ a, const P_ b);
template <typename P_>
auto cros(const P_ a, const P_ b);
template <typename P_>
auto line_check(const P_ p, const pair<P_,P_> &s);

template <typename P_>
bool lines_check(const pair<P_,P_> &l1, const pair<P_,P_> &l2);

template <typename P_>
auto d(const P_ p, const pair<P_,P_> &s);


template <typename P_>
bool polypoint(const P_ p, const vector<pair<P_,P_>> &pol, const double n);

template <typename T>
T pol_area2(const vector<complex<T>> &p, const T n);

template <typename T>
T pol_area_pick(const vector<pair<complex<T>,complex<T>>> &pol, const vector<complex<T>> &op);

template <typename T>
T Manhattandis(vector<complex<T>> &pa, const ll n);

ll sweep_line(vpll events);

typedef vector<pair<ll,pair<ll,ll>>> vpll_pll;
ll intersection_points(vpll hsb,vpll hse,vpll_pll vs, ll nymap_size);
typedef vector<pair<ld,ld>> vpld;
typedef pair<ld,ld> pld;
ld dis(const pld &a, const pld &b);
ld stcpp(const vpld &p, const ll i);

ld cpp(vpld p, ll n);

bool check_andrews(const vpll &sol, pll pp);
bool check_andrews2(const vpll &sol, pll pp);
vpll andrews(vpll p);
#endif //FAST_COMPEPTATIVE_GEOMTRY_H
