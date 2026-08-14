#include <bits/stdc++.h>
#define int long long
#define double long double
constexpr int mod = 998244353;
template<typename T>T reverse(const T& c){return T(std::rbegin(c), std::rend(c));}//使用反向迭代器
int n,m,q;
std::vector<int> a,mul,cnt;
using Node = std::pair<int,int>;
std::vector<Node> func;
std::vector<std::vector<int>> g,suf;
const std::vector<int> topo(){//返回topo排序结果
    std::vector<int> in_degree(m+1,0),res;
    std::queue<int> q;
    for(int u=0;u<=m;u++)for(const int v:g[u])in_degree[v]++;
    for(int u=0;u<=m;u++)if(!in_degree[u])q.push(u),res.push_back(u);
    while(q.size()){
        const int u = q.front();q.pop();
        for(const int v:g[u]){
            in_degree[v]--;
            if(!in_degree[v])q.push(v),res.push_back(v);
        }
    }
    return res;
}
signed main(){
    std::cin.tie(0)->sync_with_stdio(0);
    {//输入初始值
        std::cin>>n;
        a.resize(n+1);
        for(int i=1;i<=n;i++)std::cin>>a[i];
    }
    {//输入/初始化实例函数
        std::cin>>m;
        func.resize(m+1),g.resize(m+1);
        int p,v,c,t;
        for(int i=1;i<=m;i++){
            std::cin>>t;
            if(t == 1){//单点加函数
                std::cin>>p>>v;
                func[i] = {p,v};
            }else if(t == 2){//全局乘函数
                std::cin>>v;
                func[i] = {-1,v};
            }else if(t == 3){//调用函数
                std::cin>>c;
                for(int j=1,f;j<=c;j++){
                    std::cin>>f;
                    g[i].push_back(f);
                }
                func[i] = {-2,0};
            }
        }
    }
    {//输入/初始化主函数
        cnt.resize(m+1);
        std::cin>>q;
        for(int i=1,f;i<=q;i++){
            std::cin>>f;
            g[0].push_back(f);
        }
        cnt[0] = 1;//主程序执行一次
        func[0] = {-2,0};
    }
    const std::vector<int> t_order = topo();//topo排序
    {//首次更新，求解每个函数运行后对全局乘数的贡献 以及 每个调用函数的后缀积
        mul.resize(m+1,1),suf.resize(m+1);
        const auto rt_order = reverse(t_order);
        for(const int u:rt_order){//逆topo序列更新
            auto [pos,val] = func[u];
            if(pos == -2){//调用函数
                suf[u].resize(g[u].size() + 1);
                suf[u][g[u].size()] = 1;//后缀积初始化
                for(int iter_v = g[u].size()-1,v;iter_v>=0;--iter_v)
                    v = g[u][iter_v],suf[u][iter_v] = suf[u][iter_v+1] * mul[v],suf[u][iter_v] %= mod;
                mul[u] = suf[u][0];
            }else if(pos == -1)mul[u] = val;//全局乘函数
        }
    }
    {//第二次更新，求解单点加函数的工作次数（加权）
        for(const int u:t_order)//正topo序列更新
            if(func[u].first == -2){//类型3：调用函数
                int cur = 1;
                for(int iter_v = g[u].size()-1,v;iter_v>=0;--iter_v){
                    v = g[u][iter_v],cnt[v] = (cnt[v] + cnt[u] * suf[u][iter_v + 1])%mod;//可能有多个调用源
                }
            }
    }
    {//落实操作并输出
        for(int i=1;i<=n;i++)a[i] *= mul[0],a[i]%= mod;//应用全局
        for(int i=1;i<=m;i++)if(func[i].first > 0)a[func[i].first] += func[i].second * cnt[i],a[func[i].first] %= mod;//应用单点
        for(int i=1;i<=n;i++)std::cout<<(a[i]%mod + mod) % mod<<' ';//取模输出（注意负数）
    }
    return 0;
}