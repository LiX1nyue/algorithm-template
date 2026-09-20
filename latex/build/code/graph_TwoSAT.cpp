#include<bits/stdc++.h>
#define int long long

using namespace std;

struct SCC{
	int n;
	vector<vector<int>> &v;
	vector<int> stk;
	vector<int> dfn,low,bel;
	int cur,cnt;
	
	SCC(vector<vector<int>> &g) : v(g){
		n = g.size() - 1;
		stk.resize(n+1);
		dfn.resize(n+1);
		low.resize(n+1);
		bel.resize(n+1);
		cur = cnt = 0;
	}
	
	void tarjan(int u){
		dfn[u] = low[u] = ++ cur;
		stk.push_back(u);
		for(int i : v[u]){
			if(!dfn[i]){
				tarjan(i);
				low[u] = min(low[u],low[i]);
			}else if(!bel[i]){
				low[u] = min(low[u],dfn[i]);
			}
		}
		
		if(dfn[u] == low[u]){
			cnt ++;
			int x;
			do{
				x = stk.back();
				bel[x] = cnt;
				stk.pop_back();
			}while(x != u);
		}
	}
	
	vector<int> work(){
		for(int i = 1; i <= n; i ++){
			if(!dfn[i]) tarjan(i);
		}
		return bel;
	}

};

struct TwoSAT{
	int n;                       // 变量个数
	vector<vector<int>> g;       // 蕴含图, 节点 1..2n
	vector<int> bel;             // SCC 编号
	bool ok = true;
	
	TwoSAT(int n_): n(n_), g(n_*2 + 1) {}
	
	// 加入子句 (a in {0,1}) ∨ (b in {0,1}): na/nb=0 取原变量, 1 取反
	// 变量 a 的节点为 a, 其否定为 a+n; 用 na*n 作偏移
	void add_or(int a, int na, int b, int nb){
		g[a + !na * n].push_back(b + nb * n);   // ¬a -> b
		g[b + !nb * n].push_back(a + na * n);   // ¬b -> a
	}
	// 加入单变量约束 a = val (即加入单位子句 a 或 ¬a)
	void add_eq(int a, int val){
		add_or(a, !val, a, !val);
	}
	// 加入异或约束 a ⊕ b = val (val=1 两值不同, val=0 两值相同)
	void add_xor(int a, int b, int val){
		add_or(a, 0, b, !val);
		add_or(a, 1, b, val);
	}

	// 求解, 返回 {xi} 赋值 (0/1), 无解返回 {-1}
	vector<int> solve(){
		SCC scc(g);
		bel = scc.work();
		for(int i = 1; i <= n; i++){
			if(bel[i] == bel[i + n]){
				ok = false;
				return {-1};
			}
		}
		vector<int> res(n + 1);
		// 先弹出的 SCC 编号小 = 拓扑序靠后; 选靠后的字面值为真
		for(int i = 1; i <= n; i++){
			res[i] = bel[i] < bel[i + n] ? 1 : 0;
		}
		return res;
	}
};

/*
==================== 常见加边方法速查 ====================
所有约束最终拆成子句调用 add_or / add_eq 即可。

语义                       等价 2-CNF 子句               add_or / add_eq 写法
--------------------------------------------------------------
x = 1                     x                          add_eq(x, 1)
x = 0                     ¬x                         add_eq(x, 0)
x ∨ y                     x ∨ y                      add_or(x,0, y,0)
¬x ∨ y                    ¬x ∨ y                     add_or(x,1, y,0)
x → y                     ¬x ∨ y                     add_or(x,1, y,0)
x ⇔ y (双向蕴含)          (¬x∨y) ∧ (¬y∨x)            add_or(x,1,y,0); add_or(y,1,x,0)
x ⊕ y = 1                 (x∨y) ∧ (¬x∨¬y)            add_or(x,0,y,0); add_or(x,1,y,1)
x ⊕ y = 0 (同真同假)      (x∨¬y) ∧ (¬x∨y)            add_or(x,0,y,1); add_or(x,1,y,0)
¬(x ∧ y)  (NAND)          ¬x ∨ ¬y                     add_or(x,1, y,1)
¬(¬x ∧ ¬y) (至少一真)     x ∨ y                       add_or(x,0, y,0)
--------------------------------------------------------------
高阶约束 (拆成多条子句):
- 蕴含 x→y          单向, 仅加 1 条边  g[lit(x,0)].push_back(lit(y,0))
- 至多选一 (AMO)    两两 NAND  add_or(xi,1, xj,1)            O(k^2) 条
- 恰好选一 (EO)     AMO + ∨(全体)  add_or(xi,0, xj,0) ...    O(k^2) 条
- k 中选 m (cardinality) 需分组枚举或引入辅助变量, 也可用前缀优化到 O(k)

避坑提示:
1. 比较拓扑序时只比较 同一变量 的 x 与 ¬x 所在 SCC, 不要跨变量比较
2. Tarjan 求出的 SCC 编号是反向拓扑序, 编号小 = 先出栈 = 拓扑靠后, 选靠后的为真
3. 并查集/二分图染色可在以下情况替代 2-SAT, 常数更小:
     . 所有约束均为 XOR      (XOR-SAT, 每个连通分量 2 种赋值)
     . 所有约束均为同真同假   (等价类并查集, 节点带权)
   复杂度: 2-SAT O(V+E) ; 并查集 O((V+E)α) 但实现代码少很多
============================================================
*/