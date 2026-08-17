//
// Created by nevo4 on 17/08/2026.
//

#ifndef FAST_COMPEPTATIVE_MATTH_H
#define FAST_COMPEPTATIVE_MATTH_H

#include "typ.h"
#include <tuple>
#include <vector>
#include <unordered_set>

extern vb primes;
extern vll primes_2;
extern vpll primdevisor;

void Eratosthenes_prime(ll m);
void Eratosthenes_prime_2(ll m);
ll addprime(ll m, ll ind);
void find_prime_factors(ll m);
ll number_of_prime_factors();

bool is_perfect(ll m);
bool is_prim(const ll m);
vll factors(const ll m);
vll primefactors(ll m);

ll gcd_hand(ll a, ll b);
ll toint(ll n);

pll diophantine_gcd(ll a, ll b);
pll diophantine_eq(ll a, ll b, ll c);
std::tuple<ll, ll, ll> eucliduce_pythogoram(ll n, ll m);

void precompute_factorials(ll max_n, ll mod);
ll fact(const ll n, const ll mod = 1);
bool wilson_prime(const ll n);
ll power_mdular_expention(ll base, ll exp, const ll mod);
ll modInverse(ll n, ll mod);

ll nCr(ll n, ll k, ll mod);
ll multichoose(ll n, const vll& k, ll mod);
ll catalan_fast(ll n, ll mod);
ll derangement(ll n, ll mod);
ll burnside_lemma(ll n, ll m, ll mod);
vll prufer(std::vector<std::unordered_set<ll>> tree, int n);

extern vvd markov_chain;
extern vector<unordered_set<ll>> markov_chain_edegs;
vd markovchain(vd p, const ll m);
vvd markovchain_v2(vd p, const ll m);

int order_statistics(vll arr, int i, int f, int d);
vvll mult_metrix_(const vvll &A,const vvll &B, const int n_a, const int m_a, const int n_b, const int m_b);
bool metix_mul_mc (vvll A, vvll B, vvll C, ll n);

vb coloring(vvll edges, ll n, ll m);

usll uniun(usll s1, usll s2);
usll intersect(usll s1, usll s2);
usll complement(usll A, usll U);
#endif //FAST_COMPEPTATIVE_MATTH_H