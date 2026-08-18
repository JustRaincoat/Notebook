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
    BIT bit(n+1);//维护差分数组
    for(int i=1,x,last = 0;i<=n;++i){
        cin>>x;
        bit.add(i,x-last);//差分建树
        last = x;
    }
    for(int i=1,op,x,y,k;i<=m;++i){
        cin>>op;
        if(op == 1){
            cin>>x>>y>>k;
            bit.add(x,k),bit.add(y+1,-k);//区间 [x,y] 加 k
        }else{
            cin>>x;
            cout<<bit.sum(x)<<"\n";//单点查询 = 差分前缀和
        }
    }
    return 0;
}
