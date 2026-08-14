#include <bits/stdc++.h>
#include <bits/extc++.h>
#define endl '\n'
#define int long long
using std::cin;
using std::cout;
constexpr int maxn = 1e5+7;
int n,m;
std::string o;
std::vector<int> g[maxn];
struct SGT{//Segment Tree
    struct Rge{
        int l,r,mid,len;
        Rge(){};
        Rge(int _l,int _r){l = _l,r = _r,mid = (l+r)>>1,len = r-l+1;}
        friend bool operator <=(const Rge a,const Rge b){return b.l <= a.l && a.r <= b.r;}
        std::pair<Rge,Rge> div(){return std::make_pair(Rge(l,mid),Rge(mid+1,r));}
    } inrg;
    struct Node{
        Node *ls,*rs;
        int res,tag;
        Rge rg;
        Node(){ls = rs = nullptr;};
        Node(Rge _rg){ls = rs = nullptr,rg = _rg,tag = res = 0,tag = -1;}
        void update(const bool t){res = t * rg.len,tag = t;}
        void pushdown(){
            if(tag == -1)return;
            ls->update(tag),rs->update(tag);
            tag = -1;
        }
        void pushup(){res = ls->res + rs->res;}
    };
    Node* root;
    Node* create(Rge rg){return new Node(rg);}
    void build(Node* p){
        if(p->rg.len == 1)return;
        auto d = p->rg.div();
        build(p->ls = create(d.first));
        build(p->rs = create(d.second));
    }
    void modify(Node* p,const bool type){
        if(p->rg <= inrg){
            p->update(type);
            return;
        }
        p->pushdown();
        if(inrg.l <= p->rg.mid)modify(p->ls,type);
        if(inrg.r > p->rg.mid)modify(p->rs,type);
        p->pushup();
    }
};
struct HLDT{//Heavy-Light Decomposed Tree
    struct Node{//树中的每个节点 each node in the tree
        int id;//该节点的编号 idenity
        int faid;//该节点的父节点编号 father idenity
        Node* fa;//该节点的父节点 father
        /*特别的，对于根节点 fa = nullptr faid = 0 */
        int dept;//该节点深度 depth
        int siz;//该节点的子树大小 size
        Node* hs;//该节点的重儿子 heavy_son
        int dfn;//该节点的dfs序号 DFS_number
        Node* top;//该节点所属的重链的 top 节点
        std::vector<Node*> s;//该节点的所有儿子 son
        Node(Node* u,int _id,auto& m){//新建一个编号为v的节点，指定u为他的父节点，自动添加 id 映射
            id = _id,dept = u->dept + 1,fa = u,faid = u->id,siz = 1,hs = nullptr,top = nullptr,dfn = 0;m[_id] = this;
            u->s.push_back(this);//把该节点加入到父节点的儿子队列中
        }
        Node(auto& m){id = 0,dept = 0,fa = nullptr,faid = 0,siz = 1,hs = nullptr,top = nullptr,dfn = 0;m[id] = this;}//新建一个编号为v的节点，指定u为他的父节点，自动添加 id 映射
    };
    Node* root;
    std::unordered_map<int,Node*> idc;//Id 控制器 id-controller
    int time = 0;//用于创建 DFN 的时间戳
    void dfs_build(Node* u){
        int heavy=0;
        for(const int v:g[u->id]){
            if(v == u->faid)continue;//不走回头路
            Node *vp = new Node(u,v,idc);//指向子节点的指针 v-pointer
            dfs_build(vp);//递归建树
            u->siz += vp->siz;//统计子树大小 count the size of subtree
            if(vp->siz > heavy)heavy = vp->siz,u->hs = vp;//选出重儿子 selection for heavy son
        }
    }
    void dfs_link(Node* u){
        u->dfn = ++time;
        if(u->s.empty())return;//叶子结点
        u->hs->top = u->top;dfs_link(u->hs);//重儿子在重链上，top 继承本节点。重儿子优先 dfs，保证重链dfn连续。
        for(const auto v:u->s){
            if(v == u->hs)continue;//重儿子已经走过
            v->top = v;dfs_link(v);//轻儿子的 top 节点是它自己
        }
    }
    void build(){dfs_build(root= new Node(idc));}//建树
    void link(){root->top = root;dfs_link(root);}//链接重链
    SGT sgt;
    HLDT(){
        build(),link();
        sgt.build(sgt.root = new SGT::Node(SGT::Rge(1,n)));
    }
};

int op(int x,const int type,HLDT& T){
    int oldd = T.sgt.root->res;
    HLDT::Node *u = T.idc[x];//需要操作的软件包
    if(type == 0)T.sgt.inrg = SGT::Rge(u->dfn,u->dfn + u->siz - 1),T.sgt.modify(T.sgt.root,type);//卸载 - 子树内全部卸载
    else for(;u != nullptr;u = u->top->fa)T.sgt.inrg = SGT::Rge(u->top->dfn,u->dfn),T.sgt.modify(T.sgt.root,type);//安装 - 从 u 往根节点一路安装
    return abs(oldd - T.sgt.root->res);
}
signed main(){
    cin.tie(0)->sync_with_stdio(0);
    cin>>n;
    for(int u=1,v;u<n/*0-index*/;u++)cin>>v,g[u].push_back(v),g[v].push_back(u);//为除 0 外的所有节点指定依赖包，无向边存树
    HLDT tree;//创建一颗剖分树
    cin>>m;
    for(int i=1,x;i<=m;i++){
        cin>>o>>x;//输入操作名称和要进行操作的软件包
        cout<<op(x,(o=="install"),tree)<<endl;//进行修改操作
    }
    return 0;
}