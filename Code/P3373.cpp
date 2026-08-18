#include <bits/stdc++.h>
#define int long long
using std::cin;
using std::cout;
template<typename T>using vec = std::vector<T>;
int p;
struct Tag{//懒标记：先乘 mul 再加 add
    int mul = 1,add = 0;
    void overlay(const Tag& t){//标记叠加：本标记先作用，t 后作用
        add = (add*t.mul + t.add)%p;
        mul = mul*t.mul%p;
    }
};
struct Info{//节点信息：区间和
    int sum = 0;
    Info operator+(Info x) const {return {(sum + x.sum)%p};}//合并
    void apply(Tag t,int len){sum = (sum*t.mul + t.add*len)%p;}//懒标记作用到信息
};
struct SegTree{
    struct Node{
        const int l,r;
        Tag tag;
        Info info;
        Node *ls = nullptr,*rs = nullptr;
        Node(int _l,int _r):l(_l),r(_r),tag{1,0},info{0}{};
    };
    const int n;
    int ql,qr;
    std::deque<Node> pool;
    Node* newNode(const int l,const int r){return pool.emplace_back(l,r),&pool.back();};
    Node *root;
    void apply(Node* u,const Tag TAG){
        u->info.apply(TAG,u->r - u->l + 1);
        u->tag.overlay(TAG);
    }
    void pushup(Node *u){u->info = (u->ls?u->ls->info:Info{0}) + (u->rs?u->rs->info:Info{0});}
    void pushdown(Node *u){
        if(u->tag.mul != 1 || u->tag.add != 0){
            if(!u->ls)u->ls = newNode(u->l,(u->l + u->r)/2);
            if(!u->rs)u->rs = newNode((u->l + u->r)/2+1,u->r);
            apply(u->ls,u->tag);
            apply(u->rs,u->tag);
            u->tag = Tag();
        }
    }
    SegTree(const int _n):n(_n),root(newNode(1,n)){};
    void update(Node* u,const Tag t){
        if(ql <= u->l && u->r <= qr){apply(u,t);return;}
        pushdown(u);
        int m = (u->l + u->r)/2;
        if(ql <= m)update(u->ls = (u->ls?u->ls:newNode(u->l,m)),t);
        if(qr >  m)update(u->rs = (u->rs?u->rs:newNode(m+1,u->r)),t);
        pushup(u);
    }
    Info query(Node* u){
        if(ql <= u->l && u->r <= qr)return u->info;
        pushdown(u);
        int m = (u->l + u->r)/2;
        if(qr <= m)return u->ls?query(u->ls):Info{};
        if(ql >  m)return u->rs?query(u->rs):Info{};
        return (u->ls?query(u->ls):Info{}) + (u->rs?query(u->rs):Info{});
    }
};
signed main(){
    std::cin.tie(0)->sync_with_stdio(0);
    int n,m;
    cin>>n>>m>>p;
    SegTree seg(n);
    for(int i=1,x;i<=n;++i){
        cin>>x;
        seg.ql = seg.qr = i;
        seg.update(seg.root,Tag{1,x});//单点赋值等价于 乘1加x
    }
    for(int i=1,op,x,y,k;i<=m;++i){
        cin>>op>>x>>y;
        seg.ql = x,seg.qr = y;
        if(op == 1){//区间乘 k
            cin>>k;
            seg.update(seg.root,Tag{k,0});
        }else if(op == 2){//区间加 k
            cin>>k;
            seg.update(seg.root,Tag{1,k});
        }else{//区间求和
            cout<<seg.query(seg.root).sum<<"\n";
        }
    }
    return 0;
}
