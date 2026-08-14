#include<bits/stdc++.h>
#include<bits/extc++.h>
__gnu_cxx::rope<int> str, rstr ,tmp;
int n,m,l,r;
int main() {
    std::cin.tie(0)->sync_with_stdio(0);
    std::cin >>n>>m;
    for(int i = 1;i<=n;i++)str.push_back(i),rstr.push_back(n - i + 1);
    while(m--) {
        std::cin >> l >> r;
        l--,r--;
        tmp = str.substr(l,r-l+1);//临时对象
        str.replace(l,r-l+1,rstr.substr(n-r-1,r-l+1));
        rstr.replace(n-r-1,r-l+1,tmp);
    }
    for(int x : str) std::cout << x << ' ';
    return 0;
}