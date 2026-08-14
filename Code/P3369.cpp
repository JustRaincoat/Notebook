#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;
typedef pair<int,int> pii;
tree<pii,null_type,less<pii>,rb_tree_tag,tree_order_statistics_node_update> s;
int n,o,x;
auto it = s.begin();
signed main(){
	ios::sync_with_stdio(0),cin.tie(0),cout.tie(0);
	cin>>n;
	for(int i=1;i<=n;i++){
		cin>>o>>x;
		if(o==1)s.insert({x,i});
		if(o==2)if((it = s.lower_bound({x,0}))->first==x)s.erase(it);
		if(o==3)cout<<s.order_of_key({x,0})+1<<"\n";
		if(o==4)cout<<s.find_by_order(x-1)->first<<"\n";
		if(o==5)cout<<prev(s.lower_bound({x,0}))->first<<"\n";
		if(o==6)cout<<s.upper_bound({x,n+1})->first<<"\n";
	}
}