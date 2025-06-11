#include <bits/stdc++.h>
using ll=long long int;
using namespace std;

//point struct to store x and y coordinates with + and / overloaded
struct Point{
    int x;
    int y;

    Point(){}
    Point(int X, int Y): x(X), y(Y){}

    //overloading of + operator
    Point operator + (Point p){
        Point res(0,0);
        res.x = x+p.x;
        res.y = y+p.y;
        return res;
    }

    //overloading of / operator to find centroid
    Point operator / (int fac){
        Point res(0,0);
        res.x = x/fac;
        res.y = y/fac;
        return res;
    }
};


//Island class (abstract base class)
class Island {
    protected:
        int type;
        Point centroid;
        double maxDist;
        string ID;
    
    public:

        Island(){}
        //pure virtual destructor
        virtual ~ Island(){}

        //overloading of >> operator for Island class
        friend istream & operator >>(istream & stream, Island & island){
            island.setValues(stream);
            return stream;
        }
        //pure virtual function for setting values to island.
        virtual void setValues(istream& stream) = 0;

        //overloading << operator of Island class
        friend ostream & operator << (ostream & stream, Island & island){
            stream << island.ID << " ";
            return stream;
        }

        //getter methods

        int getType(){
            return type;
        }

        string getID(){
            return ID;
        }

        Point getCentre(){
            return this->centroid;
        }

        double getMaxDist(){
            return this->maxDist;
        }
};

//Rectangle derived class
class Rectangle : public Island {
    private:
        Point a;
        Point b;
        Point c;
        Point d;

    public:

        //constructor with id and type
        Rectangle(string id):Island(){
            this->type = 0;
            this->ID = id;
        }
        
        //sets points' values and calls centroid and distance functions
        void setValues(istream & stream){
            int x1,y1,x2,y2,x3,y3,x4,y4;
            stream >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
            a.x=x1; a.y=y1;
            b.x=x2; b.y=y2;
            c.x=x3; c.y=y3;
            d.x=x4; d.y=y4;
            findCentroid();
            computeMaxDist();
        }

        //computes centre of rectangle
        void findCentroid(){
            Point ctr = (a+b+c+d)/4;
            this->centroid = ctr;
        }

        //computes maximum distance 
        void computeMaxDist(){
            int xdist = a.x - centroid.x;
            int ydist = a.y - centroid.y;
            long long squared = xdist*xdist + ydist*ydist;
            double md = sqrtl(squared);
            this->maxDist = md;
        }
};

//triangle derived class
class Triangle : public Island {
    private:
        Point a;
        Point b;
        Point c;

    public:

        //constructor with id and type
        Triangle(string id): Island(){
            this->type = 1;
            this->ID = id;
        }

        //sets points' values and calls centroid and distance functions
        void setValues(istream & stream){
            int x1,y1, x2,y2, x3,y3;
            stream >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
            a.x=x1; a.y=y1;
            b.x=x2; b.y=y2;
            c.x=x3; c.y=y3;
            findCentroid();
            computeMaxDist();
        }
        
        //finds centroid of triangle
        void findCentroid(){
            Point ctr = (a+b+c)/3;
            this->centroid = ctr;
        }

        //computes maximum distance of centroid from one of the points
        void computeMaxDist(){
            int xdist1 = a.x - centroid.x;
            int ydist1 = a.y - centroid.y;
            long long squared1 = xdist1*xdist1 + ydist1*ydist1;
            double md1 = sqrtl(squared1);

            int xdist2 = b.x - centroid.x;
            int ydist2 = b.y - centroid.y;
            long long squared2 = xdist2*xdist2 + ydist2*ydist2;
            double md2 = sqrtl(squared2);

            int xdist3 = c.x - centroid.x;
            int ydist3 = c.y - centroid.y;
            long long squared3 = xdist3*xdist3 + ydist3*ydist3;
            double md3 = sqrtl(squared3);

            this->maxDist = max(md1,max(md2,md3));
        }
};

//Circle derived class
class Circle : public Island {
    private:
        Point ctr;
        int radius;

    public:

        //constructor with id and type
        Circle(string id): Island(){
            this->type = 2;
            this->ID = id;
        }

        //sets points' values (centre and radius)
        void setValues(istream & stream){
            int x, y, rad;
            stream >> x >> y >> rad;
            ctr.x=x; ctr.y=y; 
            this->centroid = ctr;
            this->radius = rad;
            this->maxDist = radius;
        }
};


class Archipelago{
    private:
        int n;
        //list of islands
        vector<Island*> islands;
        //final list containing islands in path
        vector<Island*> path;
        //adjacency list
        vector<vector<int>> adj;

    public:
        
        //constructor
        Archipelago(int N){
            this->n = N;
            adj.resize(n);
        }


        void addIsland(Island* is){
            islands.push_back(is);
        }
        
        //adds island to adjacency list if hopping is possible between them
        void addEdges(){
            for (int i=0; i<n; i++){
                for (int j=i+1; j<n; j++){
                    if (canHop(islands[i],islands[j])){
                        adj[i].push_back(j);
                        adj[j].push_back(i);
                    }
                }
            }
        }

        

        //solver function
        void findPath(){
            vector<vector<bool>> dp(1<<n, vector<bool>(n,false));
            vector<vector<int>> parent(1<<n, vector<int>(n,-1));
            
            //initialising - all paths ending at i contain the ith vertex
            for (int i=0; i<n; i++){
                dp[1<<i][i] = true;
            }

            //for each possible combination of islands
            for (int mask=1; mask<(1<<n); mask++){
                //considering all possible ending points
                for (int u=0; u<n; u++){
                    //if set doesn't contain u or if there is no path possible ending at u
                    if (!(mask &(1<<u)) || !(dp[mask][u])){
                        continue;
                    }
                    //for every neighbour of u
                    for (int v : adj[u]){
                        //if v is already visited in path (no cycles)
                        if (mask & (1<<v)){
                            continue;
                        }
                        //set with v included as well
                        long long int nextMask = mask|(1<<v);
                        dp[nextMask][v] = true;         //set with v has a path ending at v
                        parent[nextMask][v] = u;        //prev node in this path is u (back ptr)
                    }
                }
            }

            //final mask including all nodes
            ll full = (1<<n)-1;
            int end=-1;     
            for (int u=0; u<n; u++){
                //if there is a path ending at u which includes all vertices exactly once
                if (dp[full][u]){
                    end=u;      //set ending pt of this path tou
                    break;
                }
            }   
            //if no such path is found
            if (end==-1){
                cout << "NO" << "\n";
                bool found=false;
                int maxLength=0;
                int endNode=-1;
                int bestMask=0;
                
                //iterate over all possible combinations of nodes
                for (ll mask=0; mask<(1<<n); mask++){
                    for (int u=0; u<n; u++){
                        //if a path with this particular combination exists
                        if (dp[mask][u]){
                            int length = countOnes(mask);
                            //finding max length of path so far
                            if (length >maxLength){
                                maxLength=length;
                                endNode=u;
                                bestMask=mask;
                            }
                        }
                    }
                }
                cout << maxLength << "\n";
                int cur=endNode;
                int mask= bestMask;
                //push nodes in path in reverse order (using prev ptr)
                while (cur!=-1){
                    path.push_back(islands[cur]);
                    int prev = parent[mask][cur];
                    mask ^=(1<<cur);
                    cur = prev;
                }
                reverse(path.begin(),path.end());
            }
            //if a hamiltonian path exists
            else {
                cout << "YES" << "\n";
                int mask= full;
                int cur= end;
                //push nodes in path in reverse order
                while (cur != -1){
                    path.push_back(islands[cur]);
                    int prev = parent[mask][cur];
                    mask ^= (1<<cur);
                    cur = prev;
                }
                reverse(path.begin(),path.end());
                
            }
        }

        //fn to display ids of islands in found path
        void printPath(){
            for (Island* isl : path){
                cout << *isl;
            }
            cout << "\n";
        }

        //fn to take input from stdin
        void takeInput(){
            for (int i=0; i<n; i++){
                string type;
                string id;
                cin >> type;
                cin >> id;
                if (type=="RECTANGLE"){
                    Rectangle* rec = new Rectangle(id);
                    cin >> *rec;
                    this->addIsland(rec);
                }
                else if (type == "TRIANGLE"){
                    Triangle* tri = new Triangle(id);
                    cin >> *tri;
                    this->addIsland(tri);
                }
                else if (type == "CIRCLE"){
                    Circle* cir = new Circle(id);
                    cin >> *cir;
                    this->addIsland(cir);
                }
            }
            this->addEdges();
        }

        private:

            //fn which checks if one can hop between two islands
            //if edge is possible between them or not
            bool canHop(Island* i1, Island* i2){
                Point ctr1 = i1->getCentre();
                Point ctr2 = i2->getCentre();
                double maxDist1 = i1->getMaxDist();
                double maxDist2 = i2->getMaxDist();
                int xd = ctr1.x - ctr2.x;
                int yd = ctr1.y - ctr2.y;
                double dist = sqrtl(xd*xd + yd*yd);
                return (maxDist1+maxDist2 >= dist);
            }

            //fn which finds length (if path exists) of current set (by counting 1s)
            int countOnes(ll x){
                int ct=0;
                while (x){
                    x &=(x-1);
                    ct++;
                }
                return ct;
            }
};


int main(){
    int n;
    cin >> n;
    Archipelago arch(n);
    arch.takeInput();
    arch.findPath();
    arch.printPath();
}