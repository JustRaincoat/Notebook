#include <bits/stdc++.h>
#define int long long
using namespace std;
constexpr int mod = 9901;
int a,b,ans=1;
map<int,int> mp;
int qpow(int a,int b){//快速幂
    int ret = 1;
    for(;b;b>>=1,a*=a,a%=mod)if(b & 1)ret *= a,ret %= mod;
    return ret;
}
int sum(int b,int e){//等比数列求和
    if (b % mod == 1)return (e + 1) % mod;//质数太小，可能没有逆元
    return ((qpow(b,e+1) - 1 + mod) * qpow(b-1,mod-2)) % mod;//加mod避免负数
}
signed main(){
    cin.tie(0)->sync_with_stdio(0);
    cin>>a>>b;
    if (a == 1 || b == 0) {
        cout << 1 << endl;
        return 0;
    }
    for(int i=2;i<=a;i++)while(a%i==0)a/=i,mp[i]++;//分解质因数
    for(auto [p,e]:mp)ans *= sum(p,e * b),ans %= mod;
    cout<<ans;
    return 0;
}