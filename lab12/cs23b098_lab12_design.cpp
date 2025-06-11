#include <bits/stdc++.h>
using namespace std;
using ll = long long int;


class Card{
    protected:
        long long int strength;
        string shape;

    public:

        //card empty constructor
        Card(){}

        virtual ~Card(){}

        //getter for strength of card
        long long int getStrength(){
            return this->strength;
        }

        //overloading comparator operator for cards
        bool operator < (Card& c) const{
            return (this->strength < c.strength);
        }

        //setter method (pure virtual fn) to set values for each shape
        virtual void setValues(istream& stream) = 0;
        //pure virtual fn for displaying each shape
        virtual void display(ostream& stream) = 0;

        //overloading of istream (>>) for cards
        friend istream & operator >>(istream & stream, Card& c){
            c.setValues(stream);
            return stream;
        }
        //overloading of ostream (<<) operator for cards
        friend ostream & operator << (ostream & stream, Card& c){
            c.display(stream);
            return stream;
        }
};

class Square : public Card{
    private:
        ll sideLength;
    public:

        //empty constructor for square derived from card 
        Square() : Card() {}

        //fn to set values of square (from pure virtual fn in card class)
        void setValues(istream & stream){
            ll side;
            stream >> side;
            this->sideLength = side;
            this->strength = side*side;
            this->shape = "SQUARE";
        }
        
        //fn to display values of each shape
        void display(ostream & stream){
            stream << this->shape << " " << this->sideLength << "\n";
        }
        

};

class Rectangle :public Card{
    private:
        ll length;
        ll breadth;

    public:

        //empty constructor of rectangle
        Rectangle() : Card() {}

        //fn to set values of rectange (from pure virtual fn in card class)
        void setValues(istream & stream){
            ll le, br;
            stream >> le >> br;
            this->length = le;
            this->breadth = br;
            this->strength = le*br;
            this->shape = "RECTANGLE";
        }

        //fn to display values of each shape
        void display(ostream& stream){
            stream << this->shape << " " << this->length << " " << this->breadth << "\n";
        }
};

class Triangle : public Card{
    private:
        ll base;
        ll height;
    
    public:

        //empty constructor of triangle
        Triangle() : Card() {}

        //fn to set values of triangle (from pure virtual fn in card class)
        void setValues(istream & stream){
            ll b,h;
            stream >> b >> h;
            this->base = b;
            this->height = h;
            this->strength = (b*h)/2;
            this->shape = "TRIANGLE";
        }

        //fn to display values of each shape
        void display(ostream & stream){
            stream << this->shape << " " << this->base << " " << this->height << "\n";
        }
};

//card organiser class with problem solving
class CardOrganizer{
    private:
        vector<Card*> pileTops;        //vector which store topmost card in each pile.
        vector<Card*> cards;           //vector storing cards in order of input

        //vector which stores indices of the smallest possible last card in an increasing sequence of length index+1
        //ie. tailIdx[i] contains index of smallest possible tail of an IS of length i+1
        //gets updated every time a new card is added
        vector<ll> tailIdx;  
        
        //vector which stores back pointer for reconstructing LIS from the end
        //prevIdx[i] contains index of card which comes before ith card in prevIdx, in the final LIS 
        vector<ll> prevIdx;

        ll pileCount;   //keeps track of number of piles so far
        ll lisLength;   //keeps track of maximum LIS length so far
        ll cardCount;   //keeps track of number of cards added so far


    public:

        //initialises all counters to 0
        CardOrganizer(){
            pileCount=0; lisLength=0;cardCount=0;
        }

        //fn to add a new card to a certain pile and to do precomputing of length of LIS
        void addCard(Card* card) {
            cards.push_back(card);
            ll currIdx = cardCount++;               //increments cardCount

            //find correct pile on which card is to be placed
            ll pileIdx = findPos(card);                 
            if (pileIdx == pileCount){
                pileTops.push_back(card);           //if possible, place on an existing pile
                pileCount++;
            }
            else {
                pileTops[pileIdx] = card;           //else, create a new pile with this as topmost card
            }

            //find length of an Increasing sequence with this card as its tail
            ll lisPos = findPosTails(card);
            if (lisPos==lisLength){             //current card cannot be the smallest tail of any IS shorter than prev maximum LIS length
                tailIdx.push_back(currIdx);     //an LIS of length+1 is possible with this card as its tail
                lisLength++;                    //this means that the current card extends the LIS found so far
            }
            else {
                tailIdx[lisPos]=currIdx;        //current card can be smallest tail of any LIS shorter than prev max length 
            }

            if (lisPos==0){                     //if current card starts a new IS (prev ptr is -1)
                prevIdx.push_back(-1);
            }
            else {
                prevIdx.push_back(tailIdx[lisPos-1]);       //prev ptr is stored in prev position of tailIdx. this is stored in current position in prevIdx
            }

        }

        //fn which returns number of piles currently
        size_t getPile() const {
            return pileTops.size();
        }

        //fn which constructs LIS by backtracking using prev. indices.
        vector<Card*> getLIS(){
            vector<Card*> lis;
            if (lisLength==0){
                return lis;
            }
            //index of last element in LIS is the last element of tailIdx (ie. tail of longest possible IS)
            ll idx=tailIdx[lisLength-1];        
             
            //pushes elements of LIS in reverse order until invalid prev ptr (-1) is encountered.
            while (idx!=-1){
                lis.push_back(cards[idx]);
                idx = prevIdx[idx];
            }
            reverse(lis.begin(), lis.end());        //to obtain LIS in correct order
            return lis;
        }

        //displaying output for query type 3
        void printLIS(){
            vector<Card*> lis = getLIS();
            ll n = lis.size();
            cout << n << endl;
            for (Card* c : lis){
                cout << *c;
            }
        }

        //fn to create card based on shape (calls appropriate setValues function)
        static Card* createCard(istream & input){
            string shapeType;
            input >> shapeType;
            Card* c = nullptr;
            if (shapeType == "SQUARE"){
                c = new Square();
            }    
            else if (shapeType == "RECTANGLE"){
                c = new Rectangle();
            }
            else if (shapeType == "TRIANGLE"){
                c = new Triangle();
            }

            if (c) {
                input >> *c;
            }
            return c;
        }
    
    private:
        
        //binary search to find pile with smallest possible topmost card on which current card can be placed
        ll findPos(Card* card){
            ll left=0; ll right = pileTops.size();
            while(left < right){
                ll mid = (left+right)/2;
                if (*pileTops[mid] < *card){
                    left = mid+1;
                }
                else {
                    right = mid;
                }
            }
            return left;
        }
        
        //binary search to find length of IS with current card as its smallest possible tail
        ll findPosTails(Card* card){
            ll strength = card->getStrength();
            ll left=0; ll right = tailIdx.size();
            while (left<right){
                ll mid = (left+right)/2;
                if (cards[tailIdx[mid]]->getStrength() < strength){
                    left =mid+1;
                }
                else {
                    right = mid;
                }
            }
            return left;
        }
};


int main(){
    long long int q;
    cin >> q;

    CardOrganizer org;

    while (q--){
        int queryType;
        cin >> queryType;
        
        if (queryType == 1){
            Card* card = org.createCard(cin);
            org.addCard(card);
        }
        else if (queryType == 2){
            cout << org.getPile() << "\n";
        }
        else if (queryType == 3){
            org.printLIS();
        }

    }
}