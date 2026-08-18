#include <bits/stdc++.h>
#define int long long
using std::cin;
using std::cout;
template<typename T>using vec = std::vector<T>;
struct BIT{
    const int n;vec<int>c;
    BIT(int _n):n(_n),c(n+1,0){};
    int lowbit(int x){return x&-x;}
    void add(int x,int v){
        for(;x<=n;x+=lowbit(x))c[x] += v;
    }
    int sum(int x){
        int res = 0;
        for(;x>0;x-=lowbit(x))res += c[x];
        return res;
    }
};
signed main(){
    std::cin.tie(0)->sync_with_stdio(0);
    int n,m;
    cin>>n>>m;
    BIT bit(n);
    for(int i=1,x;i<=n;++i)cin>>x,bit.add(i,x);
    for(int i=1,op,x,y;i<=m;++i){
        cin>>op>>x>>y;
        if(op == 1)bit.add(x,y);
        else cout<<bit.sum(y) - bit.sum(x-1)<<"\n";
    }
    return 0;
}
