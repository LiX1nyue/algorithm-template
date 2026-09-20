#include <bits/stdc++.h>
#define int long long
using namespace std;

//  线性筛（欧拉筛）
struct EulerSieve {
    int n;                  // 筛的范围上界 [2, n]
    vector<int> primes;     // 素数表：升序存放筛出的素数，下标 0-based
                            //   primes[k-1] 即第 k 个素数（1-based 第 k 小）
    vector<int> vis;        // 合数标记：vis[i]=1 表示 i 是合数；vis[i]=0 表示 i 是 1 或素数
                            //   （vis[0]、vis[1] 均为 0，但 1 不是素数，判素时手动挡掉）
    EulerSieve(int _n) : n(_n) {
        sieve();
    }

    void sieve() {
        vis.assign(n + 1, 0);            
        for (int i = 2; i <= n; i++) {
            if (!vis[i]) primes.push_back(i);            
            for (int p : primes) {                      
                if (1ll * i * p > n) break;                        
                vis[i * p] = 1;                             
                if (i % p == 0) break;                   
            }
        }
    }

    //注意 0、1 不算素数
    bool isPrime(int x){
        return x >= 2 && x <= n && !vis[x];
    }

    // 第 k 个素数（1-based，升序；即第 k 小的素数）, 越界返回 -1
    int kthPrime(int k){
        if (k >= 1 && k <= (int)primes.size()) return primes[k - 1];
        return -1;
    }
};
