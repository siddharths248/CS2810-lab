#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;
using pll = pair<ll,ll>;
using vpll = vector<pll>;

//custom comparator struct with comparator functor and a  bool var to check if ascending or descending (for mergesort)
struct Comparator{
    bool operator ()(ll a, ll b){      
            if (asc){
                return (a<=b);
            }
            else{
                return (a>=b);
            }  
    }
    bool asc;
};


//board class
class Board{
    public:

        //Board constructor - initialises bool flags
        Board(){
            invComp = new Comparator;
            invComp->asc = true;
        }

        ~Board(){
            delete invComp;
        }

        //function to store values in matrix
        istream & board_init(istream & stream){
            ll size;
            stream >> size;
            n = size;
            for (ll i=0; i<size; i++){
                vll row;
                matrix.push_back(row);
                for (ll j=0; j<size; j++){
                    ll a;
                    stream >> a;
                    matrix[i].push_back(a);
                }
            }
            return stream;
        }

        //driver function which takes ptr to comparator as argument
        void sortRows(Comparator* comp){
            string ord;
            cin >> ord;
            if (ord == "ascending"){
                comp->asc = true;
            }
            else if (ord == "descending"){
                comp->asc = false;
            }
            for (vll & row : matrix){
                mergesort(row, 0, n-1, comp);
            }
        }

        //recursive mergesort function to sort each row.
        void mergesort(vll & arr, ll st, ll end, Comparator* comp){

            if (st >= end){
                return;
            }
            ll mid = st + (end-st)/2;
            mergesort(arr,st,mid,comp);
            mergesort(arr,mid+1,end,comp);
            merge(arr,st,mid,end,comp);
            
        }

        //merge function for mergesort
        ll merge(vll & arr, ll st, ll mid, ll end, Comparator* comp){
            ll n1 = mid-st+1; //no. of elements in left array
            ll n2 = end-mid;  //no. of elements in right array
            ll inv = 0;
            
            vll lvec(n1) , rvec(n2);

            for (ll i=0; i<n1; i++){
                lvec[i] = arr[st+i];
            }
            for (ll j=0; j<n2; j++){
                rvec[j] = arr[mid+1 + j];
            }

            ll i=0,j=0;
            ll fin=st;

            while(i<n1 && j<n2){                    //putting elements into final array in ascending order
                if ((*comp)(lvec[i],rvec[j])){  
                        arr[fin] = lvec[i];
                        i++;
                    }
                    else{
                        arr[fin] = rvec[j];
                        j++;
                        inv += (n1-i);   //in case the element at the current pointer in right array is lesser then element at curr. pointer in left array, increment no of inversions by no. of elements to the right of current element in left array
                    }
                    fin++;
            }

            while (i<n1){                //put remaining elements back into array
                arr[fin] = lvec[i];
                i++;
                fin++;
            }
            while (j<n2){
                arr[fin] = rvec[j];
                j++;
                fin++;
            }
            return inv;                  //returns no. of inversions ACROSS left and right subarrays
        }



        //driver fn to count inversions in matrix
        void countInversions(){
            flatten();                //finds flat (1D) representation of matrix in row major order
            ll totalInv = mergeAndCount(flat,0,n*n-1);  
            cout << totalInv << endl;
        }


        //fn to get the 1D representation of the matrix in a single vector of size n*n
        void flatten(){
            flat.clear();
            for (ll i=0; i<n; i++){
                for (ll j=0; j<n; j++){
                    flat.push_back(matrix[i][j]);
                }
            }
        }

        //recursive merge and count function for counting inversions using divide and conquer
        ll mergeAndCount(vll & arr, ll st, ll end){
            ll inv = 0;
            if (st >= end){
                return inv;
            }
            ll mid = st + (end-st)/2;
            ll leftInv = mergeAndCount(flat,st,mid);     //no. of inversions only in left subarray
            ll rightInv = mergeAndCount(flat,mid+1,end); //no. of inversions only in right subarray

            inv += (leftInv+rightInv + merge(flat,st,mid,end,invComp));  //merging and finding increments across both halves.
            
            return inv;
        }

        //fn to display matrix
        ostream & display(ostream & stream){
            for (ll i=0; i<n; i++){
                for (ll j=0; j<n; j++){
                    stream << matrix[i][j] << " ";
                }
                stream << endl;
            }
            return stream;
        }

    private:
        ll n; //size (n)
        vll flat; //1d representation of matrix
        vvll matrix;    // matrix in row major form (vector of vectors)
        Comparator* invComp;  //comparator in which asc is always set to true (for mergesort in countInversions)
};

//operator overloading of >> to take board from cin
istream & operator >> (istream & stream, Board* b){
    return (b->board_init(stream));
}

//operator overloading of << to display board
ostream & operator << (ostream & stream, Board* b){
    return(b->display(stream));
}

class Point{
    public:

    Point(){}   //empty constructor for point object declaration

    //point constructor
    Point(ll x1, ll y1, ll in) : x(x1), y(y1), ind(in){} 

    //operator overloading of < to compare 2 points : compares x coordinate, then y coordinate.
    bool operator<(const Point &q)const{
        if (x!=q.x) return (x < q.x);
        if (y!=q.y) return (y < q.y);
        return (false);
    }

    // '==' operator for points
    bool operator == (const Point &q){
        return ( (x==q.x)&&(y==q.y));
    }

    ll x;
    ll y;
    ll ind;
};

//class to store coordinate pairs on a plane for closest pair problem
class Plane{

    public:

    //constructor of plane
    Plane(){}

    //function to store pairs in plane class
    istream & plane_init(istream & stream){
        ll num;
        stream >> num;
        for (ll i=0; i<num; i++){
            ll x,y;
            stream >> x >> y;
            Point p = Point(x,y,i);
            points.push_back(p);
        }
        return stream;
    }

    //computes square of distance between two points
    double sqdistance(Point a, Point b){
        return ((a.x-b.x)*(a.x-b.x)) + ((a.y-b.y)*(a.y-b.y));
    }

    //fn to find closest pair on plane
    void closestPair(){
        x_sorted_points = y_sorted_points = points;

        //sorts points by x coordinate (using lambda function to check x coord, y coord, and index in order)
        sort(x_sorted_points.begin(),x_sorted_points.end(),[](Point a, Point b){
            if (a.x!=b.x) return (a.x < b.x);
            if (a.y!=b.y) return (a.y < b.y);
            return (a.ind < b.ind);
        });

        //sorts points by y coordinate (using lambda function to check y coord, x coord and index in order)
        sort(y_sorted_points.begin(),y_sorted_points.end(),[](Point a, Point b){
            if (a.y!=b.y) return (a.y < b.y);
            if (a.x!=b.x) return (a.x < b.x);
            return (a.ind < b.ind);
        });

        //res contains the pair of closest points on the plane
        pair<Point,Point> res = closestPairRecursive(x_sorted_points,y_sorted_points);

        //final checking to print points in the same order as they appear in the input by comparing point indices
        Point p = (res.first.ind < res.second.ind)? res.first : res.second;
        Point q = (res.first.ind > res.second.ind)? res.first : res.second;

        cout << p.x << " " << p.y << " " << q.x << " " << q.y << endl;
    }


    //recursive function to obtain closest pair of points
    pair<Point,Point> closestPairRecursive(vector<Point> x_sorted, vector<Point> y_sorted){
        ll n=x_sorted.size();

        //base case when n is 3 or less
        if (n<=3){
            pair<Point,Point> best;
            double min_dist = 1e18;

            for (ll i=0; i<n; i++){
                for (ll j=i+1; j<n; j++){
                    double d = sqdistance(x_sorted[i],x_sorted[j]);
                    if (d<min_dist){                //if distance is less, update best pair to new pair, and set min_dist to new minimum distance
                        min_dist = d;
                        best = {x_sorted[i],x_sorted[j]};
                    }
                    else if (d==min_dist){          //in case distance is the same, invoke tiebreaker of pairs to set best to winner of tiebreaker
                        pair<Point,Point> cand = {x_sorted[i], x_sorted[j]};
                        if (cand < best) best = cand;
                    }
                }
            }
            return best;
        }

        Point midpoint = x_sorted[n/2];
        vector<Point> x_left(x_sorted.begin(),x_sorted.begin()+n/2); //left half of points on the plane
        vector<Point> x_right(x_sorted.begin()+n/2, x_sorted.end()); //right half of points on plane

        //get sorted order of points by y coordinate in left half and right half separately
        vector<Point> y_left,y_right;
        for (Point p : y_sorted){
            if (p.x < midpoint.x){
                y_left.push_back(p);
            }else{
                y_right.push_back(p);
            }
        }


        pair<Point,Point> left_pair = closestPairRecursive(x_left,y_left);          //closest pair in left half
        pair<Point,Point> right_pair = closestPairRecursive(x_right,y_right);       //closest pair in right half    

        double left_min = sqdistance(left_pair.first,left_pair.second);             //min distance between points in left half
        double right_min = sqdistance(right_pair.first,right_pair.second);          //min distance between points in right half

        pair<Point,Point> best;
        if (left_min<right_min) best = left_pair;                                   //obtain closest pair among the two, by invoking the tiebreaker 
        else if (right_min < left_min) best = right_pair;
        else best = min(left_pair, right_pair);
        double min_dist = min(left_min,right_min);

        vector<Point> strip;                                                        //vector to store points that are within min horizontal distance from the midpoint.
        for (Point p : y_sorted){
            if ((p.x-midpoint.x)*(p.x-midpoint.x) < min_dist){
                strip.push_back(p);
            }
        }
        
        //
        for (ll i=0; i<(ll)strip.size(); i++){
            for (ll j=i+1; j<(ll)strip.size() && (strip[j].y-strip[i].y)*(strip[j].y-strip[i].y)<min_dist; j++){  //for every pair of point within strip that is within less than min_dist
                double d = sqdistance(strip[i],strip[j]);
                if (d < min_dist){                                  
                    best = {strip[i],strip[j]};                         //update best pair to new pair if d < min_dist
                    min_dist = d;
                }
                else if (d==min_dist){
                    pair<Point,Point> cand;                     
                    cand = {strip[i], strip[j]};
                    if (cand < best) best = cand;                       //invoke tiebreaker of pairs if d==min_dist
                }   
            }
        }

        return best;
    }
    
    private:
        vector<Point> points;  //stores coordinate pairs of points
        vector<Point> x_sorted_points;  //stores points sorted by x coordinate
        vector<Point> y_sorted_points;  //stores points sorted by y coordinate
};

//operator overloading of >> to take plane from cin
istream & operator >> (istream & stream, Plane* p){
    return(p->plane_init(stream));
}

int main(){
    Board* cb = new Board;
    Comparator* cmp = new Comparator;

    string cmd;

    while (cin >> cmd){

        if (cmd == "CREATE_2D"){
            cin >> cb;
        }
        else if (cmd == "SORT_2D"){
            cb->sortRows(cmp);
        }
        else if (cmd == "INVERSION_2D"){
            cb->countInversions();
        }
        else if (cmd == "DISPLAY_2D"){
            cout << cb;
        }

        else if (cmd == "CLOSEST_2D"){
            Plane* pl = new Plane;
            cin >> pl;
            pl->closestPair();
            delete pl;
        }
        else if (cmd == "END"){
            delete cb;
            delete cmp;
            break;
        }
    }
}
