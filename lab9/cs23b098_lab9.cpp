#include <bits/stdc++.h>
using namespace std;
using ll = long long int;


class Node{
    public:
        ll val;
        ll score;
        ll indeg;
        ll outdeg;
        ll dfsColor;
        ll dfsParent;
        ll dfsDisc;
        ll dfsFinish;
        ll dfsLow;
        ll sccID;

        Node(ll vl, ll scr) : val(vl), score(scr) {
            indeg = outdeg=0;
            dfsColor = 0;
            dfsParent = dfsDisc = dfsFinish = dfsLow = sccID = -1;
        }

    friend class Events;
};

//events class to store adjacencies and hype scores of graph
class Events{
    private:
        ll n;
        ll m;
        vector<set<ll>> adj;
        vector<Node*> nodes;
        vector<ll> topOrder;
        vector<Node*> condensedNodes;
        vector<ll> sortedCondensed;
        vector<set<ll>> sccList;
        vector<set<ll>> sccAdj;
        ll numSCC;
        ll maxSCCpathScore;
        bool hasCycle = false;
        bool topsortexists = true;
    
    public:
        
        //constructor for events (initialising vertices)
        Events(ll N, ll M) : n(N), m(M) {
            for (ll i=0; i<=n; i++){
                set<ll> s;
                this->adj.push_back(s);
            }
            Node* n = new Node(-1,-1);
            this->nodes.push_back(n);
            this->addNodes();
            this->addEdges();

            DFS(sccList);

            numSCC = sccList.size();

            this->lexBFS();

            this->condense();
            this->sortSCCs();
            this->findMaxScorePath();
        }

        void containsCycle(){
            if (hasCycle){
                cout << "YES" << endl;
            }
            else {
                cout << "NO" << endl;
            }
        }

        void sccFind(){
            ll ct = numSCC;
            ll mct = 0;
            for (set<ll> s : sccList){
                ll compSz = 0;
                for (ll i : s){
                    compSz++;
                }
                if (compSz >= mct){ mct = compSz; }
            }
            cout << ct << " " << mct << endl;
        }

        void topSort(){
            if (topsortexists){
                for (ll x : topOrder){
                    cout << x << " ";
                }
                cout << endl;
            }
            else {
                cout << "NO" << endl;
            }
            
        }

        void maxPath(){
            cout << maxSCCpathScore << endl;
        }

        //getter for adjacency list
        vector<set<ll>> getAdj(){
            return this->adj;
        }


    private:

        //function to add scores to each node
        void addNodes(){
            for (ll i=1; i<=n; i++){
                ll hi;
                cin >> hi;
                Node* nd = new Node(i,hi);
                this->nodes.push_back(nd);
            }
        }

        //fn to set edges
        void addEdges(){
            for (ll j=0; j<m; j++){
                ll a,b;
                cin >> a >> b;
                this->adj[a].insert(b);
                nodes[a]->outdeg++;
                nodes[b]->indeg++;
            }
        }


        //general multi-purpose DFS function
        void DFS(vector<set<ll>> & sccList){
            ll t=0;
            ll sccCt=0;
            bool cycleFound = false;
            stack<ll> st;
            //vector<ll> low(n+1,-1);

            for (ll u=1; u<=n; u++){
                if (nodes[u]->dfsColor == 0){
                    DSFvisit(u,t,cycleFound,sccList,st,sccCt);
                }
            }
            this->hasCycle = cycleFound;
        }

        void DSFvisit(ll u, ll & t, bool & found, vector<set<ll>> & sccList, stack<ll> & st, ll & sccCt){
            t++;
            nodes[u]->dfsDisc = t;
            nodes[u]->dfsLow = t;
            nodes[u]->dfsColor = 1;
            st.push(u);

            for (ll v : this->adj[u]){
                if (nodes[v]->dfsColor == 1){
                    found = true;
                    nodes[u]->dfsLow = min(nodes[u]->dfsLow, nodes[v]->dfsDisc);
                }
                else if (nodes[v]->dfsColor == 0){
                    nodes[v]->dfsParent = u;
                    DSFvisit(v,t,found,sccList,st,sccCt);
                    nodes[u]->dfsLow = min(nodes[u]->dfsLow, nodes[v]->dfsLow);
                }
            }
            //t++;
            if (nodes[u]->dfsLow == nodes[u]->dfsDisc){
                set<ll> SCC;
                while(true){
                    ll nd = st.top();
                    st.pop();
                    nodes[nd]->dfsColor = 2;
                    nodes[nd]->sccID = sccCt;
                    SCC.insert(nd);
                    if (nd==u){ break; }
                }
                sccList.push_back(SCC);
                sccCt++;
                
            }
            nodes[u]->dfsFinish = t;
        }

        void lexBFS(){
            priority_queue<ll,vector<ll>,greater<ll>> pq;
            for (ll u=1; u<=n; u++){
                if (nodes[u]->indeg == 0){
                    pq.push(u);
                }      
            }
            ll ct=0;

            while (!pq.empty()){
                ll u = pq.top();
                pq.pop();
                topOrder.push_back(u);
                for (ll v : adj[u]){
                    if (--(nodes[v]->indeg) == 0){
                        pq.push(v);
                    }
                }
                ct++;
            }
            
            if (ct!=n){
                topsortexists = false;
            }
        }

        void condense(){
            sccAdj.resize(numSCC);
            ll ct=0;
            for (set<ll> scc : sccList){
                ll sm=0;
                for (ll u : scc){
                    sm+= nodes[u]->score;
                }
                Node* newscc = new Node(ct,sm);
                this->condensedNodes.push_back(newscc);
                ct++;
            }
            for (ll u=1; u<=n; u++){
                for (ll v : this->adj[u]){
                    if (nodes[u]->sccID != nodes[v]->sccID){
                        sccAdj[nodes[u]->sccID].insert(nodes[v]->sccID);
                        condensedNodes[nodes[u]->sccID]->outdeg++;
                        condensedNodes[nodes[v]->sccID]->indeg++;
                    }
                }
            }
        }

        void sortSCCs(){
            queue<ll> q;
            for (ll u=0; u<numSCC; u++){
                if (condensedNodes[u]->indeg == 0){
                    q.push(u);
                }
            }

            while (!q.empty()){
                ll u = q.front();
                q.pop();
                sortedCondensed.push_back(u);
                for (ll v : this->sccAdj[u]){
                    if (--(condensedNodes[v]->indeg) == 0){
                        q.push(v);
                    }
                }
            }
        }

        void findMaxScorePath(){
            ll maxscore=0;
            vector<ll> dp(numSCC,0);
            for (ll u : sortedCondensed){
                dp[u] = max(dp[u],condensedNodes[u]->score);
                for (ll v : sccAdj[u]){
                    dp[v] = max(dp[v], dp[u]+condensedNodes[v]->score);
                }
                maxscore = max(dp[u],maxscore);
            }
            maxSCCpathScore = maxscore;
        }
        

    friend class GraphAlgorithm;
};

//abstract class graph algorithm
class GraphAlgorithm{
    private:

        

    public:
        virtual void Query(){};
};

//isCycle class
class isCycle : public GraphAlgorithm{
    public:

        void Query(){

        }
};

//class for independent components
class indepComponent : public GraphAlgorithm{

};

//class for topological sorting
class validOrder : public GraphAlgorithm{

};

//class for computing maximum hype score path
class maxHype : public GraphAlgorithm{

};


//main function to take input in and perform queries
int main(){
    ll n, m;
    cin >> n >> m;

    Events* events = new Events(n,m);
    
    
    ll q;
    cin >> q;
    for (ll k=0; k<q; k++){
        ll tp;
        cin >> tp;
        if (tp == 1){
            events->containsCycle();
        }
        else if (tp == 2){
            events->sccFind();
        }
        else if (tp == 3){
            events->topSort();
        }
        else if (tp == 4){
            events->maxPath();
        }
    }
    /*std::cout << "No. of cycles" << tp;*/
}
