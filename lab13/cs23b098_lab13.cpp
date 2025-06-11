#include <bits/stdc++.h>
using namespace std;
using ll=long long int;
using ld=long double;

//custom comparator functor for sentinels
template<typename T>
class Compare {
    public:
        bool operator()(T* a, T* b) {
            return !(*a > *b); // lower rank => comes first
        }
};
    
//templated Kingdom class
template <class NodeType>
class Kingdom{
    private:
        vector<NodeType*> outposts;
        ll SenapatiCount;
        ll DandanayakaCount;
        ll ChaturanginiCount;
        //adjacency list
        vector<set<int>> adjOutposts;
    public:

        //constructor initialises everything to 0
        Kingdom(ll nodes){
            SenapatiCount=0;
            DandanayakaCount=0;
            ChaturanginiCount=0;
            adjOutposts.resize(nodes);
        }

        //getter methods
        int getSenapatiCount(){
            return SenapatiCount;
        }

        int getDandanayakaCount(){
            return DandanayakaCount;
        }

        int getChaturanginiCount(){
            return ChaturanginiCount;
        }

        NodeType* getOutpost(ll id){
            return outposts[id];
        }


        //fn to add new outpost (sentinel-node)
        void addOutpost(NodeType* nd, int rank){
            outposts.push_back(nd);
            if (rank==0){
                SenapatiCount++;
            }
            else if (rank==1){
                DandanayakaCount++;
            }
            else if (rank==2){
                ChaturanginiCount++;
            } else {}
        }

        //fn to add new road (edge)
        void addRoad(ll u, ll v){
            adjOutposts[u].insert(v);
            adjOutposts[v].insert(u);
        }

        // fn to find minimum vertex cover
        void findMinVertexCover(){
            int n = adjOutposts.size();
            //dp table: dp[i][1] is minimum cover if ith node is included in it and vice versa
            vector<vector<int>> dp(n,vector<int> (2,0));
            dfs(0,-1,dp);
            //overall min vertex cover is lesser of the two.
            ll ans = min(dp[0][0],dp[0][1]);
            cout << ans << endl;
        }

        //fn to sort sentinels by rank and to display sorted order
        void sortSentinels(){
            vector<NodeType*> sorting = outposts;
            sort(sorting.begin(),sorting.end(),Compare<NodeType>());
            for (NodeType* nd : sorting){
                cout << *nd;
            }
            cout << "\n";
        }
    
    private:
        
        //postorder dfs to find cover for subtree rooted at each node
        // dp table is modified in-place
        void dfs (ll node, ll parent, vector<vector<int>> & dp){
            dp[node][0] = 0;
            dp[node][1] = 1;

            for (int neighbour : adjOutposts[node]){
                if (neighbour != parent){
                    dfs(neighbour,node,dp);

                    //if node is not included, child (neighbour) must be included
                    dp[node][0] += dp[neighbour][1];        
                    //if node is included, choose minimum out of two possibilities available
                    dp[node][1] += min(dp[neighbour][0],dp[neighbour][1]);      
                }
            }
        }
    
    friend class Sentinel;
};

//abstract class for Sentinels
class Sentinel{
    protected:
        int ID;
        string name;
        int rank;
        Kingdom<Sentinel> * kingdom = nullptr;

    public:


        Sentinel(){}
        //virtual destructor
        virtual ~ Sentinel(){}

        void setKingdom(Kingdom<Sentinel>* k){
            kingdom = k;
        }

        //getter methods for values
        int getRank(){
            return rank;
        }

        int getID(){
            return ID;
        }

        //pure virtual countHigherRanked function
        virtual int countHigherRanked() = 0;

        //overloading comparison operator for Sentinels
        bool operator > (Sentinel & s){
            if (this->getRank()>s.getRank()){
                return true;
            }
            else if (this->getRank()== s.getRank()){
                return (this->getID()>s.getID());
            }
            else {
                return false;
            }
        }

        //overloading the ostream (<<) operator
        friend ostream & operator <<(ostream& stream, Sentinel & s){
            stream << s.ID << " "; 
            return stream;
        }
};

//Senapati class
class Senapati : public Sentinel {
    public:
        //constructor
        Senapati(int id, string nm) : Sentinel(){
            this->name = nm;
            this->ID = id;
            this->rank = 0;
        }
        // 0 higher ranked for Senapati
        int countHigherRanked(){
            return 0;
        }
        //displays ID
        void display(ostream & stream){
            cout << ID << " ";
        }
};

//Dandanayaka class
class Dandanayaka : public Sentinel {
    public:

        Dandanayaka(int id, string nm) : Sentinel(){
            this->name = nm;
            this->ID = id;
            this->rank = 1;
        }    


        int countHigherRanked(){
            return kingdom? kingdom->getSenapatiCount():0;
        }

        //displays ID
        void display(ostream & stream){
            cout << ID << " ";
        }
};

//Chaturangini class
class Chaturangini : public Sentinel {
    public:

        Chaturangini(int id, string nm) : Sentinel(){
            this->name = nm;
            this->ID = id;
            this->rank = 2;
        }

        int countHigherRanked(){
            return kingdom ? (kingdom->getSenapatiCount()+kingdom->getDandanayakaCount()):0;
        }

        //displays ID
        void display(ostream & stream){
            cout << ID << " ";
        }
};




int main(){
    ll nodes;
    cin >> nodes;
    Kingdom<Sentinel> kingdom(nodes);
    for (ll i=0; i<nodes-1; i++){
        ll u,v;
        cin >> u >> v;
        kingdom.addRoad(u,v);
    }
    for (ll id=0; id<nodes; id++){
        string name;
        string rank;
        cin >> name >> rank;
        if (rank=="SENAPATI"){
            Senapati* sp = new Senapati(id,name);
            sp->setKingdom(&kingdom);
            kingdom.addOutpost(sp,0);
        } 
        else if (rank=="DANDANAYAKA"){
            Dandanayaka* dd = new Dandanayaka(id,name);
            dd->setKingdom(&kingdom);
            kingdom.addOutpost(dd,1);
        }
        else if (rank=="CHATURANGINI"){
            Chaturangini* ch = new Chaturangini(id,name);
            ch->setKingdom(&kingdom);
            kingdom.addOutpost(ch,2);
        } else {}
    }
    int q;
    cin >> q;
    while (q--){
        int tp;
        cin >> tp;
        switch (tp){
            case (1):{
                kingdom.findMinVertexCover();
                break;
            }
            case (2):{
                kingdom.sortSentinels();
                break;
            }
            case (3):{
                ll id;
                cin >> id;
                Sentinel* s = kingdom.getOutpost(id);
                ll count = s->countHigherRanked();
                cout << count << "\n";
                break;
            }
            
            default:
                break;
            }
    }
   
}