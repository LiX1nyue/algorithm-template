#include<bits/stdc++.h>
#define int long long
using namespace std;

// 三维偏序（P3810）：统计每个点在三维上均 <= 它的其他点数。
// 输入 n,k 和 n 个点 (a,b,c)，1 <= c <= k；输出答案为 0..n-1 的点数。
struct Node{
    int a,b,c,cnt = 1,ans = 0;
};
vector<Node> p;

struct Fenwick{
	const int len;
	vector<int> a;
	Fenwick(int n1) : len(n1),a(len + 3){}
#define lowbit(x) ((x) & (-x))
	void init(vector<int> &b)
	{
		for(int i = 1; i <= len; i ++)
		{
			a[i] += b[i];
			int j = i + lowbit(i);
			if(j <= len) a[j] += a[i];
		}
	}
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
	int sum(int l,int r)
	{
		return sum(r) - sum(l - 1);
	}

	//返回sum <= k的最靠右位置
	int select(int k){
		int x = 0,cur = 0;
		for(int i = 1 << __lg(len); i ; i /= 2){
			if(x + i <= len && cur + a[x+i] <= k){
				cur += a[x + i];
				x += i;
			}
		}
		return x;
	}
};

void cdq(int l,int r){
    if(l >= r) return;
    int mid = (l+r) >> 1;
    cdq(l,mid);
    cdq(mid+1,r);

    // 左右身份由初始字典序决定，只在各自区间内按 b 排序。
    auto cmp = [](const Node &x,const Node &y){ return x.b < y.b; };
    sort(p.begin()+l,p.begin()+mid+1,cmp);
    sort(p.begin()+mid+1,p.begin()+r+1,cmp);

    vector<int> c;
    for(int i = l; i <= mid; i++) c.push_back(p[i].c);
    sort(c.begin(),c.end());
    c.erase(unique(c.begin(),c.end()),c.end());
    Fenwick fw(c.size());

    int i = l;
    for(int j = mid+1; j <= r; j++){
        while(i <= mid && p[i].b <= p[j].b){
            int pos = lower_bound(c.begin(),c.end(),p[i].c) - c.begin() + 1;
            fw.add(pos,p[i].cnt);
            i++;
        }
        // 右点的 c 可能不在离散表中，取 <= p[j].c 的前缀长度。
        int pos = upper_bound(c.begin(),c.end(),p[j].c) - c.begin();
        p[j].ans += fw.sum(pos);
    }
}

signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,k;
    if(!(cin >> n >> k)) return 0;
    vector<Node> a(n);
    for(auto &u : a) cin >> u.a >> u.b >> u.c;
    sort(a.begin(),a.end(),[](const Node &x,const Node &y){
        return tie(x.a,x.b,x.c) < tie(y.a,y.b,y.c);
    });
    for(auto &u : a){
        if(!p.empty() && p.back().a == u.a && p.back().b == u.b && p.back().c == u.c)
            p.back().cnt++;
        else p.push_back(u);
    }

    cdq(0,(int)p.size()-1);
    vector<int> ans(n);
    // 每个重合点还能统计到同组的 cnt-1 个其他点。
    for(auto &u : p) ans[u.ans + u.cnt - 1] += u.cnt;
    for(int x : ans) cout << x << '\n';
    return 0;
}
