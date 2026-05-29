#include <bits/stdc++.h>
#define int long long
using std::cin,std::cout,std::cerr;
constexpr int inf = INT_MAX;
template<typename T = int>using vec = std::vector<T>;
template<typename T = int>using vec2 = vec<vec<T>>;
signed main(){
    cin.tie(0)->sync_with_stdio(0);
    int n,m,q,u,v,t,inc = 0;cin>>n>>m>>q>>u>>v>>t;
    vec<> a(n);for(int &i:a)cin>>i;
    std::sort(a.begin(),a.end(),std::greater<int>());
    std::queue<int> s0,sa,sb;
    for(int i:a)s0.emplace(i);
    for(int i = 1;i <= m;++i){
        int fs0 = s0.size()?s0.front():-inf,fsa = sa.size()?sa.front():-inf,fsb = sb.size()?sb.front():-inf,x = std::max({fs0,fsa,fsb});
        if(x == fs0)s0.pop();
        else if(x == fsa)sa.pop();
        else if(x == fsb)sb.pop();
        x += inc;
        if(!(i % t))cout<<x<<' ';
        inc += q;
        sa.emplace(u * x / v - inc),sb.emplace(x - u * x / v - inc);//此时的inc已经包含了q故不用减掉
    }
    cout<<'\n';
    for(int i = 1;i <= n + m;++i){
        int fs0 = s0.size()?s0.front():-inf,fsa = sa.size()?sa.front():-inf,fsb = sb.size()?sb.front():-inf,x = std::max({fs0,fsa,fsb});
        if(x == fs0)s0.pop();
        else if(x == fsa)sa.pop();
        else if(x == fsb)sb.pop();
        if(!(i%t))cout<<x + inc<<' ';
    }
    return 0;
}