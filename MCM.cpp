// Original version implemented in 06/08/23
// Need to be simplified

#include <bits/stdc++.h>
#define INF 999999999.0
using namespace std;
typedef long long ll;
int par[200005]; // ufd
int v0; ll minw;
vector<ll> w; // weight array
priority_queue<tuple<double,int,int,ll,ll,ll>> pq[200005]; //pq saving h-arcs
//crit, left, right, cumucost, cumuw, cost
int ufd(int x){return par[x]=(x==par[x])?x:ufd(par[x]);}
ll ANS; // total sum of top costs of 2 terminal pqs is the answer
void merge(int x,int y){
    ANS-=(get<5>(pq[x].top())+get<5>(pq[y].top()));
    int i=get<1>(pq[x].top()), j=get<2>(pq[y].top()), k=get<2>(pq[x].top());
    int m, M, target; // target direction contains M
    if(w[i]<=w[j]&&j!=v0){
        m=i; M=j; target=y;
    }
    else{
        m=j; M=i; target=x;
    }
    ll cumucost=get<5>(pq[x].top())+get<5>(pq[y].top()); // cost outside fan
    //printf("%lld ",cumucost);
    ll cumuw=w[k]*w[M]; // arc of fan (initially)
    while(!pq[target].empty()&&(double)w[m]<=get<0>(pq[target].top())){
        auto tp=pq[target].top();
        cumucost+=(-get<5>(tp)+get<3>(tp));
        cumuw+=(-w[get<1>(tp)]*w[get<2>(tp)]+get<4>(tp));
        pq[target].pop();
    }
    ll cost=cumucost+w[m]*cumuw;
    cumuw+=w[k]*w[m]; // now it's W of fan polygon
    target=(pq[x].size()>pq[y].size())?x:y; // small 2 large
    while(!pq[x+y-target].empty()){ // merge pq
        pq[target].push(pq[x+y-target].top());
        pq[x+y-target].pop();
    }
    par[x+y-target]=target;
    double crit;
    while(!pq[target].empty()){
        crit=(double)(cost-cumucost)/(cumuw-w[i]*w[j]);
        auto tp=pq[target].top();
        if(get<0>(tp)<crit)break;
        cumucost+=(-get<5>(tp)+get<3>(tp));
        cumuw+=(-w[get<1>(tp)]*w[get<2>(tp)]+get<4>(tp));
        pq[target].pop();
    }
    pq[target].emplace(crit,i,j,cumucost,cumuw,cost);
    ANS+=cost;
}
int main(){
    int n;
    scanf("%d",&n);
    stack<int> st; // tracks local max
    
    for(int i=0;i<n;i++){ // find v0
        ll x,y;
        scanf("%lld%lld",&x,&y);
        if(!i){
            v0=(x>y); minw=min(x,y);
            w.push_back(x); w.push_back(y);
        }
        else{
            if(y<minw){
                v0=i+1; minw=y;
            }
            w.push_back(y);
        }
    }
    n++; // n-gon
    for(int i=0;i<n;i++)par[i]=i; // ufd initial setting
    for(int i=0;i<n;i++){
        int x=(v0+i)%n, prev_x=(v0+i-1+n)%n;
        pq[ufd(x)].emplace(INF,prev_x,x,0LL,w[prev_x]*w[x],0LL); // pq initial setting
        while(st.size()>1&&w[x]<w[st.top()]){ // loc max at st.top
            merge(ufd(st.top()),ufd(x));
            st.pop();
        }
        st.push(x);
    }
    while(st.size()>2){ // flush remaining LIS
        merge(ufd(st.top()),ufd(v0));
        st.pop();
    }
    printf("%lld",ANS);
}