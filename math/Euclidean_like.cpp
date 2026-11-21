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

    __int128 m = ((__int128)a*n + b) / c;
    if(m == 0) return res;
    res += (__int128)n * m - floorsum(c,c - b - 1,a, m - 1);
    return res;
}

__int128 floorsum_h(int a,int b,int c,int n);

// g = sum(i * floor((ai + b) / c)), i = 0..n
// a,b,n >= 0, c > 0; g 和 h 的递归相互依赖
__int128 floorsum_g(int a,int b,int c,int n){
    __int128 res = 0;
    // 拆出整除部分，分别乘上 sum(i^2) 和 sum(i)
    if(a >= c){
        res += (__int128)n * (n+1) * (2 * (__int128)n+1) / 6 * (a/c);
        a %= c;
    }

    if(b >= c){
        res += (__int128)n * (n+1) / 2 * (b/c);
        b %= c;
    }

    __int128 m = ((__int128)a*n + b) / c;
    if(m == 0) return res;
    // 换序求和：g = m*n*(n+1)/2 - (h' + f')/2
    // f',h' 的参数均为 (c,c-b-1,a,m-1)
    res += (__int128)n * (n+1) * m / 2
        - (floorsum_h(c,c - b - 1,a,m - 1) + floorsum(c,c - b - 1,a,m - 1)) / 2;
    return res;
}

// h = sum(floor((ai + b) / c)^2), i = 0..n
// a,b,n >= 0, c > 0
__int128 floorsum_h(int a,int b,int c,int n){
    __int128 res = 0;
    if(a >= c || b >= c){
        __int128 x = a/c,y = b/c;
        a %= c,b %= c;
        // 令 t_i = floor((ai+b)/c)，展开 (x*i+y+t_i)^2
        res += x * x * n * (n+1) * (2 * (__int128)n+1) / 6;
        res += x * y * n * (n+1) + y * y * (n+1);
        if(x) res += 2 * x * floorsum_g(a,b,c,n);
        if(y) res += 2 * y * floorsum(a,b,c,n);
    }

    __int128 m = ((__int128)a*n + b) / c;
    if(m == 0) return res;
    // 由 t^2 = sum(2j+1), j = 0..t-1，换序得到 h = n*m^2 - 2*g' - f'
    // f',g' 的参数均为 (c,c-b-1,a,m-1)
    res += (__int128)n * m * m - 2 * floorsum_g(c,c - b - 1,a,m - 1)
        - floorsum(c,c - b - 1,a,m - 1);
    return res;
}
