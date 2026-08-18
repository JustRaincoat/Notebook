#include <bits/stdc++.h>
#include <bits/extc++.h>
#define int long long
#define double long double
using std::cin;
using std::cout;
using std::cerr;
using pii = std::pair<int,int>;
template<typename T1,typename T2>using map = __gnu_pbds::gp_hash_table<T1,T2>;
template<typename T>using vec = std::vector<T>;
template<typename T>using vec2 = vec<vec<T>>;
struct City{int w,col;};
struct Tag{
    int set = 0;bool setTag = false;
    void overlay(const Tag& t){set = t.set,setTag = true;}
};
struct Info{
    int sum = 0,max = 0;
    Info operator+(Info x) const {return {sum + x.sum,std::max(max,x.max)};}//merge Info
    void apply(Tag t){sum = t.set,max = t.set;}
};
struct SegTree{
    struct Node{
        const int l,r;
        Tag tag;
        Info info;
        Node *ls = nullptr,*rs = nullptr;
        Node(int _l,int _r):l(_l),r(_r),tag{0,false},info{0,0}{};
    };
    const int n;
    int ql,qr;
    std::deque<Node> pool;
    Node* newNode(const int l,const int r){return pool.emplace_back(l,r),&pool.back();};
    Node *root;
    void apply(Node* p,const Tag TAG){
        p->info.apply(TAG);
        p->tag.overlay(TAG);
    }
    void pushup(Node *p){p->info = (p->ls?p->ls->info:Info{0,0}) + (p->rs?p->rs->info:Info{0,0});}
    void pushdown(Node *p){
        if(p->tag.setTag){
            if(!p->ls)p->ls = (pool.emplace_back(p->l, (p->l + p->r)/2),&pool.back());
            if(!p->rs)p->rs = (pool.emplace_back((p->l + p->r)/2+1,p->r),&pool.back());
            apply(p->ls,p->tag);
            apply(p->rs,p->tag);
            p->tag = Tag();
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
struct HLDT{
    struct Node{
        using ctrl = map<int,Node*>;
        int id,faid,sz,dept,dfn;
        Node *fa,*hs,*top;
        vec<Node*>ch;
        Node(Node *f,int _id,ctrl& c):id(_id),faid(f->id),sz(1),dept(f->dept+1),dfn(0),fa(f),hs(nullptr),top(nullptr){
            c[id]=this;
            f->ch.push_back(this);
        };
        Node(int _id,ctrl& c):id(_id),faid(0),sz(1),dept(0),dfn(0),fa(nullptr),hs(nullptr),top(this){
            c[id]=this;
        }
    };
    const int n;
    const vec2<int>& g;
    std::deque<Node>pool;
    int time = 0;
    Node::ctrl idc;//id-control
    Node *root;
    Node* newNode(Node *f,int _id){return pool.emplace_back(f,_id,idc),&pool.back();};
    Node* newNode(int _id){return pool.emplace_back(_id,idc),&pool.back();};
    void dfs_build(Node *u){
        int heavy = 0;
        for(const int& vid:g[u->id]){
            if(vid == u->faid)continue;
            Node *v = newNode(u,vid);
            dfs_build(v);
            u->sz += v->sz;
            if(v->sz > heavy)heavy = v->sz,u->hs = v;
        }
    }
    void dfs_link(Node *u){
        u->dfn = ++time;
        if(u->ch.empty())return;
        u->hs->top = u->top;dfs_link(u->hs);
        for(Node *v:u->ch)if(v!=u->hs){v->top = v;dfs_link(v);}
    }
    HLDT(int _n,const vec2<int>& _g):n(_n),g(_g){
        root = newNode(1);
        dfs_build(root);
        dfs_link(root);
    }
    Info query_on_path(int uid,int vid,std::function<Info(int,int)> func){
        Node *u = idc[uid],*v = idc[vid];
        Info info;
        for(;u->top != v->top;u = u->top->fa){
            if(u->top->dept < v->top->dept)std::swap(u,v);
            info = info + func(u->top->dfn,u->dfn);
        }
        if(u->dfn > v->dfn)std::swap(u,v);
        return info + func(u->dfn,v->dfn);
    }
};
signed main(){
    std::cin.tie(0)->sync_with_stdio(0);
    int n,q;
    cin>>n>>q;
    vec<City>city(n+1);
    vec2<int>g(n+1);
    for(int i=1;i<=n;++i)
        cin>>city[i].w>>city[i].col;
    for(int i=1,u,v;i<=n-1;++i){
        cin>>u>>v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    HLDT tree(n,g);
    map<int,SegTree*>sgtr;
    for(int i=1;i<=n;++i){
        int col = city[i].col;
        if(sgtr.find(col)==sgtr.end())sgtr.insert({col,new SegTree(n)});
        SegTree& seg = *sgtr.find(col)->second;
        seg.ql = seg.qr = tree.idc[i]->dfn;
        seg.update(seg.root,Tag{city[i].w,true});
    }
    for(int i=1;i<=q;++i){
        std::string op;
        int x,y;
        cin>>op>>x>>y;
        if(op == "CC"){
            int oldd = city[x].col,neww = y;
            city[x].col = y;
            if(sgtr.find(neww) == sgtr.end())sgtr.insert({neww,new SegTree(n)});
            SegTree& seg1 = *sgtr.find(oldd)->second;
            SegTree& seg2 = *sgtr.find(neww)->second;
            int pos = tree.idc[x]->dfn;
            seg1.ql = seg1.qr = pos;
            seg2.ql = seg2.qr = pos;
            seg1.update(seg1.root,Tag{0,false});
            seg2.update(seg2.root,Tag{city[x].w,true});
        }else if(op == "CW"){
            city[x].w = y;
            SegTree& seg = *sgtr.find(city[x].col)->second;
            seg.ql = seg.qr = tree.idc[x]->dfn;
            seg.update(seg.root,Tag{y,true});
        }else{
            Info info = tree.query_on_path(x,y,[&](int l,int r){
                SegTree& seg = *sgtr.find(city[x].col)->second;
                seg.ql = l,seg.qr = r;
                return seg.query(seg.root);
            });
            cout<<(op[1] == 'S'?info.sum:info.max)<<"\n";
        }
    }
    return 0;
}
