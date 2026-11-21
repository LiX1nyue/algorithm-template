#include<bits/stdc++.h>
#define int long long

using namespace std;
struct Fenwick{
	const int len;
	vector<int> a;
	Fenwick(int n1) : len(n1),a(len + 3){}
#define lowbit(x) ((x) & (-x))
	void add(int x,int c)
	{
		for(int i = x; i <= len; i += lowbit(i))
			a[i] += c;
	}
	int sum(int x)
	{
		int ret = 0;
		for(int i = x; i; i -= lowbit(i)) ret += a[i];
		return ret;
	}
	//返回sum <= k的最靠右位置，要求频次非负且k >= 0
	int select(int k){
		if(!len) return 0;
		int x = 0,cur = 0;
		for(int i = 1ll << __lg(len); i ; i /= 2){
			if(x + i <= len && cur + a[x+i] <= k){
				cur += a[x + i];
				x += i;
			}
		}
		return x;
	}
};

struct OrderStatistic{
	vector<int> val;
	Fenwick bit;
	int cnt = 0;

	static vector<int> compress(vector<int> nums){
		sort(nums.begin(),nums.end());
		nums.erase(unique(nums.begin(),nums.end()),nums.end());
		return nums;
	}

	//nums需包含所有可能插入的数，构造时集合为空
	OrderStatistic(vector<int> nums):val(compress(nums)),bit(val.size()){}

	void insert(int x){
		int p = lower_bound(val.begin(),val.end(),x) - val.begin();
		assert(p < (int)val.size() && val[p] == x);
		bit.add(p + 1,1);
		cnt ++;
	}

	//返回第k大的数，重复值分别计数，要求1 <= k <= cnt
	int kth(int k){
		assert(k >= 1 && k <= cnt);
		return val[bit.select(cnt - k)];
	}
};
