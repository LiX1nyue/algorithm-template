#include<bits/stdc++.h>
#define int long long
using namespace std;
// floor((ai + b) / c)
__int128 floorsum(int a,int b,int c,int n){
    __int128 res = 0;
    if(a >= c){
        res += (__int128)n * (n+1) * (a / c) / 2;
        a %= c; 
    }

    if(b >= c){
        res += (__int128)(n+1) * (b/c);
        b %= c;
    }

    __int128 m = (__int128)(a*n + b) / c;
    if(m == 0) return res;
    res += (__int128)n * m - floorsum(c,c - b - 1,a, m - 1);
    return res;
}