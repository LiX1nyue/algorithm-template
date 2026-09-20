#include<bits/stdc++.h>
#define int long long

using namespace std;

struct Diff2D {
    int n, m;
    vector<vector<int>> d;

    Diff2D(int _n, int _m):n(_n),m(_m),d(_n+2,vector<int>(_m+2)){}

    // 矩形 (x1,y1)-(x2,y2) 全部 +v
    void add(int x1, int y1, int x2, int y2, int v) {
        d[x1][y1] += v;
        d[x1][y2 + 1] -= v;
        d[x2 + 1][y1] -= v;
        d[x2 + 1][y2 + 1] += v;
    }

    // 前缀和还原，d[i][j] 即为最终值
    void build() {
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                d[i][j] += d[i - 1][j] + d[i][j - 1] - d[i - 1][j - 1];
    }
};

