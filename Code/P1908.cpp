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
    int n,ans = 0;
    cin>>n;
    vec<int>a(n+1),vals;
    for(int i=1;i<=n;++i)cin>>a[i],vals.push_back(a[i]);
    std::sort(vals.begin(),vals.end());
    vals.erase(std::unique(vals.begin(),vals.end()),vals.end());//离散化
    BIT bit(n);
    for(int i=n;i>=1;--i){//从右往左统计逆序对
        int rk = std::lower_bound(vals.begin(),vals.end(),a[i]) - vals.begin() + 1;
        ans += bit.sum(rk-1);//之前（右侧）比 a[i] 小的数的个数
        bit.add(rk,1);
    }
    cout<<ans<<"\n";
    return 0;
}
