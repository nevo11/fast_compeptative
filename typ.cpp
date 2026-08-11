//
// Created by nevo4 on 01/08/2026.
//

#include "typ.h"

template <typename T_2>
class BIT2 {
public:
    vector<T_2> tree;
    T_2 n;
    BIT2(T_2 n_, T_2* arr_) {
        vector<T_2> clc = calc_pref(arr_, n_);
        for (T_2 i = 1; i<=n_;i++) {
            T_2 k = 1;
            while (i%(2*k)==0) {
                k *=2;
            }
            tree.push_back(sum_v2(clc,(i-1) - k + 1, i-1));
        }
        n = n_;
    }
    int sum_(T_2 a) {
        int su =0;
        while (a >0) {
            su += tree[a-1];
            a -= a&-a;
        }
        return su;
    }
    int sum_q(T_2 a, T_2 b) {
        return sum_(b) - sum_(a-1);
    }
    void add(T_2 i, T_2 v) {
        while (i<=n) {
            tree[i-1] +=v;
            i += i&-i;
        }
    }

};
template <typename T>
class Segment {
public:
    T val;
    vector<T> segment;
    vector<T> org;
    T n_;
    T default_val;
    Segment(vector<T> v, T v_n, T n, T dv) {
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
        return min(a,b);
    }
    // void delta(T v, T i) {
    //     T ii = n_-1+i;
    //     segment[ii] += v;
    //     while (segment[ii] == org[i]) {
    //         segment[ii] = action(segment[ii*2+1], segment[2*ii+2]);
    //         ii = (ii-1)/2;
    //     }
    // }
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