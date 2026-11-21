#include<bits/stdc++.h>
#define int long long

using namespace std;

struct Info{
    int l, r;
    int mx,maxIdx;
};

Info operator+(const Info &l,const Info &r){
    Info ret = {l.l,r.r};
    if(l.mx > r.mx){
        ret.mx = l.mx;
        ret.maxIdx = l.maxIdx;
    }else{
        ret.mx = r.mx;
        ret.maxIdx = r.maxIdx;
    }
    return ret;
}

struct SegTree{
    int n;
    vector<Info> seg;
    vector<int> tag;
    
    SegTree(int _n):n(_n+1){
		seg.resize(n << 2 | 2);
		tag.resize(n << 2 | 2);
        work();
	}

    void add(int np,int v){
        seg[np].mx += v;        
    }

    void work(){
        auto build = [&](auto &&self,int np,int l,int r) -> void{
            if(l == r){
                seg[np] = {l,r,0ll,l}; 
            }else{
                seg[np] = {l, r};
                int mid = l + r >> 1;
                self(self,np << 1, l, mid),self(self,np << 1 | 1,mid + 1,r);
                pull(np);
            }
        };
        build(build,1,1,n);
    }

    void pull(int np){
        seg[np] = seg[np << 1] + seg[np << 1 | 1];
    }

    void push(int np){
        add(np << 1,tag[np]),add(np << 1 | 1,tag[np]);
        tag[np << 1] += tag[np],tag[np << 1 | 1] += tag[np];
        tag[np] = 0;
    }

    void modify(int np,int x,int v){
        if(seg[np].l == seg[np].r){
            add(np,v);
        }
        else{
            int mid = seg[np].l + seg[np].r >> 1;
            push(np);
            if(x <= mid) modify(np << 1,x,v);
            else modify(np << 1 | 1,x,v);
            pull(np);
        }
    }

    void rangeadd(int np,int l,int r,int v){
        if(seg[np].l >= l && seg[np].r <= r){
            add(np,v);
            tag[np] += v;
        }else{
            push(np);
            int mid = seg[np].l + seg[np].r >> 1;
            if(l <= mid) rangeadd(np << 1,l,r,v);
            if(r > mid) rangeadd(np << 1 | 1,l,r,v);
            pull(np);
        }
    }

    Info query(int np,int l,int r){
        if(seg[np].l >= l && seg[np].r <= r) return seg[np];
        push(np);
        int mid = seg[np].l + seg[np].r >> 1;
        if(r <= mid) return query(np << 1,l,r);
        if(l > mid) return query(np << 1 | 1,l,r);
        return query(np << 1,l,r) + query(np << 1 | 1,l,r);
    }

    bool check(Info x){
    }

    int max_right(int np,int l){
        Info cur = {l,l,LLONG_MIN,l};
        auto find = [&](auto &&self,int p) -> int{
            if(seg[p].r < l) return l - 1;
            if(seg[p].l >= l && check(cur + seg[p])){
                cur = cur + seg[p];
                return seg[p].r;
            }
            if(seg[p].l == seg[p].r) return seg[p].l - 1;
            push(p);
            int left = self(self,p << 1);
            if(left < seg[p << 1].r) return left;
            return self(self,p << 1 | 1);
        };
        return find(find,1);
    }

};
