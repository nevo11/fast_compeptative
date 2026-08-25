//
// Created by nevo4 on 17/08/2026.
//
#include "typ.h"
#include "matth.h"

#include <numeric>
#include <cmath>
#include <iostream>
#include <random>

#include "metrix.h"

using namespace std;

vb primes;
void Eratosthenes_prime(ll m) {
    primes.clear();
    primes.assign(m+1,true);
    if (m >= 0) primes[0] = false;
    if (m >= 1) primes[1] = false;
    for (int k=2; k*k<=m;k++) {
        if (primes[k]) {
            for (int z =k*k;z<=m;z= z+k) {
                primes[z] = false;
            }
        }
    }
}

vll primes_2;
void Eratosthenes_prime_2(ll m) {
    primes_2.clear();
    primes_2.assign(m+1,0);
    for (int k=2; k*k<=m;k++) {
        if (primes_2[k] == 0) {
            for (int z =k*2;z<=m;z= z+k) {
                if (primes_2[z] == 0) primes_2[z] = k;
            }
        }
    }
}

vpll primdevisor;
ll addprime(ll m, ll ind) {
    if (m % ind == 0) {
        ll c = 0;
        while (m %ind == 0) {
            m = m / ind;
            c ++;
        }
        primdevisor.push_back({ind,c});
        return m;
    }
    return m;
}

void find_prime_factors(ll m) {
    ll ind = 3;
    ll msqrt = static_cast<ll>(::sqrt(static_cast<double>(m)));
    Eratosthenes_prime(msqrt);
    primdevisor.clear();
    ll matg = addprime(m, 2);
    while (ind <= msqrt) {
        if (primes[ind]) {
            matg = addprime(matg, ind);
        }
        if (matg == 1) {
            break;
        }
        ind +=2;
    }
    if (matg > 1) {
        addprime(matg, matg);
    }
}

ll number_of_prime_factors() {
    ll v =1;
    for (auto h:primdevisor) {
        v *=1+h.second;
    }
    return v;
}

bool is_perfect(ll m) {
    ll ind;
    ll su=1;
    ind =2;
    if (m ==1) {
        return false;
    }
    while (ind * ind <=m) {
        if (su >m) {
            return false;
        }
        if (m%ind == 0) {
            su +=ind;
            if (ind!=m/ind) {
                su +=m/ind;
            }
        }
        ind++;
    }
    return su == m;
}

bool is_prim(const ll m) {
    if (m <= 1) return false;
    if (m == 2) return true;
    if (m%2==0) {
        return false;
    }
    ll ind = 3;
    while (ind * ind <=m) {
        if (m%ind == 0) {
            return false;
        }
        ind +=2;
    }
    return true;
}

vll factors(const ll m) {
    vll factors;
    ll ind = 1;
    while (ind * ind <=m) {
        if (m%ind == 0) {
            factors.push_back(ind);
            if (m/ind != ind) {
                factors.push_back(m/ind);
            }
        }
        ind +=1;
    }
    return factors;
}

vll primefactors(ll m) {
    vll factors;
    ll ind = 2;
    while (ind * ind <=m) {
        while (m%ind == 0) {
            factors.push_back(ind);
            m /= ind;
        }
        ind +=1;
    }
    if (m>1) {
        factors.push_back(m);
    }
    return factors;
}

ll gcd_hand(ll a, ll b) {
    if (b==0) return a;
    return gcd_hand(b, a%b);
}

ll toint(ll n) {
    ll v =1;
    for (auto h:primdevisor) {
        v *= power_mdular_expention(h.first, h.second-1, -1);
    }
    return v;
}

pll diophantine_gcd(ll a, ll b) {
    if (b==0) return {1,0};
    ll c =a/b;
    pll z = diophantine_gcd(b, a%b);
    return {z.second,-c*z.second + z.first};
}

pll diophantine_eq(ll a, ll b, ll c) {
    ll g = std::gcd(a,b);
    if (c%g ==0) {
        pll p = diophantine_gcd(a,b);
        return {p.first*c/g, p.second*c/g};
    }
    return {0,0};
}

tuple<ll,ll,ll> eucliduce_pythogoram(ll n, ll m) {
    return {n*n-m*m,2*m*n,n*n+m*m};
}

const int MAX_FACT = 1e6 + 5;
vll fact_arr;

void precompute_factorials(ll max_n, ll mod) {
    fact_arr.assign(max_n + 1, 1);
    for (int i = 2; i <= max_n; i++) {
        fact_arr[i] = (fact_arr[i - 1] * i) % mod;
    }
}

ll fact(const ll n, const ll mod) {
    if (n < 0) return -1;
    if (n < 2) return 1;

    if (mod != -1 && !fact_arr.empty() && n < fact_arr.size()) {
        return fact_arr[n];
    }

    ll res = 1;
    for (ll i = 2; i <= n; i++) {
        if (mod == -1) res = res * i;
        else res = (res * i) % mod;
    }
    return res;
}

bool wilson_prime(const ll n) {
    if (n <= 1) return false;
    return fact(n-1, n) == n-1;
}

ll power_mdular_expention(ll base, ll exp, const ll mod) {
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


// ll modInverse(ll n, ll mod) {
//     if (mod == -1) return 1;
//     return power_mdular_expention(n, mod - 2, mod);
// }

ll nCr(ll n, ll k, ll mod) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    ll num = fact(n, -1);

    if (mod == -1) {
        ll den = fact(k, mod) * fact(n - k, mod);
        return num / den;
    }

    ll den = fact(k, -1) * fact(n - k, -1);
    return (num / den) %mod;
}

ll multichoose(ll n, const vll& k, ll mod) {
    if (k.size() == 1) return 1;
    if (k.size() == 2) return nCr(n, k[0], mod);

    ll su = fact(n, -1);
    ll den = 1;

    for (const auto k_i : k) {
        den *= fact(k_i, -1);
    }

    if (mod == -1) return su / den;
    return (su / den)%mod;
}

ll catalan_fast(ll n, ll mod) {
    ll c = nCr(2 * n, n, mod);
    if (mod == -1) return c / (n + 1);
    // ll den_inv = modInverse(n + 1, mod);
    return (c / (n + 1)) % mod;
}

ll derangement(ll n, ll mod) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 1;

    ll a = 0, b = 1, c = 0;
    for (ll i = 3; i <= n; i++) {
        if (mod == -1) {
            c = (i - 1) * (a + b);
        } else {
            c = ((i - 1) * (a + b) % mod) % mod;
        }
        a = b;
        b = c;
    }
    return b;
}

ll burnside_lemma(ll n, ll m, ll mod) {
    ll su = 0;
    for (int i = 0; i < n; i++) {
        if (mod == -1) {
            su += power_mdular_expention(m, std::gcd(i, n), mod);
        } else {
            su = (su + power_mdular_expention(m, std::gcd(i, n), mod)) % mod;
        }
    }

    if (mod == -1) return su / n;
    return (su / n) % mod;
}

vll prufer(vector<unordered_set<ll>> tree, int n) {
    priority_queue<ll> s;
    vll sol;
    for (int i = 1; i <= n; i++) {
        if (tree[i].size() == 1) {
            s.push(-i);
        }
    }
    int i = n;
    while (i > 2) {
        ll sk = -s.top();
        sol.push_back(sk);
        auto f_it = tree[sk].begin();
        ll f = *f_it;
        tree[f].erase(sk);
        if (tree[f].size() == 1) {
            s.push(-f);
        }
        s.pop();
        i--;
    }
    return sol;
}

usll uniun(usll s1, usll s2) {
    for (auto x:s2) {
        s1.insert(x);
    }
    return s1;
}

usll intersect(usll s1, usll s2) {
    usll sol;
    if (s1.size()<s2.size()) {
        for (auto x:s1) {
            if (s2.contains(x)) {
                sol.insert(x);
            }
        }
    }
    else {
        for (auto x:s2) {
            if (s1.contains(x)) {
                sol.insert(x);
            }
        }
    }
    return sol;
}


usll complement(usll A, usll U) {
    usll sol;
    for (auto x:U) {
        if (!A.contains(x)) {
            sol.insert(x);
        }
    }
    return sol;
}


vvd markov_chain;
vector<unordered_set<ll>> markov_chain_edegs;
// o(n^2*m)
vd markovchain(vd p, const ll m) {
    const ll n = p.size();
    for (int i =0;i<m;i++) {
        vd nextp(n,0);
        for (int j=0;j<n;j++) {
            if (p[j]==0) {
                continue;
            }
            for (int k=0;k<n;k++) {
                if (markov_chain[j][k]==0) {
                    continue;
                }
                nextp[k]+=markov_chain[j][k]*p[j];
            }
        }
        p = nextp;
    }
    return p;
}

// o(n^3*ln(m))
vvd markovchain_v2(vd p, const ll m) {
    const ll n = p.size();
    vvd markov_chain_t =transpose_metrix_d(markov_chain, n, n);
    markov_chain_t = power_metrix_d(markov_chain_t,m,n,n);
    vvd p_(n,vd(1,0));
    for (int i =0;i<n;i++) {
        p_[i][0] = p[i];
    }
    return mult_metrix_d(markov_chain_t, p_, n,n,n,1);
}


int order_statistics(vll arr, int i, int f, int d) {
    while (true) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib(i, f); ;
        int m = distrib(gen);
        deque<int> k;
        k.push_back(arr[m]);
        int t=0;
        for (int j =i;j<f;j++) {
            if (j == m) continue;
            if (  arr[j] > arr[m]  ) {
                k.push_back(arr[j]);
            }
            else {
                t++;
                k.push_front(arr[j]);
            }
        }
        if (t+i == d) {
            return arr[m];
        }
        if (t+i > d) {
            int sa = arr[m];
            int y = i;
            while (k.front() != sa) {
                arr[y] = k.front();
                k.pop_front();
                y++;
            }
            f = y - 1;
        }
        else {
            int sa = arr[m];
            int y = f;
            while (k.front() != sa) {
                arr[y] = k.front();
                k.pop_front();
                y--;
            }
            f = y + 1;
        }
    }
}
vvll mult_metrix_(const vvll &A,const vvll &B, const int n_a, const int m_a, const int n_b, const int m_b) {
    if ( m_a!= n_b) {
        return {{}};
    }
    vvll sol;
    for (int i =0;i<n_a;i++) {
        vll tmp;
        for (int j =0;j<m_b;j++) {
            ll tmp2=0;
            for (int k =0;k<m_a;k++) {
                tmp2 += A[i][k]*B[k][j];
            }
            tmp.push_back(tmp2);
        }
        sol.push_back(tmp);
    }
    return sol;
}
bool metix_mul_mc (vvll A, vvll B, vvll C, ll n) {
    vvll X;
    for (int i=0;i<n;i++) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib(1, 10);
        X.push_back({distrib(gen)});
    }
    vvll S = mult_metrix_(A,mult_metrix_(B,X,n,n,n,n),n,n,n,n);
    vvll S_t = mult_metrix_(C,X,n,n,n,n);
    for (int i=0;i<n;i++) {
        if (S[i][0] != S_t[i][0]) {
            return false;
        }
    }
    return true;
}

vb coloring(vvll edges, ll n, ll m) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> bit_dist(0, 1);
    vb sol;
    while (true) {
        sol.clear();
        sol.push_back(false);
        for (ll i =1; i<=n;i++) {
            bool random_bit = bit_dist(gen);
            sol.push_back(random_bit);
        }
        ll m_t= 0;
        for (ll i =1; i<=n;i++) {
            for (auto e: edges[i]) {
                if (sol[i] != sol[e]) {
                    m_t++;
                }
            }
        }
        if (m_t>=m) {
            return sol;
        }
    }
}