#include <bits/stdc++.h>
#define int long long
using namespace std;
constexpr int maxl = 32;//29 = __lg(1e9+7);
unordered_map<string,int> mp = {{"AND",1},{"OR",2},{"XOR",3}};
int n,m;
string o;
bitset<maxl> a,b,c;
signed main(){
    cin.tie(0)->sync_with_stdio(0);
    b.flip();//全1
    cin>>n>>m;
    for(int i=1,t;i<=n;i++){
        cin>>o>>t;
        switch(mp[o]){
            case 1:a&=t,b&=t;break;
            case 2:a|=t,b|=t;break;
            case 3:a^=t,b^=t;break;
        }
    }
    int cur = 0; // 当前初始攻击力
    for (int i = 29; i >= 0; i--) {
        if (a[i]) {
            // 初始值这一位为 0 时，最终结果为 1，直接取
            c[i] = 1;
        } else if (b[i]) {
            // 初始值这一位为 1 时，最终结果为 1
            // 检查是否可以选择这一位为 1
            if ((cur | (1 << i)) <= m) {
                cur |= (1 << i);
                c[i] = 1;
            } else {
                c[i] = 0;
            }
        } else {
            c[i] = 0;
        }
    }
    cout<<c.to_ullong();
    return 0;
}