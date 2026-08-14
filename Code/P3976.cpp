#include <bits/stdc++.h>
#include <bits/extc++.h>
#define endl '\n'
#define int long long
using std::cin;
using std::cout;
using msg = std::tuple<int,int,int,int>;
constexpr int maxn = 5e4+7,inf = 0x3f3f3f3f;
int n,m,node_w[maxn],a,b,c,d;
std::vector<int> g[maxn],dfn_id;

struct SGT{
    struct Rge{
        int l,r,mid;
        Rge(int _l = 0,int _r = 0){l = (_l < _r)?_l:_r,r = (_r > _l)?_r:_l,mid = (l + r)>>1;}
    } inrg;
    struct Node{
        Node *ls,*rs;Rge rg;
        int max,min,lv,rv;
        std::pair<bool,int> add;
        Node(Rge _rg):ls(nullptr),rs(nullptr),max(-inf),min(inf),lv(0),rv(0),rg(_rg){};
        void pushup(){
            max = std::max(ls->max,rs->max);
            min = std::min(ls->min,rs->min);
            lv = std::max({ls->lv,rs->lv,ls->max - rs->min});
            rv = std::max({ls->rv,rs->rv,rs->max - ls->min});
        }
        void apply_add(const int& k){
            max += k,min += k;
            add.first = true;
            add.second += k;//与之前可能存在的tag合并
        }
        void pushdown(){
            if(add.first)ls->apply_add(add.second),rs->apply_add(add.second),add = {0,0};
        }
    };
    Node *root;
    void build(Node* u,const std::vector<int>& m){
        if(u->rg.l == u->rg.r){
            u->max = u->min = node_w[m[u->rg.l]],u->lv = u->rv = 0;
            return;
        }
        build(u->ls = new Node(Rge(u->rg.l,u->rg.mid)),m);
        build(u->rs = new Node(Rge(u->rg.mid + 1,u->rg.r)),m);
        u->pushup();
    }
    void modify(Node *u ,const int& k,void(* apply_func)(Node*,const int&)){
        if(inrg.l <= u->rg.l && u->rg.r <= inrg.r){
            apply_func(u,k);
            return;
        }
        u->pushdown();
        if(inrg.l <= u->rg.mid)modify(u->ls,k,apply_func);
        if(inrg.r > u->rg.mid)modify(u->rs,k,apply_func);
        u->pushup();
    }
    msg query(Node *u,msg(* merge_func)(const msg&,const msg&)){
        if(inrg.l <= u->rg.l && u->rg.r <= inrg.r)return msg{u->max,u->min,u->lv,u->rv};
        u->pushdown();
        if(inrg.r <= u->rg.mid)return query(u->ls,merge_func);
        if(inrg.l > u->rg.mid)return query(u->rs,merge_func);
        return merge_func(query(u->ls,merge_func),query(u->rs,merge_func));
    }
};

struct HLDT{
    struct Node{
        int id,siz,dfn,dep;
        Node *fa,*top,*hs;
        std::vector<Node*> s;
        Node(int _id,std::vector<Node*>& m){
            id = _id,siz = 1,dfn = 0,dep = 0;
            fa = this,top = this,hs = nullptr;
            m[_id] = this;
        }
        Node(Node* _fa,int _id,std::vector<Node*>& m){
            id = _id,siz = 1,dfn = 0,dep = _fa->dep + 1;
            fa = _fa,top = nullptr,hs = nullptr;
            m[_id] = this,fa->s.push_back(this);
        }
    };
    Node* root;
    std::vector<Node*> f_id,f_dfn;
    void dfs_build(Node* u){
        int heavy = 0;
        for(int v_id:g[u->id]){
            if(v_id == u->fa->id)continue;
            auto v = new Node(u,v_id,f_id);
            dfs_build(v);
            u->siz += v->siz;
            if(v->siz > heavy)heavy = v->siz,u->hs = v;
        }
    }
    int timer;
    void dfs_link(Node* u){
        u->dfn = ++timer;f_dfn[u->dfn] = u,dfn_id[u->dfn] = u->id;
        if(!u->hs)return;
        u->hs->top = u->top;dfs_link(u->hs);
        for(auto v:u->s){
            if(v == u->hs)continue;
            v->top = v;dfs_link(v);
        }
    }
    SGT sgt;
    HLDT(int root_id,int range_n){
        f_id.resize(range_n + 7),f_dfn.resize(range_n + 7),dfn_id.resize(range_n + 7);
        root = new Node(root_id,f_id);
        dfs_build(root);
        timer = 0;
        dfs_link(root);
        sgt.build(sgt.root = new SGT::Node(SGT::Rge(1,range_n)),dfn_id);
    }
    Node* operate(int _u,int _v,std::function<void()> factor){
        auto u = f_id[_u],v = f_id[_v];
        for(;u->top != v->top;u = u->top->fa){
            if(u->top->dep < v->top->dep)std::swap(u,v);
            sgt.inrg = SGT::Rge(u->dfn,u->top->dfn);
            factor();
        }
        sgt.inrg = SGT::Rge(u->dfn,v->dfn);
        factor();
        return f_dfn[std::min(u->dfn,v->dfn)];
    }
};

msg merge(const msg& a,const msg& b){
    int maxa,maxb,mina,minb,lva,lvb,rva,rvb;
    std::tie(maxa,mina,lva,rva) = a,std::tie(maxb,minb,lvb,rvb) = b;
    if(maxa == -inf)return b;//空值直接merge会出错，merge遇到inf返回非空值
    if(maxb == -inf)return a;//空值直接merge会出错，merge遇到inf返回非空值
    return msg{
        std::max(maxa,maxb),
        std::min(mina,minb),
        std::max({lva,lvb,maxa - minb}),
        std::max({rva,rvb,maxb - mina})
    };
}

void apply_add(SGT::Node* u,const int& k){u->apply_add(k);}//调用apply_add(成员函数)

signed main(){
    cin.tie(0)->sync_with_stdio(0);
    cin>>n;
    for(int i=1;i<=n;i++)cin>>node_w[i];
    for(int i=1,u,v;i<=n-1;i++){
        cin>>u>>v;
        g[u].push_back(v),g[v].push_back(u);
    }
    HLDT Tree(1,n);
    
    
    cin>>m;
    for(int i=1,u_,v_,w;i<=m;i++){
        cin>>u_>>v_>>w;
        auto LCA = Tree.operate(u_,v_,[&](){Tree.sgt.modify(Tree.sgt.root,w,apply_add);}),
            u = Tree.f_id[u_],v = Tree.f_id[v_];
        msg Lmsg{-inf,inf,0,0},Rmsg{-inf,inf,0,0},tot_msg;
        
        for(;u->top != LCA->top;u = u->top->fa){
            Tree.sgt.inrg = SGT::Rge(u->dfn,u->top->dfn);
            std::tie(a,b,c,d) = Tree.sgt.query(Tree.sgt.root,merge);
            Lmsg = merge(Lmsg,msg{a,b,d,c});//上行方向，u->LCA，dfn从小到大，需要翻转
        }
        Tree.sgt.inrg = SGT::Rge(u->dfn,LCA->dfn);
        std::tie(a,b,c,d) = Tree.sgt.query(Tree.sgt.root,merge);
        Lmsg = merge(Lmsg,msg{a,b,d,c});//上行方向，u->LCA，dfn从小到大，需要翻转

        for(;v->top != LCA->top;v = v->top->fa){
            Tree.sgt.inrg = SGT::Rge(v->dfn,v->top->dfn);
            std::tie(a,b,c,d) = Tree.sgt.query(Tree.sgt.root,merge);
            Rmsg = merge(msg{a,b,c,d},Rmsg);//下行方向，LCA->v，dfn从小到大，无需翻转
        }
        Tree.sgt.inrg = SGT::Rge(v->dfn,LCA->dfn);
        std::tie(a,b,c,d) = Tree.sgt.query(Tree.sgt.root,merge);
        Rmsg = merge(msg{a,b,c,d},Rmsg);//下行方向，LCA->v，dfn从小到大，无需翻转

        cout<<std::get<3>(merge(Lmsg,Rmsg))<<endl;
    }
    return 0;
}