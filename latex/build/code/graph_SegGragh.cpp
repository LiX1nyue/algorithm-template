#include<bits/stdc++.h>
#define int long long

using namespace std;
typedef pair<int,int> PII;
// 线段树建图：实点编号 1..n，线段树节点 o 对应辅助点 n+o
// 用途：addrange(u,l,r) 一次加 "u -> [l,r] 内所有实点" 的边，只需 O(log n) 条
struct SegGraph{

	int n,m,tot;
	vector<vector<int>> v;

	SegGraph(int n_):n(n_){
		m = 1; 
		while(m < n) m <<= 1;          // m：线段树区间长度（补齐到 2 的幂）
		tot = n + 2 * m;               // 辅助点最多占 2m 个（堆式编号最大到 2m）
		v.assign(tot+1,{});
		build(1,1,m);                  // 建树内 辅助点->辅助点 / 叶子->实点 的骨架边
	}

	void addedge(int x,int y){
		v[x].push_back(y);
	}

	// 节点 o 管区间 [l,r]：向两个儿子连边；叶子位置(若在 1..n 内)连向实点 l
	void build(int o,int l,int r){
		int id = n + o;
		if(l == r){
			if(l <= n){
				addedge(n + o, l);
			}
			return;
		}
		int mid = (l + r) >> 1;
		addedge(n + o, n + 2*o);
        addedge(n + o, n + 2*o + 1);
		build(2*o,l,mid);
		build(2*o+1,mid+1,r);
	}

	// u 向区间 [L,R]（当前节点管 [l,r]）连边：完全覆盖则连向本辅助点，否则下递归
	void addrange(int u,int L,int R,int o,int l,int r){
		if(L <= l && r <= R){
			addedge(u,n+o);
			return;
		}
		int mid = (l + r) >> 1;
		if(L <= mid) addrange(u,L,R,2*o,l,mid);
		if(R > mid) addrange(u,L,R,2*o+1,mid+1,r);
	}

	// 对外接口：u -> 实点 [l,r]（自动跳过空区间）
	void addrange(int u,int l,int r){
		if(l > r) return;
		addrange(u,l,r,1,1,m);
	}
	
	vector<vector<int>>& getGragh(){
		return v;
	}
};