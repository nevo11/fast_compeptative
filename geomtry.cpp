//
// Created by nevo4 on 29/08/2026.
//
#include "typ.h"
#include "geomtry.h"
#define X real()
#define Y imag()
template <typename T>
T horn(const T a, const T b , const T c) {
    const T s = (a+b+c)/static_cast<T>(2);
    return ::sqrt(s*(s-a)*(s-b)*(s-c));
}
template <typename T>
T pol_area(const vector<pair<T,T>> &p, const T n) {
    auto [fst, snd] = p[0];
    T sum = 0;
    for (T i =0;i<n-1;++i) {
        sum += (p[i].first * p[i+1].second - p[i+1].first * p[i].second);
    }
    sum += (p[n-1].first*snd-p[n-1].second * fst);
    return abs(sum)/2;
}


template <typename P_>
bool eqP(const P_ a, const P_ b) {
    return a.X == b.X && a.Y == b.Y;
}
template <typename P_>
auto cros(const P_ a, const P_ b) {
    return (conj(a)*b).Y;
}
template <typename P_>
auto line_check(const P_ p, const pair<P_,P_> &s) {
    return cros(p-s.first, p-s.second);
}


// template <typename P_>
// auto safe_line_check(const P_ p, const pair<P_,P_> &s) {
//     P_ a = p-s.first;
//     P_ b = p-s.second;
//     if (a.X == 0 || b.Y == 0) {
//         return -(b.X / abs(b.X)) * (a.Y / abs(a.Y));
//     }
//     if (b.X == 0 || a.Y == 0) {
//         return (b.Y / abs(b.Y)) * (a.X / abs(a.X));
//     }
//     if (a.X == a.Y) {
//         return (b.Y-b.X)/abs(b.Y-b.X);
//     }
//     if (b.X == b.Y) {
//         return (a.X-a.Y)/abs(a.X-a.Y);
//     }
//
// }

template <typename P_>
bool lines_check(const pair<P_,P_> &l1, const pair<P_,P_> &l2){
    if (eqP(l1.first, l2.first) || eqP(l1.second, l2.first)||eqP(l1.first, l2.second)||eqP(l1.second, l2.second)) {
        return true;
    }
    auto l112 = line_check(l1.first, l2);
    l112 = l112 / abs(l112);
    auto l122 = line_check(l1.second, l2);
    l122 = l122 / abs(l122);
    auto l211 = line_check(l2.first, l1);
    l211 = l211 / abs(l211);
    auto l221 = line_check(l2.second, l1);
    l221 = l221 / abs(l221);
    if (l112 == 0 && l122 == 0) {
        using CoordType = decltype(l1.first.X);
        if (l1.first.X != l2.first.X) {
            vector<CoordType> x ={l1.first.X,l1.second.X,l2.first.X,l2.second.X};
            sort(x.begin(),x.end());
            if (x[0] == l1.first.X || x[0] == l1.second.X) {
                if (x[1] == l1.first.X || x[1] == l1.second.X) {
                    return false;
                }
                return true;
            }
            if (x[1] == l2.first.X || x[1] == l2.second.X) {
                return false;
            }
            return true;
        }
        vector<CoordType> y ={l1.first.Y,l1.second.Y,l2.first.Y,l2.second.Y};
        sort(y.begin(),y.end());
        if (y[0] == l1.first.Y || y[0] == l1.second.Y) {
            if (y[1] == l1.first.Y || y[1] == l1.second.Y) {
                return false;
            }
            return true;
        }
        if (y[1] == l2.first.Y || y[1] == l2.second.Y) {
            return false;
        }
        return true;
    }
    if (l211 * l221 < 0 && l112 * l122 < 0) {
        return true;
    }
    if (l211 * l221 < 0 && l112 * l122 == 0) {
        return true;
    }
    if (l211 * l221 == 0 && l112 * l122 < 0) {
        return true;
    }
    return false;
}

template <typename P_>
auto d(const P_ p, const pair<P_,P_> &s) {
    return abs(line_check(p,s)/abs(s.second-s.first));
}


template <typename P_>
bool polypoint(const P_ p, const vector<pair<P_,P_>> &pol, const double n) {
    auto maxx = abs(p.X - pol[0].first.X);
    auto maxy = abs(p.Y - pol[0].first.Y);
    for (auto z:pol) {
        if (eqP(z.first, p)) {
            return true;
        }
        if ( maxx < abs(p.X - z.first.X)) maxx = abs(p.X - z.first.X);
        if ( maxy < abs(p.Y - z.first.Y)) maxy = abs(p.Y - z.first.Y);
        if (line_check(p,z) == 0) {
            auto xx = z.first.X - z.second.X;
            auto yy = z.first.Y - z.second.Y;
            P_ p_tag = {p.X-xx, p.Y+yy};
            if (line_check(z.first,{p,p_tag}) * line_check(z.second,{p,p_tag}) < 0) {
                return true;
            }
        }
    }
    P_ rot = { maxx, maxy };
    auto d = polar(1.0,2*M_PI/(n-1));
    double k = 0;
    for (double i = 0; i < n; i++) {
        ll h = 0;
        for (auto z:pol) {
            if (lines_check({rot + p,p},z)) h++;
        }
        if (h%2 == 1) {
            k++;
        }
        rot *= d;
    }
    if ( k > n/2) {
        return true;
    }
    return false;
}

template <typename T>
T pol_area2(const vector<complex<T>> &p, const T n) {
    T sum = 0;
    for (T i =0;i<n-1;++i) {
        sum += cros(p[i],p[i+1]);
    }
    sum += cros(p[n-1],p[0]);
    return abs(sum)/2;
}

template <typename T>
T pol_area_pick(const vector<pair<complex<T>,complex<T>>> &pol, const vector<complex<T>> &op) {
    T a = 0;
    T b = 0;
    for (auto opt: op) {
        bool br=false;
        for (auto z:pol) {
            if (eqP(z.first, opt)) {
                b++;
                br = true;
                break;
            }
            if (line_check(opt,z) == 0) {
                auto xx = z.first.X - z.second.X;
                auto yy = z.first.Y - z.second.Y;
                complex<T> p_tag = {opt.X-xx, opt.Y+yy};
                if (line_check(z.first,{opt,p_tag}) * line_check(z.second,{opt,p_tag}) < 0) {
                    b++;
                    br = true;
                    break;
                }
            }
        }
        if (!br && polypoint(op, pol, 100)) {
            a++;
        }
    }
    return a+b/static_cast<T>(2)+1;
}

template <typename T>
T Manhattandis(vector<complex<T>> &pa, const ll n) {
    T maxx=std::numeric_limits<T>::lowest();
    T maxy=std::numeric_limits<T>::lowest();
    T minx=std::numeric_limits<T>::max();
    T miny=std::numeric_limits<T>::max();
    for (ll i=0;i<n;i++) {
        T xx = pa[i].X+pa[i].Y;
        T yy = -pa[i].X+pa[i].Y;
        if (maxx < xx) maxx = xx;
        if (minx > xx) minx = xx;
        if (maxy < yy) maxy = yy;
        if (miny > yy) miny = yy;
    }
    return max(abs(maxx-minx), abs(maxy-miny));
}

ll sweep_line(vpll events) {
    sort(events.begin(), events.end());
    priority_queue<ll> s;
    ll m=0;
    ll mm=0;
    for (auto [fst, snd]: events) {
        s.push(-snd);
        while (fst > -s.top()) {
            s.pop();
            m--;
        }
        m++;
        if (mm<m) mm =m;
    }
    return mm;
}

template <typename T>
class SUMSegment {
public:
    T val;
    vector<T> segment;
    vector<T> org;
    T n_;
    T default_val;
    SUMSegment(vector<T> v, T v_n, T n, T dv) {
        n_ = n;
        segment.resize(2*n);
        org = v;
        default_val = dv;
        for (ll i =n-1;i<2*n-1;i++) {
            if (v_n > i-n+1 ) {
                segment[i] = v[i-n+1];
            }
            else {
                segment[i] = default_val;
            }
        }
        for (ll i =n-2;i>-1; i--) {
            segment[i] = action(segment[i*2+1], segment[2*i+2]);
        }
    }
    T action(T a, T b) {
        return a+b;
    }
    void update(T v, T i) {
        T ii = n_-1+i;
        segment[ii] = v;
        if (n_ == 1) return;
        while (ii>=0) {
            ii = (ii-1)/2;
            T temp = segment[ii];
            segment[ii] = action(segment[ii*2+1], segment[2*ii+2]);
            if (ii == 0 || temp == segment[ii]) {
                return;
            }
        }
    }
    void inc(T i) {
        T ii = n_-1+i;
        update(segment[ii]+1,i);
    }
    void dec(T i) {
        T ii = n_-1+i;
        update(segment[ii]-1,i);
    }
    void delta(T v,T i) {
        T ii = n_-1+i;
        update(segment[ii]+v,i);
    }
    T range( T a, T b) {
        if (a ==b) {
            return segment[n_-1+a];
        }
        T k_a =n_-1+a;
        T k_b =n_-1+b;
        T ret = default_val;
        while (k_a <= k_b) {
            if (k_b %2 == 1) {
                ret = action(ret, segment[k_b]);
                k_b--;
            }
            if (k_a %2 == 0) {
                ret = action(ret, segment[k_a]);
                k_a++;
            }
            if (k_a > k_b) {
                break;
            }
            k_a = (k_a-1)/2;
            k_b = (k_b-1)/2;
        }
        return ret;
    }
    void print() {
        for (auto v :segment) {
            cout << v << " ";

        }
        cout << "\n";
    }
};
typedef vector<pair<ll,pair<ll,ll>>> vpll_pll;
ll intersection_points(vpll hsb,vpll hse,vpll_pll vs, ll nymap_size) {
    sort(hsb.begin(), hsb.end());
    sort(hse.begin(), hse.end());
    sort(vs.begin(), vs.end());
    ll hsbn = hsb.size();
    ll hsen = hse.size();
    ll vsn = vs.size();
    const vll v(nymap_size,0);
    ll n =1;
    while (n < nymap_size) n = n << 1;
    SUMSegment<ll> *s = new SUMSegment<ll>(v, nymap_size, n, 0);
    ll j = 0 ,i = 0, k=0;
    ll su =0;
    while (i<hsen && k<vsn) {
        ll f = min(hse[i].first,vs[k].first);
        while (j<hsbn && hsb[j].first <= f) {
            s->inc(hsb[j].second);
            j++;
        }
        while (k<vsn && vs[k].first == f) {
            su += s->range(vs[k].second.first,vs[k].second.second);
            k++;
        }
        while (i<hsen && hse[i].first == f) {
            s->dec(hse[i].second);
            i++;
        }
    }
    delete s;
    return su;

}
typedef vector<pair<ld,ld>> vpld;
typedef pair<ld,ld> pld;
ld dis(const pld &a, const pld &b) {
    const ld x = (a.first-b.first)*(a.first-b.first);
    const ld y = (a.second-b.second)*(a.second-b.second);
    return sqrt(x+y);
}
ld stcpp(const vpld &p, const ll i) {
    ld d = dis(p[0], p[i]);
    for (ll j=i; j < i;j++) {
        if (const ld dji = dis(p[j], p[i]); d < dji) d = dji;
    }
    return d;
}

ld cpp(vpld p, ll n) {
    sort(p.begin(),p.end());
    ll i = 2;
    ld d = dis(p[0], p[1]);
    set<pld> s ={{p[0].second,p[0].first}};
    s.insert({p[1].second,p[1].first});
    queue<pld> q;
    q.push(p[0]);
    q.push(p[1]);
    while (i <n) {
        while (!q.empty() && (p[i].first - q.front().first)>=d) {
            pld f ={q.front().second,q.front().first};
            const auto jf = s.find(f);
            s.erase(jf);
            q.pop();
        }
        // if (i<10) {
        //     if (const ld dji = stcpp(p, i); dji<d) d=dji;
        // }
        if (s.size() !=0) {
            auto low = s.lower_bound({p[i].second-d, numeric_limits<ld>::min()});
            auto high = s.upper_bound({p[i].second+d, numeric_limits<ld>::max()}) ++;
            while (low != high) {
                auto a = *low;
                const ld d2 = dis(p[i],{a.second, a.first});
                if ( d2 <d) {
                    d = d2;
                }
                ++low;
            }
        }
        q.push(p[i]);
        s.insert({p[i].second,p[i].first});
        i++;
    }
    return d;
}

bool check_andrews(const vpll &sol, pll pp) {
    const ll n = sol.size();
    const P last ={pp.first,pp.second};
    const P last2 ={sol[n-1].first,sol[n-1].second};
    const P last3 ={sol[n-2].first,sol[n-2].second};
    return cros(last2-last3, last-last2) > 0;
}
bool check_andrews2(const vpll &sol, pll pp) {
    const ll n = sol.size();
    const P last ={pp.first,pp.second};
    const P last2 ={sol[n-1].first,sol[n-1].second};
    const P last3 ={sol[n-2].first,sol[n-2].second};
    return cros(last2-last3, last-last2) < 0;
}
vpll andrews(vpll p) {
    sort(p.begin(),p.end());
    auto it = p.begin();
    vpll sol = {};
    while (it !=p.end()) {
        auto pp = *it;
        while (sol.size()>1 && check_andrews(sol, pp)) {
            sol.pop_back();
        }
        sol.push_back(pp);
        it++;
    }
    auto it2 = p.rbegin();
    vpll sol2 = {};
    while (it2 !=p.rend()) {
        auto pp = *it2;
        while (sol2.size()>1 && check_andrews(sol2, pp)) {
            sol2.pop_back();
        }
        sol2.push_back(pp);
        it2++;
    }
    sol.pop_back();
    sol2.pop_back();
    sol.insert(sol.end(),sol2.begin(),sol2.end());
    return sol;
}

template bool lines_check<std::complex<long long>>(
    const std::pair<std::complex<long long>, std::complex<long long>>&,
    const std::pair<std::complex<long long>, std::complex<long long>>&
);