#include <bits/stdc++.h>
#include <type_traits>
using namespace std;
using lli = long long int;
using ld = long double;

// class for storing complex numbers
template <class T>
class Complex{
public:
    T real;
    T im;

    //constructors for complex class
    Complex<T>(){
        real = 0;
        im = 0;
    }
    Complex<T>(T r, T i) : real(r), im(i) {}

    // operator overloading of + for complex no. addition
    Complex<T> operator +(Complex<T> c){
        Complex<T> ans;
        ans.real = this->real + c.real;
        ans.im = this->im + c.im;
        return ans;
    }

    // operator overloading of * for multiplying two complex numbers
    Complex<T> operator *(const Complex<T> &c){
        Complex<T> ans;
        ans.real = this->real * c.real - this->im * c.im;
        ans.im = this->real * c.im + this->im * c.real;
        return ans;
    }

    //operator overloading of * for multiplying a complex no. with an integer
    Complex<T> operator *(const int x){
        Complex<T> ans;
        ans.real = this->real * x;
        ans.im = this->im * x;
        return ans;
    }

    //operator overloading of >> for input of complex nos.
    friend istream &operator>>(istream &stream, Complex<lli> &c){
        stream >> c.real >> c.im;
        return stream;
    }

    //operator overloading of << for output
    friend ostream &operator<<(ostream &stream, Complex<lli> &c){
        stream << c.real << " " << c.im;
        return stream;
    }
};

// operator overloading of * for string multiplication
string operator *(string s, lli tim){
    string pdt = "";
    for (lli i = 0; i<tim; i++){
        pdt += s;
    }
    return pdt;
}

//forward declaration of polynomial class for other classes (mult,eval,diff etc)
template <class T>
class Polynomial;


// class for polynomial multiplication
template <class T>
class Multiplication{
public:

    // function to multiply two polynomials
    static Polynomial<T> multiply(Polynomial<T> &A, Polynomial<T> &B){
        int deg1 = A.degree;
        int deg2 = B.degree;
        int dg = deg1 + deg2;
        Polynomial<T> product(dg);
        product.coefficients.resize(dg-1, T());
        for (int idx1 = 0; idx1 < deg1; idx1++){
            for (int idx2 = 0; idx2 < deg2; idx2++){
                product.coefficients[idx1 + idx2] =product.coefficients[idx1 + idx2] + (A.coefficients[idx1] * B.coefficients[idx2]);
            }
        }
        return product;
    }
};



// class for polynomial evaluation
template <class T>
class Evaluation{
public:

    // function which evaluates a polynomial at a point (integer or float coefficients)
    static T evaluate(Polynomial<T> &p, lli x){
        T result = T(0);
        vector<lli> pows(p.degree,0);
        pows[0] = 1;
        for (int i=1; i<p.degree; i++){
            pows[i] = pows[i-1]*x;
        }
        for (int idx= p.degree-1; idx>=0; idx--){
            result = result + p.coefficients[idx]* pows[idx];
        }
        return result;
    }

    //function which evaluates a polynomial at a point (for string coefficients)
    static string evaluateString(Polynomial<T> &p, lli x){
        string result = "";
        vector<lli> pows(p.degree,0);
        pows[0] = 1;
        for (int i=1; i<p.degree; i++){
            pows[i] = pows[i-1]*x;
        }
        for (int idx= p.degree-1; idx>=0; idx--){
            result = result + p.coefficients[idx]* pows[idx];
        }
        return result;
    }
};


// class for polynomial differentiation
template <class T>
class Differentiation{
public:

    //class method for differentiating a polynomial 
    static Polynomial<T> diff(Polynomial<T> &p){
        int deg = p.degree - 1;
        Polynomial<T> der(deg);
        for (int pow = 1; pow <= deg; pow++){
            T val = (p.coefficients[pow]) * pow;
            der.coefficients.push_back(val);
        }
        return der;
    }
};


//polynomial class for storing polynomials and containing driver functions for operations
template <class T>
class Polynomial{
public:
    int degree;
    vector<T> coefficients;

    // Polynomial constructor: takes in degree and coefficients and stores them in vector
    Polynomial<T>(int dg){
        degree = dg;
    }

    // operator overloading of * for polynomial multiplication
    Polynomial<T> operator*(Polynomial<T> &p2){
        Polynomial<T> product = Multiplication<T>::multiply(*this, p2);
        return product;
    }

    // driver function to evaluate a polynomial at a point
    T evaluate(lli x){
        T ans;
        if constexpr(is_same<T,string>::value){
            ans = Evaluation<T>::evaluateString(*this,x);
        }
        else{
            ans = Evaluation<T>::evaluate(*this, x);
        }
        return ans;
    }

    //driver function for polynomial differentiation
    Polynomial<T> differentiate(){
        Polynomial<T> derivative = Differentiation<T>::diff(*this);
        return derivative;
    }

    //operator overloading of >> to input a polynomial
    friend istream &operator>>(istream &stream, Polynomial<T> &p){
        int dg = p.degree;
        for (int i = 0; i < dg; i++){
            T cf;
            stream >> cf;
            p.coefficients.push_back(cf);
        }
        return stream;
    }

    //operator overloading of << to output a polynomial
    friend ostream &operator<<(ostream &stream, Polynomial<T> &p){
        if (std::is_same<T,long double>::value){
            stream << fixed << setprecision(6);
        }
        for (T coeff : p.coefficients)
        {
            stream << coeff << " ";
        }
        return stream;
    }
};


int main(){
    lli q;
    cin >> q;
    while (q--){
        int op;
        cin >> op;

        switch (op){

            //problem type 1 - differentiation
            case (1):{
                string type;
                cin >> type;
                if (type == "integer"){
                    int deg1;
                    cin >> deg1;
                    Polynomial<lli> A(deg1);
                    cin >> A;
                    int deg2;
                    cin >> deg2;
                    Polynomial<lli> B(deg2);
                    cin >> B;
                    Polynomial<lli> product = A * B;
                    cout << product << endl;
                }
                else if (type == "float"){
                    int deg1;
                    cin >> deg1;
                    Polynomial<ld> A(deg1);
                    cin >> A;
                    int deg2;
                    cin >> deg2;
                    Polynomial<ld> B(deg2);
                    cin >> B;
                    Polynomial<ld> product = A * B;
                    cout << product << endl;
                }
                else if (type == "complex"){
                    int deg1;
                    cin >> deg1;
                    Polynomial<Complex<lli>> A(deg1);
                    cin >> A;
                    int deg2;
                    cin >> deg2;
                    Polynomial<Complex<lli>> B(deg2);
                    cin >> B;
                    Polynomial<Complex<lli>> product = A * B;
                    cout << product << endl;
                }

                break;
            }

            //problem type 2 - evaluation
            case (2):{   
                string type;
                cin >> type;
                if (type == "integer"){
                    int deg; cin >> deg;
                    Polynomial<lli> P(deg);
                    cin >> P;
                    int x; cin >> x;
                    lli ev = P.evaluate(x);
                    cout << ev << endl;
                }
                else if (type == "float"){
                    int deg; cin >> deg;
                    Polynomial<ld> P(deg);
                    cin >> P;
                    int x; cin >> x;
                    ld ev = P.evaluate(x);
                    cout << fixed << setprecision(6)<< ev << endl;
                }
                else if (type == "string"){
                    int deg; cin >> deg;
                    Polynomial<string> P(deg);
                    cin >> P;
                    int x; cin >> x;
                    string ev = P.evaluate(x);
                    cout << ev << endl;
                }

                break;
            }

            //problem type 3 - differentiation
            case (3):{
                string type;
                cin >> type;
                if (type == "integer"){
                    int deg;
                    cin >> deg;
                    Polynomial<lli> P(deg);
                    cin >> P;
                    Polynomial<lli> derivative = P.differentiate();
                    cout << derivative << endl;
                }
                else if (type == "float"){
                    int deg;
                    cin >> deg;
                    Polynomial<ld> P(deg);
                    cin >> P;
                    Polynomial<ld> derivative = P.differentiate();
                    cout << derivative << endl;
                }
            }
        }
    }
}