#include <bits/stdc++.h>
#define int long long
using namespace std;
using pii = pair<int,int>;
constexpr int n = 5,maxn = 8;
int T;
bool g[maxn][maxn],cg[maxn][maxn];
char c;
pii d[5] = {{0,0},{0,1},{1,0},{0,-1},{-1,0}} ;
void solve(){
    int ans = INT_MAX;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            cin>>c;
            g[i][j] = (c=='1');
        }
    }
    for(int mask = 0,step = 0;mask < (1<<n);step = 0,mask++){
        auto rev = [&step](int i,int j){
            for(auto [dx,dy]:d)cg[i+dx][j+dy] = !cg[i+dx][j+dy];
            step++;
        };
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                cg[i][j] = g[i][j];//还原cg矩阵
        for(int i=0,pos;i<n;i++)
            if((1<<i) & mask)pos = i+1,rev(1,pos);
        for(int i=2,flag = 1;i<=n && flag;i++)
            for(int j=1;j<=n;j++){
                if(!cg[i-1][j])rev(i,j);
                if(step > 6)step = INT_MAX,flag=0;
            }
        for(int i=1,flag = 1;flag && i<=n;i++)if(!cg[n][i])step = INT_MAX,flag = 0;
        ans = min(ans,step);
    }
    if(ans == INT_MAX)cout<<-1<<'\n';
    else cout<<ans<<'\n';
}
signed main(){
    cin.tie(0)->sync_with_stdio(0);
    cin>>T;
    while(T--)solve();
    return 0;
}