//
// Created by nevo4 on 16/08/2026.
//

#include "metrix.h"
#include "utils.h"
#define INF 100000000
#include "typ.h"
#include <vector>
#include <cmath>
vvll transpose_metrix(const vvll &A, const int n, const int m) {
    vvll sol;
    for (int i =0;i<m;i++) {
        vll tmp;
        for (int j =0;j<n;j++) {
            tmp.push_back(A[j][i]);
        }
        sol.push_back(tmp);
    }
    return sol;
}

vvll sum_metrix(const vvll &A,const vvll &B, const int n_a, const int m_a, const int n_b, const int m_b) {
    if (n_a!=n_b || m_a!= m_b) {
        return {{}};
    }
    vvll sol;
    for (int i =0;i<n_a;i++) {
        vll tmp;
        for (int j =0;j<m_a;j++) {
            tmp.push_back(A[i][j]+B[i][j]);
        }
        sol.push_back(tmp);
    }
    return sol;
}

vvll mult_scalar_metrix(const vvll &A,const ll c, const int n_a, const int m_a) {
    vvll sol;
    for (int i =0;i<n_a;i++) {
        vll tmp;
        for (int j =0;j<m_a;j++) {
            tmp.push_back(A[i][j]*c);
        }
        sol.push_back(tmp);
    }
    return sol;
}

vvll mult_metrix(const vvll &A,const vvll &B, const int n_a, const int m_a, const int n_b, const int m_b) {
    if ( m_a!= n_b) {
        return {{}};
    }
    vvll sol;
    for (int i =0;i<n_a;i++) {
        vll tmp;
        for (int k =0;k<m_b;k++) {
            ll tmp2=0;
            for (int j =0;j<m_a;j++) {
                tmp2 += A[i][k]*B[k][j];
            }
            tmp.push_back(tmp2);
        }
        sol.push_back(tmp);
    }
    return sol;
}

vvll power_metrix(const vvll &A,const ll k, const int n, const int m) {
    if ( m!= n) {
        return {{}};
    }
    vvll sol;
    if (k ==0) {
        sol.assign(n,vector<ll>(n,0));
        for (int i=0;i<n;i++) {
            sol[i][i] = 1;
        }
        return sol;
    }
    if (k ==1) {
        return A;
    }
    const vvll ha = power_metrix(A,k/2,n,n);
    sol = mult_metrix(ha,ha, n,n,n,n);
    if (k%2 ==1) {
        sol = mult_metrix(sol, A,n,n,n,n);
    }
    return sol;
}


ll det(vvll &A, ll n) {
    ll num1, num2, det = 1, index, total = 1;
    vll temp(n + 1);
    for (int i = 0; i < n; i++) {
        index = i;
        while (index < n && A[index][i] == 0) {
            index++;
        }
        if (index == n)
        {
            continue;
        }
        if (index != i) {
            for (int j = 0; j < n; j++) {
                swap(A[index][j], A[i][j]);
            }
            det *= pow(-1, index - i);
        }
        for (int j = 0; j < n; j++) {
            temp[j] = A[i][j];
        }
        for (int j = i + 1; j < n; j++) {
            num1 = temp[i];
            num2 = A[j][i];
            for (int k = 0; k < n; k++) {
                A[j][k] = (num1 * A[j][k]) - (num2 * temp[k]);
            }
            total *= num1;
        }
    }
    for (int i = 0; i < n; i++) {
        det *= A[i][i];
    }
    return (det / total);
}


ll fast_fib(ll n) {
    if (n <2) return n;
    vvll Z = {{0},{1}};
    vvll H ={{0,1}, {1,1}};
    H = power_metrix(H,n,2,2);
    return mult_metrix(H,Z,2,2,2,1)[0][0];
}

ll linear_recurrences(ll n, vll c, vll f, ll k) {
    if (n<k) return f[n];
    vvll Z;
    vvll H(k,vll(k,0));
    for (int i =0;i<k;i++) {
        Z.push_back({f[i]});
        if (i<k-1) {
            H[i][i+1] = 1;
        }
        H[k-1][i]=c[k-i-1];
    }
    H = power_metrix(H,n,k,k);
    return mult_metrix(H,Z,k,k,k,1)[0][0];
}


vvll mult_metrix_shortest_path(const vvll &A,const vvll &B, const int n_a, const int m_a, const int n_b, const int m_b) {
    if ( m_a!= n_b) {
        return {{}};
    }
    vvll sol;
    for (int i =0;i<n_a;i++) {
        vll tmp;
        for (int j =0;j<m_b;j++) {
            ll tmp2=INF;
            for (int k =0;k<m_a;k++) {
                tmp2 = min(tmp2,A[i][k]+B[k][j]);
            }
            tmp.push_back(tmp2);
        }
        sol.push_back(tmp);
    }
    return sol;
}


vvll power_metrix_shortest_path(const vvll &A,const ll k, const int n, const int m) {
    if ( m!= n) {
        return {{}};
    }
    vvll sol;
    if (k ==0) {
        sol.assign(n,vector<ll>(n,0));
        for (int i=0;i<n;i++) {
            sol[i][i] = 1;
        }
        return sol;
    }
    if (k ==1) {
        return A;
    }
    sol = mult_metrix_shortest_path(power_metrix(A,k/2,n,n),power_metrix(A,k/2,n,n), n,n,n,n);
    if (k%2 ==1) {
        sol = mult_metrix_shortest_path(sol, A,n,n,n,n);
    }
    return sol;
}

vvll shortest_path(const vvll &V,ll k, ll n) {
    return power_metrix_shortest_path(V,k,n,n);
}


vvd transpose_metrix_d(const vvd &A, const int n, const int m) {
    vvd sol;
    for (int i =0;i<m;i++) {
        vd tmp;
        for (int j =0;j<n;j++) {
            tmp.push_back(A[j][i]);
        }
        sol.push_back(tmp);
    }
    return sol;
}

vvd sum_metrix_d(const vvd &A,const vvd &B, const int n_a, const int m_a, const int n_b, const int m_b) {
    if (n_a!=n_b || m_a!= m_b) {
        return {{}};
    }
    vvd sol;
    for (int i =0;i<n_a;i++) {
        vd tmp;
        for (int j =0;j<m_a;j++) {
            tmp.push_back(A[i][j]+B[i][j]);
        }
        sol.push_back(tmp);
    }
    return sol;
}

vvd mult_scalar_metrix_d(const vvd &A,const ll c, const int n_a, const int m_a) {
    vvd sol;
    for (int i =0;i<n_a;i++) {
        vd tmp;
        for (int j =0;j<m_a;j++) {
            tmp.push_back(A[i][j]*c);
        }
        sol.push_back(tmp);
    }
    return sol;
}

vvd mult_metrix_d(const vvd &A,const vvd &B, const int n_a, const int m_a, const int n_b, const int m_b) {
    if ( m_a!= n_b) {
        return {{}};
    }
    vvd sol;
    for (int i =0;i<n_a;i++) {
        vd tmp;
        for (int k =0;k<m_b;k++) {
            ll tmp2=0;
            for (int j =0;j<m_a;j++) {
                tmp2 += A[i][k]*B[k][j];
            }
            tmp.push_back(tmp2);
        }
        sol.push_back(tmp);
    }
    return sol;
}

vvd power_metrix_d(const vvd &A,const ll k, const int n, const int m) {
    if ( m!= n) {
        return {{}};
    }
    vvd sol;
    if (k ==0) {
        sol.assign(n,vd(n,0));
        for (int i=0;i<n;i++) {
            sol[i][i] = 1;
        }
        return sol;
    }
    if (k ==1) {
        return A;
    }
    const vvd ha = power_metrix_d(A,k/2,n,n);
    sol = mult_metrix_d(ha,ha, n,n,n,n);
    if (k%2 ==1) {
        sol = mult_metrix_d(sol, A,n,n,n,n);
    }
    return sol;
}


double det_d(vvd &A, ll n) {
    double num1, num2, det = 1, index, total = 1;
    vd temp(n + 1);
    for (int i = 0; i < n; i++) {
        index = i;
        while (index < n && A[index][i] == 0) {
            index++;
        }
        if (index == n)
        {
            continue;
        }
        if (index != i) {
            for (int j = 0; j < n; j++) {
                swap(A[index][j], A[i][j]);
            }
            det *= pow(-1, index - i);
        }
        for (int j = 0; j < n; j++) {
            temp[j] = A[i][j];
        }
        for (int j = i + 1; j < n; j++) {
            num1 = temp[i];
            num2 = A[j][i];
            for (int k = 0; k < n; k++) {
                A[j][k] = (num1 * A[j][k]) - (num2 * temp[k]);
            }
            total *= num1;
        }
    }
    for (int i = 0; i < n; i++) {
        det *= A[i][i];
    }
    return (det / total);
}


ll fast_fib_d(ll n) {
    if (n <2) return n;
    vvd Z = {{0},{1}};
    vvd H ={{0,1}, {1,1}};
    H = power_metrix_d(H,n,2,2);
    return mult_metrix_d(H,Z,2,2,2,1)[0][0];
}

ll linear_recurrences_d(ll n, vd c, vd f, ll k) {
    if (n<k) return f[n];
    vvd Z;
    vvd H(k,vd(k,0));
    for (int i =0;i<k;i++) {
        Z.push_back({f[i]});
        if (i<k-1) {
            H[i][i+1] = 1;
        }
        H[k-1][i]=c[k-i-1];
    }
    H = power_metrix_d(H,n,k,k);
    return mult_metrix_d(H,Z,k,k,k,1)[0][0];
}


vvd mult_metrix_shortest_path_d(const vvd &A,const vvd &B, const int n_a, const int m_a, const int n_b, const int m_b) {
    if ( m_a!= n_b) {
        return {{}};
    }
    vvd sol;
    for (int i =0;i<n_a;i++) {
        vd tmp;
        for (int j =0;j<m_b;j++) {
            double tmp2=INF;
            for (int k =0;k<m_a;k++) {
                tmp2 = min(tmp2,A[i][k]+B[k][j]);
            }
            tmp.push_back(tmp2);
        }
        sol.push_back(tmp);
    }
    return sol;
}


vvd power_metrix_shortest_path_d(const vvd &A,const ll k, const int n, const int m) {
    if ( m!= n) {
        return {{}};
    }
    vvd sol;
    if (k ==0) {
        sol.assign(n,vd(n,0));
        for (int i=0;i<n;i++) {
            sol[i][i] = 1;
        }
        return sol;
    }
    if (k ==1) {
        return A;
    }
    auto J =power_metrix_d(A,k/2,n,n);
    sol = mult_metrix_shortest_path_d(J,J, n,n,n,n);
    if (k%2 ==1) {
        sol = mult_metrix_shortest_path_d(sol, A,n,n,n,n);
    }
    return sol;
}

vvd shortest_path_d(const vvd &V,ll k, ll n) {
    return power_metrix_shortest_path_d(V,k,n,n);
}

ll krichoff(vector<vector<bool>> edges,vll edges_, ll n) {
    vvll A(n,vll(n,0));
    for (ll i =0;i<n;i++) {
        A[i][i] = edges_[i+1];
        for (ll j =i+1;j<n;j++) {
            if (edges[i][j] == 1) {
                A[i][j] =-1;
                A[j][i] =-1;
            }
        }
    }
    return det(A,  n);

}