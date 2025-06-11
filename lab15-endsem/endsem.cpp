#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;
typedef long double ld;


//Event class (abstract base class)
class Event{
    protected:
        ll startTime;
        ll endTime;

        ld netProfit;
    

    public:

        //constructor
        Event(ll st, ll et){
            startTime = st;
            endTime =  et;
        }

        //getter methods
        ll getStart(){
            return startTime;
        }

        ll getEnd(){
            return endTime;
        }

        ld getProfit(){
            return this->netProfit;
        }
        
        //overloading of >> operator for extraction from istream
        friend istream & operator >> (istream & stream, Event & event){
            event.setValues(stream);
            return stream;
        }

        //pure virtual method to set parameters of derived event classes
        virtual void setValues(istream & stream) = 0;
        
        //pure virtual method to override calculateprofit
        virtual void calculateProfit() = 0;

};

//custom comparator for sorting events by end-time
class Compare{
    public:
        bool operator()(Event* ev1, Event* ev2){
            return (ev1->getEnd() > ev2->getEnd());
        }
};


// concert class
class Concert : public Event {
    private:
        ll ticketPrice;
        ll ticketsSold;
        ll artistFee;
        ll logisticCost;

        ll cost;
        ld ticketRevenue;


    public:

        //constructor - initiates start and end times for event
        Concert(ll st, ll et) : Event(st, et){}

        //sets values extracted from istream
        void setValues(istream & stream) {
            ll tp,ts,af,lc;
            stream >> tp >> ts >> af >> lc;
            ticketPrice = tp;
            ticketsSold = ts;
            artistFee = af;
            logisticCost = lc;
        }

        //calculates profit accordingly
        void calculateProfit(){ 
            ll gtr = ticketPrice * ticketsSold;
            ticketRevenue = gtr - 0.18*gtr;
            
            cost = artistFee + logisticCost;
            ld profit = ticketRevenue - cost;
            this->netProfit = (profit>(cost<<1))?(0.7*profit):profit;
            this->netProfit = (this->netProfit);
            
        }
};

class TheaterShow : public Event {
    private:
        ll baseTicketPrice;
        ll totalSeats;
        ll venueCost;

        ld ticketRevenue;
        ll popcornRevenue;


    public:

        //constructor - initiates start and end times for event
        TheaterShow(ll st, ll et) : Event(st, et){}

        //sets values extracted from istream
        void setValues(istream & stream){
            ll btp, ts, vc;
            stream >> btp >> ts >> vc;
            baseTicketPrice = btp;
            totalSeats = ts;
            venueCost = vc;
        }
        
        //calculates profit accordingly
        void calculateProfit(){
            ll gtr = (totalSeats>>2) * (baseTicketPrice<<1) + (3*baseTicketPrice)*(totalSeats>>2);
            ticketRevenue = gtr - 0.18*gtr;

            popcornRevenue = (totalSeats>>2)*150;

            this->netProfit = ticketRevenue + popcornRevenue - venueCost;
            
        }
};

class Wedding : public Event {
    private:
        ll baseAmount;
        ll decorationCost;
        ll guestCount;
        ll venueCost;

        ll totalVenueCost;
        ll cateringCost;

    public:

        //constructor - initiates start and end times for event
        Wedding(ll st, ll et) : Event(st, et){}
            
        //sets values extracted from istream
        void setValues(istream & stream){
            ll ba, dc, gc, vc;
            stream >> ba >> dc >> gc >> vc;
            baseAmount = ba;
            decorationCost = dc;
            guestCount = gc;
            venueCost = vc;
        }

        //calculates profit accordingly
        void calculateProfit(){
            totalVenueCost = (guestCount>200)?(venueCost*3):(venueCost);
            cateringCost = (guestCount>100)?(guestCount*70):(guestCount*100);

            this->netProfit = baseAmount - totalVenueCost - decorationCost - cateringCost;
        }
};


class EventScheduler{
    private:
        vector<Event*> eventsList; //list of events (ptr to event objects)
        vector<ll> prevCompatible; //stored index of previous compatible event (no overlaps)
        vector<ld> OPT;            //stored optimal value of maximum profit after considering first i events
        ll n;                      //number of events


    public:
        
        //initialises vectors before taking in input
        EventScheduler(ll N){
            n=N;
            prevCompatible.resize(n);
            OPT.resize(n);
        }

        //takes input/ creates events and stores them in eventList
        void takeInput(){
            for (ll itr=0; itr<n; itr++){
                int type;
                cin >> type;
                int start; int end;
                cin >> start >> end;
                //for each event, create event object, assign values, calculate profit and store it in list
                if (type==1){//event is of type concert
                    Concert* con = new Concert(start,end);
                    cin >> *con;
                    con->calculateProfit();
                    eventsList.push_back(con);
                }

                else if (type==2){   //event is of type theatershow
                    TheaterShow* ts = new TheaterShow(start,end);
                    cin >> *ts;
                    ts->calculateProfit();
                    eventsList.push_back(ts);
                }

                else if (type==3){   //event is of type wedding
                    Wedding* wed = new Wedding(start,end);
                    cin >> *wed;
                    wed->calculateProfit();
                    eventsList.push_back(wed);
                }
                else {}
            }
        }

        //solver - finds optimal maximum cost
        void solve(){
            //sorts list by ending times
            sort(eventsList.begin(), eventsList.end(), Compare());
            reverse(eventsList.begin(), eventsList.end());

            //for first event, previous compatible event is undefined, so -1
            prevCompatible[0] = -1;
            
            //for all other events, starting from the end 
            for (ll idx=n-1; idx>0; idx--){
                //cdIdx or index of candidate event for previous compatible event
                ll cdIdx = idx-1;
                
                //finds last compatible event from among all events ending before this event
                while (cdIdx>=0 && eventsList[cdIdx]->getEnd()>eventsList[idx]->getStart()){
                    cdIdx--;
                }
                //prevCompatible array contains last compatible event for this index
                prevCompatible[idx] = cdIdx;
            }

            //considering first event only, optimal value is cost of that event itself
            OPT[0] = eventsList[0]->getProfit();
            
            //for all other events (consider cases whether to include ith event or not - dp recurrence)
            for (ll i=1; i<n; i++){
                OPT[i] = max(OPT[i-1], (( (prevCompatible[i]==-1)?(0.0):(OPT[prevCompatible[i]]) ) + eventsList[i]->getProfit()));
            }

            //final answer is last optimal value
            ld ans = OPT[n-1];
            cout << fixed << setprecision(2) << ans << "\n";
        }
        
        //destructor
        ~EventScheduler(){
            for (ll i=0; i<n; i++){
                delete eventsList[i];
            }
        }


        

};


int main(){
    ll n;
    cin >> n;
    EventScheduler eventscheduler(n);
    eventscheduler.takeInput();
    
    eventscheduler.solve();
}