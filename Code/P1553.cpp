#include<bits/stdc++.h>
std::string str;
int main(){
    std::cin>>str;
    if(str.find('.')!=std::string::npos){
        int idx = str.find('.');
        std::string a = str.substr(0,idx),b = std::to_string(std::stoull(str.substr(idx+1)));
        std::reverse(a.begin(),a.end()),std::reverse(b.begin(),b.end());
        std::cout<<std::stoull(a)<<'.'<<b;
    }else if(str.find('/')!=std::string::npos){
        int idx = str.find('/');
        std::string a = str.substr(0,idx),b = str.substr(idx+1);
        std::reverse(a.begin(),a.end()),std::reverse(b.begin(),b.end());
        std::cout<<std::stoull(a)<<'/'<<std::stoull(b);
    }else if(str.back() == '%'){
        std::string a = str.substr(0,str.length()-1);
        std::reverse(a.begin(),a.end());
        std::cout<<std::stoull(a)<<'%';
    }else{
        std::reverse(str.begin(),str.end());
        std::cout<<std::stoull(str);
    }
}