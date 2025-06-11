#include <bits/stdc++.h>
using namespace std;
using ll = long long int;
const ll INF = LLONG_MAX;

//Node class  to store data of a node
class Node{
    private:
        string roomID;  //roomID
        ll distance;    //distance from source (initialised to INF)
        vector<pair<Node*,ll>> adjNodes;    //nodes adjacent to this node


    public:

        bool parity;    //even/odd version
        
        //constructor for node
        Node(string rID){ 
            roomID = rID; 
            distance = INF;
        }

        //getter methods
        string getID(){
            return this->roomID;
        }

        ll getDistance(){
            return distance;
        }
        
        //setter for distance
        void setDistance(ll d){
            this->distance = d;
        }

        //returns adjacency nodes' list
        vector<pair<Node*,ll>> & getAdjNode(){
            return this->adjNodes;
        }
        
        //fn to add an adjacent node to adj list
        void addAdjNode(Node* b, ll wt){
            this->adjNodes.push_back({b,wt});
        }


        //overloading comparison operator for comparing nodes in pq
        bool operator > (Node b){
            if (this->getDistance() > b.getDistance()){
                return true;
            }
            return false;
        }
};

//class to store map of rooms
class Mmap{

    public:
        //mapping from roomID to ptr to node objects
        map<string,pair<Node*,Node*>> nodeMap;
        ll n;
        ll m;

        //constructor of Mmap
        Mmap(ll N, ll M){
            this->n=N;
            this->m=M;
        }

        //fn to add new node to map
        void addNode(string id, Node* n0, Node* n1){
            this->nodeMap[id] = {n0,n1};
        }

        //fn to add edge between two nodes with alternating parity
        void addEdge(string id1, string id2, ll wt){
            pair<Node*,Node*> p1 = nodeMap[id1];
            pair<Node*,Node*> p2 = nodeMap[id2];
            (p1.first)->addAdjNode(p2.second,wt); //a_even to b_odd
            (p1.second)->addAdjNode(p2.first,wt); //a_odd to b_even
            (p2.first)->addAdjNode(p1.second,wt); //b_even to a_odd
            (p2.second)->addAdjNode(p1.first,wt); //b_odd to a_even
        }


};


//priority queue class
template <class T>
class priorityQueue{
    private:
        vector<T> arr; //nodes in pq are stored in this array
        ll currentSize;  //no. of elements currently stored pq

        //heapify functions

        //moves empty spot up so that heap property is satisfied
        void heapifyUp(ll hole){
            if (empty()){
                return;
            }
            T nd = arr[hole];
            while(hole>1 && *(arr[hole/2])>*nd){
                arr[hole] = arr[hole/2];
                hole=hole/2;
            }
            arr[hole] = nd;
            return;
        }
        //moves empty spot down so that heap property is satisfied
        void heapifyDown(ll hole){
            if (!empty()){
                T nd = arr[hole];
                while(2*hole <= currentSize){
                    ll ch1=2*hole;
                    ll ch2=ch1+1;
                    ll chmin = (ch2<=currentSize && *(arr[ch1])>*(arr[ch2]))?ch2:ch1;
                    if (*(arr[chmin])>*nd){ break; }
                    arr[hole] = arr[chmin];
                    hole=chmin;
                }
                arr[hole] = nd;
                return;
            }
        }


    public:
        //constructor for pq
        priorityQueue(){
            arr.resize(1e6);
            currentSize =0;
        }

        //returns current size of priority queue
        const size_t size(){
            return currentSize;
        }

        //fn to insert a new element into a priority queue
        void push(const T & nd){
            if (full()) { arr.resize(2*arr.size()+1);}
            ll nidx = currentSize+1;
            arr[nidx] = nd;
            currentSize++;
            heapifyUp(nidx);
            return; 
        }

        //fn to return topmost element 
        const T& top(){
            if (!empty()){
                return arr[1];
            }
            else { return nullptr; }
        }
        
        //fn to remove top element of priority queue
        void pop(){
            if (!empty()){
                arr[1] = arr[currentSize];
                currentSize--;
                heapifyDown(1);
            }
        }

        //fns to check if pq is empty/full or not
        bool empty(){
            return (currentSize==0);
        }
        bool full(){
            return (currentSize+1 == arr.size());
        }

};


//class for implementing Dijkstra's
class Dijkstra{
    private:
        set<Node*> visited;   //set to store visited nodes
        priorityQueue<Node*> pq;    //priority queue of node*s
        Mmap* mmp;                  //ptr to Mmap object

    public:
        
        void solve(string rm1, string rm2, Mmap* m){
            mmp=m;
            pair<Node*,Node*> src = mmp->nodeMap[rm1];
            pair<Node*,Node*> dest = mmp->nodeMap[rm2];
            Node* src0 = src.first;      //src0 is even version of start node
            Node* dst0 = dest.first;     //dst0 is even version of destination node 

            //set distance of even source to 0
            src0->setDistance(0);       
            pq.push(src0);

            while (!pq.empty()){
                Node* u = (pq.top());
                pq.pop();

                if (visited.find(u) != visited.end()){
                    continue;
                }
                visited.insert(u);

                for (pair<Node*,ll> ndp : u->getAdjNode()){
                    Node* v = ndp.first;
                    ll wt = ndp.second;
                    
                    ll newDist = u->getDistance() + wt;

                    if (newDist < v->getDistance()){
                        v->setDistance(newDist);
                        pq.push(v);
                    }
                }
            }

            //distance is INF if path with even no. of edges doesn't exist
            if (dst0->getDistance() == INF){
                cout << -1 << endl;
            }
            else {
                cout << dst0->getDistance() << endl;
            }

        };
};

int main(){
    ll n;
    ll m;
    cin >> n >> m;

    Mmap* mmp = new Mmap(n,m);

    for (ll i=0; i<n; i++){
        string id;
        cin >> id;
        Node* nd0 = new Node(id); nd0->parity=0;
        Node* nd1 = new Node(id); nd1->parity=1;
        mmp->addNode(id,nd0,nd1);
    }

    for (ll j=0; j<m; j++){
        string id1;
        string id2;
        ll weight;
        cin >> id1 >> id2 >> weight;
        mmp->addEdge(id1,id2,weight);
    }

    string rm1;
    string rm2;
    cin >> rm1 >> rm2;
    

    Dijkstra d;
    d.solve(rm1,rm2,mmp);
    
}