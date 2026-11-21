#include <bits/stdc++.h>
using namespace std;
#include "data structure/Fenwick kth.cpp"

signed main(){
    vector<vector<int>> cases = {{5}, {7,7,7,7}, {1,2,3,4,5}, {5,4,3,2,1}, {-9,0,9,-9,9}, {LLONG_MIN,LLONG_MAX,0,LLONG_MIN,LLONG_MAX}};
    for(auto nums : cases){
        OrderStatistic os(nums);
        vector<int> brute;
        for(int x : nums){
            os.insert(x);
            brute.push_back(x);
            sort(brute.begin(),brute.end(),greater<int>());
            for(int k = 1; k <= (int)brute.size(); k++)
                if(os.kth(k) != brute[k - 1]) return 2;
        }
    }
    OrderStatistic empty({});
    if(empty.cnt != 0 || empty.bit.select(0) != 0) return 3;
    mt19937_64 rng(712367);
    for(int tc = 0; tc < 1000; tc++){
        int n = 1 + rng() % 100;
        vector<int> candidates, inserted;
        for(int i = 0; i < n; i++) candidates.push_back((long long)(rng() % 41) - 20);
        OrderStatistic os(candidates);
        vector<int> vals = candidates;
        sort(vals.begin(), vals.end());
        vals.erase(unique(vals.begin(), vals.end()), vals.end());
        vector<int> brute;
        for(int step = 0; step < n * 3; step++){
            int x = vals[rng() % vals.size()];
            os.insert(x);
            brute.push_back(x);
            sort(brute.begin(), brute.end());
            for(int k = 1; k <= (int)brute.size(); k++){
                if(os.kth(k) != brute[brute.size() - k]){
                    cerr << "Mismatch tc=" << tc << " step=" << step << " k=" << k << '\n';
                    return 1;
                }
            }
        }
    }
    puts("PASS: 1000 randomized cases, every valid k checked after each insertion");
}
