#include <bits/stdc++.h>
#define int unsigned long long
using namespace std;
struct SGT{
    struct Rge{
        int l,r,mid,len;
        Rge(){l=r=mid=len=0;}
        Rge(int l_,int r_){
            l = l_, r = r_;
            mid = (l + r) >> 1;
            len = (r - l + 1);
        }
        friend bool operator <= (const Rge& a,const Rge& b){
            return b.l <= a.l && a.r <= b.r;
        }
    } inrg;
    struct Node{
        Node *ls,*rs;
        Rge rg;
        int res,lazy;
        Node(const Rge& _rg){
            ls = rs = nullptr;
            rg = _rg,lazy = 0;
            // 初始 a[i] = i
            res = (rg.l + rg.r) * rg.len / 2;
        }
        pair<Rge,Rge> divide(){
            return {Rge(rg.l, rg.mid), Rge(rg.mid+1, rg.r)};
        }
    };
    Node* root;
    Node* create(const Rge& rg){return new Node(rg);}
    void pushup(Node* k){
        k->res = 0;
        if(k->ls) k->res += k->ls->res;
        if(k->rs) k->res += k->rs->res;
    }
    void pushdown(Node* k){
        if(k->rg.l == k->rg.r) return;
        if(!k->ls){
            auto pr = k->divide();
            k->ls = create(pr.first);
        }
        if(!k->rs){
            auto pr = k->divide();
            k->rs = create(pr.second);
        }
        if(k->lazy){
            int v = k->lazy;
            k->ls->res += k->ls->rg.len * v;
            k->rs->res += k->rs->rg.len * v;
            k->ls->lazy += v;
            k->rs->lazy += v;
            k->lazy = 0;
        }
    }
    void mdy(Node* k, const int& val){
        if(k->rg <= inrg){
            k->res += k->rg.len * val;
            k->lazy += val;
            return;
        }
        pushdown(k);
        if(inrg.l <= k->rg.mid) mdy(k->ls, val);
        if(inrg.r >  k->rg.mid) mdy(k->rs, val);
        pushup(k);
    }
    int qry(Node* k){
        if(!k) return 0;
        if(k->rg <= inrg) return k->res;
        pushdown(k);
        int ans = 0;
        if(inrg.l <= k->rg.mid) ans += qry(k->ls);
        if(inrg.r >  k->rg.mid) ans += qry(k->rs);
        return ans;
    }
} seg;
signed main(){
    cin.tie(0)->sync_with_stdio(0);
    int n,m;
    cin>>n>>m;
    seg.root = seg.create(SGT::Rge(1, n));
    while(m--){
        int op,l,r,k;
        cin>>op>>l>>r;
        if(op==1){
            cin>>k;
            seg.inrg = SGT::Rge(l, r);
            seg.mdy(seg.root, k);
        }else{
            seg.inrg = SGT::Rge(l, r);
            cout<<seg.qry(seg.root)<<'\n';
        }
    }
    return 0;
}
