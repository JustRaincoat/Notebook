#include<bits/stdc++.h>
#define int long long
#define double long double
using std::cin,std::cout,std::cerr;
template<typename T>using vec = std::vector<T>;
template<typename T>using vec2 = vec<vec<T>>;
signed main(){
    cin.tie(0)->sync_with_stdio(0);
    int n;
    cin>>n;
    vec<int> a(n),pre(n);
    for(int i=0;i<n;++i){
        cin>>a[i];
        pre[i] = (i?pre[i-1]:0) + a[i];
    }
    vec<int> L(n),R(n);
    std::stack<int> s;
    for(int i=0;i<n;++i){
        while(s.size() && a[s.top()]>a[i])R[s.top()] = i,s.pop();
        if(s.size())L[i] = s.top();
        else L[i] = -1;
        s.push(i);
    }
    while(s.size())R[s.top()] = n,s.pop();
    int ans = 0;
    for(int i=0;i<n;++i)ans = std::max(ans,(pre[R[i]-1] - ((L[i]>=0)?pre[L[i]]:0)) * a[i]);
    cout<<ans;
    return 0;
}