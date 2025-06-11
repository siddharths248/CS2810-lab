#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vvll = vector<vector<ll>>;

class CurrencySorter{
    public:
        
        //functor to access array currency sorter
        void operator()(vector<ll> &arr){
            if (arr.empty()){ return; }
            ll n = arr.size();
            quicksort(arr,0,n-1);
            for (ll i : arr){
                cout << i << " ";
            }
            cout << endl;

        }


        //partition function for quick sort
        int partition(vector<ll> &arr, ll l, ll h){
            ll pivot = arr[h];
            ll i = l;
            for (ll j=l; j<h; j++){
                if (arr[j] <= pivot){
                    swap (arr[i],arr[j]);
                    i++;
                }
            }
            swap (arr[i],arr[h]);
            return i;

        }

        //recursive quicksort function
        void quicksort(vector<ll> &arr, ll l,ll h){
            
            if (l >= h) { return; }

            ll piv = partition(arr,l,h);
            quicksort(arr, l, piv-1);
            quicksort(arr,piv+1,h);
            return;
        }
    
};

class FibonacciGenerator{
    public:

        //functor to return ith fibonacci number
        void operator()(ll i){
            ll mod = 1e9+7;
            ll ans = 0;
            if (i == 0){
                ans = 0;
            } 
            else if (i == 1){
                ans = 1;
            }
            vector<ll> id = {1,0};
            vvll mat = { {1,1}, {1,0} };
            vvll res = raise(mat,i-1);
            ans = res[0][0] % mod;
            cout << ans << endl;
        }

        //function to raise matrix to power = exp
        vvll raise(vvll mat, ll exp){
            vvll ans = { {1,0}, {0,1} };
            while(exp){
                if (exp & 1){
                    multiply(ans,mat);
                }
                multiply(mat,mat);
                exp >>= 1;
            }
            return ans;
        }

        //function to brute force multiply two 2x2 matrices
        void multiply(vvll &a, vvll b){
            vvll c = { {0,0}, {0,0} };

            c[0][0] = ( (a[0][0]*b[0][0]) % mod + (a[0][1]*b[1][0]) % mod) % mod;
            c[0][1] = ( (a[0][0]*b[0][1]) % mod + (a[0][1]*b[1][1]) % mod) % mod;
            c[1][0] = ( (a[1][0]*b[0][0]) % mod + (a[1][1]*b[1][0]) % mod) % mod;
            c[1][1] = ( (a[1][0]*b[0][1]) % mod + (a[1][1]*b[1][1]) % mod) % mod;

            a=c;
        }

    private:   
        ll mod = 1e9 + 7;
        
};

class PrimeCalculator{
    public:
        vector<ll> primes;

        PrimeCalculator(){
            sieve();
        }

        //sieve of eratosthenes to store primes upto 10^6
        void sieve(){
            vector<bool> is_prime(1e6+1,true);
            is_prime[0]=is_prime[1] = false;

            for (ll i=2; i*i<=1e6; i++){
                if(is_prime[i]){
                    for (ll j=i*i; j<=1e6; j+=i){
                        is_prime[j]=false;
                    }
                }
            }
            for (ll i=2; i<=1e6; i++){
                if (is_prime[i]){
                    primes.push_back(i);
                    
                } 
            }
            //cout << "primes" << endl;
        }

        //obtains primes between l and r by removing composite numbers that are multiples of primes numbers in primes.
        vector<ll> getPrimes(ll l, ll r){
            if (l==1) { l=2; }
            vector<bool> is_prime(r-l+1, true);
            for (ll p : primes){
                ll q = p*p;
                if (q > r || q < 0) q = r + 1;
                if (q > r){
                    break;
                }
                //ll st = (l + p - 1) / p * p;
                //ll st = l + (p - l % p) % p;
                //ll st = ((l + p - 1) / p) * p;
                //ll st = l % p == 0 ? l : l + (p - l % p);
                //ll st = (l % p == 0) ? l : (l - (l % p) + p);
                ll st;
                ll md = l%p;
                if (md == 0){
                    st = l;
                }
                else {
                    ll k = l-md;
                    st = k+p;
                }
                //cout << "st " << st << " ";
                for (ll j = max(q,st); j<=r; j+=p){
                    is_prime[j-l] = false;
                }
            }
            vector<ll> primesInRange;
            for (ll i=0; i<= r-l; i++){
                if (is_prime[i]){
                    primesInRange.push_back(l+i);
                }
            }
            return primesInRange;
        }

        //prints all prime numbers in range (l,r)
        void printPrimes(ll l, ll r){
            for (ll p : getPrimes(l,r)){
                cout << p << " ";
            }
            cout << endl;
        }
        
        //finds sum of all prime numbers in range (l,r)
        void printPrimeSum(ll l, ll r){
            ll sum = 0;
            for (ll p : getPrimes(l,r)){
                sum+=p;
            }
            cout << sum << endl;
        }
};

class NumberAnalyser{
    public:

        //checks if x has factors that are perfect squares
        void isSquareFree(ll x){
            bool isf = true;
            if (x<=1){ isf = false; }
            for (ll i=2; i*i<=x; i++){
                if (x%i == 0){
                    if (x % (i*i)== 0){ isf = false; }
                    while (x%i == 0) { x/= i;}
                }
            }
            if (isf){
                cout << "yes" << endl;
            }
            else{
                cout << "no" << endl;
            }
        }

        //counts divisors of x
        void countDivisors(ll x){
            ll ct=0;
            for (ll i=1; i*i<=x; i++){
                if (x%i == 0){
                    ct++;
                    if (x != i*i){
                        ct++;
                    }
                }
            }
            cout << ct << endl;
        }

        //finds sum of divisors of x
        void sumOfDivisors(ll x){
            ll sm=0;
            for (ll i=1; i*i <=x; i++){
                if (x%i == 0){
                    sm += i;
                    if (x != i*i){
                        sm += x/i;
                    }
                }
            }
            cout << sm << endl;
        }


};


int main(){
    int type;
    cin >> type;
    switch(type){

        case 1:{
            CurrencySorter c;
            int t;
            cin >> t;
            while (t--){
                ll n;
                cin >> n;
                vector<ll> arr;
                for (ll i=0; i<n; i++){
                    ll a;
                    cin >> a;
                    arr.push_back(a);
                }
                c(arr);
            }
            break;
        }

        case 2:{
            FibonacciGenerator f;
            ll t;
            cin >> t;
            while (t--){
                ll i;
                cin >> i;
                f(i);
            }
            break;
        }

        case 3:{
            PrimeCalculator p;
            int t;
            cin >> t;
            while (t--){
                string cmd;
                cin >> cmd;

                if (cmd == "printPrimes"){
                    ll l,r;
                    cin >> l >> r;
                    p.printPrimes(l,r);
                }
                
                else if (cmd == "printPrimeSum"){
                    ll l,r;
                    cin >> l >> r;
                    p.printPrimeSum(l,r);
                }
            }
            break;
        }

        case 4:{
            NumberAnalyser n;
            ll t;
            cin >> t;
            while (t--){
                string cmd;
                cin >> cmd;

                if (cmd == "isSquareFree"){
                    ll x;
                    cin >> x;
                    n.isSquareFree(x);
                }

                else if (cmd == "countDivisors"){
                    ll x;
                    cin >> x;
                    n.countDivisors(x);
                }

                else if (cmd == "sumOfDivisors"){
                    ll x;
                    cin >> x;
                    n.sumOfDivisors(x);
                }
            }
            break;
        }
    }
}