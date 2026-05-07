#include <bits/stdc++.h>
#define int long long
template<typename T> using vec = std::vector<T>;
constexpr int maxs = 1e5+7,maxn = 4,max_state = (1<<maxn)-1;
signed main(){
    std::cin.tie(0)->sync_with_stdio(0);//读写优化
    vec<int> c(maxn);for(auto& x:c)std::cin>>x;//读入硬币规格
    int t;std::cin>>t;//读入数据组数
    vec<int> dp(maxs,0);dp[0] = 1;
    for(const auto x:c)for(int s=1;s<maxs;++s)if(s-x>=0)dp[s] += dp[s-x];//创建 预处理DP数组
    while(t--){
        vec<int> d(maxn);for(auto& x:d)std::cin>>x;//读入限制
        int ans = 0,s;std::cin>>s;
        for(int mask=0;mask <= max_state;++mask){
            int sum = s,cnt = 0;
            for(int i=0;i<maxn;++i)if(mask&(1<<i))sum -= (d[i] + 1)*c[i],++cnt;
            if(sum>=0)ans += ((cnt%2)?-1:1)*dp[sum];//注意避免不存在的情况RE
        }
        std::cout<<ans<<'\n';
    }
    return 0;
}