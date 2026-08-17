//
// Created by nevo4 on 16/08/2026.
//

#ifndef FAST_COMPEPTATIVE_METRIX_H
#define FAST_COMPEPTATIVE_METRIX_H
//
// Created by nevo4 on 16/08/2026.
//

#include "utils.h"
vvll transpose_metrix(const vvll &A, const int n, const int m);
vvll sum_metrix(const vvll &A,const vvll &B, const int n_a, const int m_a, const int n_b, const int m_b);
vvll mult_scalar_metrix(const vvll &A,const ll c, const int n_a, const int m_a);
vvll mult_metrix(const vvll &A,const vvll &B, const int n_a, const int m_a, const int n_b, const int m_b);
vvll power_metrix(const vvll &A,const ll k, const int n, const int m);
ll det(vvll &A, ll n);
vvll shortest_path(const vvll &V,ll k, ll n);

vvd transpose_metrix_d(const vvd &A, const int n, const int m);
vvd sum_metrix_d(const vvd &A,const vvd &B, const int n_a, const int m_a, const int n_b, const int m_b);
vvd mult_scalar_metrix_d(const vvd &A,const double c, const int n_a, const int m_a);
vvd mult_metrix_d(const vvd &A,const vvd &B, const int n_a, const int m_a, const int n_b, const int m_b);
vvd power_metrix_d(const vvd &A,const ll k, const int n, const int m);
double det_d(vvd &A, ll n);
vvd shortest_path_d(const vvd &V,ll k, ll n);

ll krichoff(vector<vector<bool>> edges,vll edges_, ll n);
#endif //FAST_COMPEPTATIVE_METRIX_H
