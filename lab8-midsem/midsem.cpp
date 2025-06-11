#include <bits/stdc++.h>
using namespace std;
using ll = long long;


class Employee{
    private:
        ll age;
        ll experience;
        ll employeeID;
        ll bossID;
        ll depth;
        ageComp agc;

        Employee* boss;
        vector<Employee*> reportees;

    public:

        
        //employee constructor
        Employee(ll age, ll exp, ll ID,ll bID)
        : age(age), experience(exp), employeeID(ID),bossID(bID){}

        //fn to push this vector to reportees list of its boss
        void initialise(){
            (this->boss)->getReportees().push_back(this);
        }

        //setter fn for boss
        void setBoss(Employee* nboss){
            this->boss = nboss;
        }

        //recursive fn to compute depth from parent
        void setDepth(){
            this->depth = this->boss->getDepth()+1;
            for (Employee* rep : this->getReportees()){
                rep->setDepth();
            }
            return;
        }

        //setter for initialising depth of founder to 0
        void initDepth(int d){
            this->depth = d;
        }

        //driver fn for tc type 2
        void computeAvgExp(){
            ll expSum = 0;
            ll count=0;
            expSum = this->findSum();
            count = this->findCount();
            ll avg = expSum/count;
            cout << avg << endl;
        }

        //recursive fn to compute sum of experiences 
        ll findSum(){
            //cout << "id " << this->getID() << endl;
            if (reportees.size()==0){
                return this->experience;
            }
            ll subTreeSum=this->experience;
            for (Employee* rep : this->reportees){
                subTreeSum += rep->findSum();
            }
            //cout << "subtre sum  " << subTreeSum << endl;
            return subTreeSum;
        }

        //recursive fn to count subordinates
        ll findCount(){
            if (reportees.size()==0){
                return 1;
            }
            ll subTreeCount=1;
            for (Employee* rep : this->reportees){
                subTreeCount+= rep->findCount();
            }
            return subTreeCount;
        }
        
        //driver fn for tc type 3
        void findKthYoungest(ll k){
            ll ct;
            ct = this->findCount();
            if (ct < k){
                cout << -1 << endl;
                return;
            }
            //priority_queue<Employee*> maxHeap(ageCompare());
            vector<Employee*> subList;
            subList.push_back(this);
            this->searchForKthYoungest(subList);
            sort(subList.begin(),subList.end(),agc());
        }

        //recursive fn to push all subordinates into
        void searchForKthYoungest(vector<Employee*> & subList){
            if (this->reportees.size()==0){
                subList.push_back(this);
                return;
            }
            for (Employee* rep : this->reportees){
                rep->searchForKthYoungest(subList);
            }
            return;
        }

            // bool ageCompare(Employee* a, Employee* b){
            //     ll a_age = a->getAge();
            //     ll b_age = b->getAge();
            //     ll aID = a->getID();
            //     ll bID = b->getID();
            //     if (a_age > b_age){
            //         return true;
            //     }
            //     else if (a_age == b_age){
            //         if (aID > bID){
            //             return true;
            //         }
            //         else {
            //             return false;
            //         }
            //     }
            //     else {
            //         return false;
            //     }
            // }

            



        //getter methods

        ll getAge(){
            return age;
        }

        ll getExperience(){
            return experience;
        }

        ll getID(){
            return employeeID;
        }

        Employee* getBoss(){
            return boss;
        }

        ll getBossID(){
            return bossID;
        }

        vector<Employee*> getReportees(){
            return reportees;
        }

        ll getDepth(){
            return depth;
        }

        
};




class TAIL{
    public:
        ll n;
        Employee* founder;

        unordered_map<ll,Employee*> employeeLookup;
        vector<Employee*> employees;

        TAIL(){
            n=0;
        }

        //fn to add employees to vector initially
        void addEmployee(Employee* emp){
            employeeLookup[emp->getID()] = emp;
            employees.push_back(emp);
            n++;
            //cout << "added_empl" << endl;
        }

        //fn to set founder
        void addFounder(Employee* emp){
            employeeLookup[emp->getID()] = emp;
            founder = emp;
            employees.push_back(emp);
            n++;
        }

        void setBosses(){
            for (Employee* emp : employees){
                if (emp->getID() == emp->getBossID()){
                    emp->setBoss(emp);
                }
                else {
                    Employee* nboss = employeeLookup[emp->getBossID()];
                    emp->setBoss(nboss);
                    emp->initialise();
                }
            }
        }

        void setUpTree(){
            founder->initDepth(0);
            for (Employee* rep : founder->getReportees()){
                rep->setDepth();
            }
            return;
        }
        

        void countNonDom(int crit){
            ll count = 0;
            for (Employee* empj : employees){
                bool isnondomable=true;
                for (Employee* empi : employees){
                    if (empi != empj){
                        if (compare(empi,empj,crit)){
                            isnondomable = false;
                        }
                        else{
                            continue;
                        }
                    }
                }
                if (isnondomable){
                    count++;
                }
            }
            cout << count << endl;
        }

        bool compare(Employee* a, Employee* b, int cr){
            if (cr==1){
                if ((a->getExperience() > b->getExperience()) && (a->getDepth() < b->getDepth())){
                    return true;
                }
                else return false;
            }
            else if (cr==2){
                if ((a->getAge() > b->getAge()) && (a->getDepth() < b->getDepth())){
                return true;
                }
                else return false;
            }
            else if (cr==3){
                if ((a->getExperience() > b->getExperience()) && (a->getAge() < b->getAge())){
                return true;
                }
                else return false;
            }
            return false;
        }

        
    
    friend class Employee;
};

int main(){
    ll n;
    cin >> n;
    TAIL* tail = new TAIL;
    
    while (n--){
        
        ll id, a, exp, bossID;
        cin >> id >> a >> exp >> bossID;
        Employee* emp = new Employee(a,exp,id,bossID);
        if (id == bossID){
            tail->addFounder(emp);
        }
        else{
            tail->addEmployee(emp);
        }
        

    }
    tail->setBosses();
    tail->setUpTree();
    ll q;
    cin >> q;
    while (q--){
        int type;
        cin >> type;
        
        if (type==1){
            int crit;
            cin >> crit;
            tail->countNonDom(crit);
        }
        else if (type==2){
            ll id;
            cin >> id;
            Employee* emp = tail->employeeLookup[id];
            emp->computeAvgExp();
        }
        else if (type==3){
            ll id, k;
            cin >> id >> k;
            Employee* emp = tail->employeeLookup[id];
            emp->findKthYoungest(k);
        }
    }
}