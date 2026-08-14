#include<bits/stdc++.h>
#define int unsigned long long
using namespace std;
constexpr int maxn = 5e6+7;
int n,m,opt,val,root=1;
namespace SGT {
    int cnt = 0;
    struct Rge {
        int l, r, mid, len;
        Rge(): l(0), r(0), mid(0), len(0) {};
        Rge(int l_, int r_): l(l_), r(r_), mid((l_ + r_) >> 1), len(r_ - l_ + 1) {};
        friend istream& operator >> (istream& stream, Rge& rg) {
            int l_, r_;
            stream >> l_ >> r_;
            rg = Rge(l_, r_);
            return stream;
        }
        friend Rge operator + (Rge a, Rge b) {
            return Rge(a.l, b.r);
        }
        friend bool operator <= (Rge a, Rge b) {
            return b.l <= a.l && a.r <= b.r;
        }
        friend pair<Rge, Rge> operator / (Rge x, const int p) {
            return make_pair(Rge(x.l, x.mid), Rge(x.mid + 1, x.r));
        }
    }inrg;
    struct Msg {
        Rge rg;
        struct Sum {
            int res, tag;
            friend Sum operator + (Sum a, Sum b) {
                return Sum{a.res + b.res, 0}; //区间合并时tag一定为0
            }
        } sum;
        friend Msg operator + (Msg a, Msg b) {
            return Msg{a.rg + b.rg, a.sum + b.sum};
        }
        void update(int val){
            sum.res += val * rg.len;
            sum.tag += val;
        }
    };
    struct Node {
        int ls, rs;
        Msg msg;
    } node[maxn];
    int create(Rge rg){
        ++cnt;
        node[cnt].msg.rg = rg;
        return cnt;
    }
    void pushdown(int k){
        auto div = node[k].msg.rg/2;
        if(!node[k].ls)node[k].ls = create(div.first);
        if(!node[k].rs)node[k].rs = create(div.second);
        if(node[k].msg.sum.tag){
            node[node[k].ls].msg.update(node[k].msg.sum.tag);
            node[node[k].rs].msg.update(node[k].msg.sum.tag);
            node[k].msg.sum.tag = 0;
        }
    }
    void mdf(int k,int val){
        if(node[k].msg.rg <= inrg){
            node[k].msg.update(val);
            return;
        }
        pushdown(k);
        if(inrg.l <= node[k].msg.rg.mid)mdf(node[k].ls,val);
        if(inrg.r > node[k].msg.rg.mid)mdf(node[k].rs,val);
        node[k].msg = node[node[k].ls].msg + node[node[k].rs].msg;
    }
    int qry(int k){
        if(node[k].msg.rg <= inrg){
            return node[k].msg.sum.res;
        }
        pushdown(k);
        int ret = 0;
        if(inrg.l <= node[k].msg.rg.mid)ret += qry(node[k].ls);
        if(inrg.r > node[k].msg.rg.mid)ret += qry(node[k].rs);
        return ret;
    }
    int get_ans(){
        return qry(root) + (inrg.l + inrg.r)*inrg.len/2;
    }
}
signed main() {
    ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
    cin>>n>>m;
    SGT::create(SGT::Rge(1,n));
    while(m--){
        cin>>opt>>SGT::inrg;
        if(opt==1){
            cin>>val;
            SGT::mdf(root,val);
        }else cout<<SGT::get_ans()<<'\n';
    }
    return 0;
}