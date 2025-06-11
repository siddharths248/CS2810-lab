#include <bits/stdc++.h>
using namespace std;

class QNS{
    private:
        int powerLevel;
        int durability;
        int energyStorage;
        int heatLevel;


    public:
        bool isFunc;
        bool isDead;

        // Parametrised constructor for QNS
        QNS(int p, int d, int e, int h){
            powerLevel = min(p,5000);
            durability = d;
            energyStorage = e;
            heatLevel = h;
            if (h > 500){
                isFunc = false;
            }
            else{ isFunc = true; }
            isDead = false;
            
        }

        // Default constructor for QNS
        QNS(){
            powerLevel = 1000;
            durability = 500;
            energyStorage = 300;
            heatLevel = 0;
            isFunc = true;
            isDead = false;
        }

        // Copy constructor
        QNS(QNS & q){
            powerLevel = min(q.powerLevel,5000);
            durability = q.durability;
            energyStorage = q.energyStorage;
            heatLevel = q.heatLevel;
            if (heatLevel > 500){
                isFunc = false;
            }
            else{ isFunc = true; }
            isDead = false;
        }

        //function to boost power of suit by factor (using * operator)
        void boostPower(int factor){
            *this * factor;
        }

        //function to boost suit through other suit (using + operator)
        void boostPower(QNS* otherSuit){
            *this + *otherSuit;
        }


        //function to reduce points of suit (using - operator   )
        void takeDamage(int dmg){
            *this - dmg;
        }


        //getter methods for parameters

        int getPowerLevel(){
            return powerLevel;
        }


        int getDurability(){
            return durability;
        }
        
        int getEnergyStorage(){
            return energyStorage;
        }

        int getHeatLevel(){
            return heatLevel;
        }
        

        //operator overloading for suit upgradation
        void operator +(QNS suit){
            if (!powerLimit(suit.getEnergyStorage())){
                powerLevel += suit.getEnergyStorage();
            }
            else { powerLevel = 5000; }

            durability += suit.getDurability();

            energyStorage += suit.getPowerLevel();

        }

        //operator overloading for suit damage
        void operator - (int dmg){
            durability -= dmg;
            energyStorage += dmg;
            heatLevel += dmg;

            if (isDestroyed()){
                isDead = true;
                isFunc = false;
            }
            if (overHeat()){
                isFunc = false;
            }

        }

        //operator overloading for power boost
        void operator *(int f){
            int p_add = (powerLevel * f)/100;
            if (!powerLimit(p_add)){
                powerLevel += p_add;
            }
            else { powerLevel = 5000; }

            energyStorage += 5*f;
            heatLevel += f;
            
            if (overHeat()){
                isFunc = false;
            }
            
        }

        //operator overloading for cooling
        void operator /(int cool){
            durability += cool;

            if (!heatDrop(cool)){
                heatLevel -= cool;
            }
            else { heatLevel = 0; }

            if (!overHeat()){
                isFunc = true;
            }
        }

        //comparison operators
        bool operator ==(QNS* suit){
            if ((this->powerLevel == suit->getPowerLevel()) && (this->durability == suit->getDurability())){
                return true;
            }
            return false;
        }

        bool operator <(QNS suit){
            if ((powerLevel + durability) < (suit.getPowerLevel() + suit.getDurability())){
                return true;
            }
            return false;
        }


        //constraint checking functions:

        //caps heatlevel
        bool heatLimit(int x){
            if (heatLevel + x >= 500){
                return true;
            }
            return false;
        }

        //checks if suit is overheating
        bool overHeat(){
            if (heatLevel > 500){
                return true;
            }
            return false;
        }

        //fixes heat at 0 on cooling if negative value
        bool heatDrop(int x){
            if (heatLevel - x <= 0){
                return true;
            }
            return false;
        }

        //checks if health drops below 0 on taking damage
        bool healthDrop(int x){
            if (durability - x <= 0){
                return true;
            }
            return false;
        }

        //checks if soat gets destroyed
        bool isDestroyed(){
            if (durability <= 0){
                return true;
            }
            return false;
        }

        //checks if power reaches limit (5000)
        bool powerLimit(int x){
            if (powerLevel + x >= 5000){
                return true;
            }
            return false;
        }



};


class Avenger{
    private:
        string name;
        QNS* suit;
        int attackStrength;

    public:

        //constructor which does not initialise suit
        Avenger(string name, int strength){
            name = name;
            attackStrength = strength;
            suit = nullptr;
        }

        //Avenger constructor
        Avenger(string avName, QNS* avSuit, int strength){
            name = avName;
            suit = avSuit;
            attackStrength = strength;
        }

        //attack fn
        void attack(Avenger* enemy){
            if (canAttack()){
                if (enemy->isAlive()){
                    enemy->takeDamage(attackStrength);
                }
            }
        }
        
        //upgrade suit
        void upgradeSuit(QNS* upgradeSuit){
            *suit + *upgradeSuit;
        }

        //repair suit
        void repair(int x){
            *suit / x;
        }

        //damage accepting fn
        void takeDamage(int dmg){
            suit->takeDamage(dmg);
        }

        //checks if avenger can attack (if suit is not destroyed AND overheated)
        bool canAttack(){
            if (isAlive() && suit->isFunc) { return true; }
            else { return false;}
        }

        //checks if avenger is alive
        bool isAlive(){
            if (!suit->isDead) { return true; }
            else { return false; }
        }

        //checks if avenger is overheated or not
        bool isOverheated(){
            if (suit->overHeat()){
                return true;
            }
            return false;
        }

        //prints status of avenger's suit
        void printStatus(){
            cout << name << " " << suit->getPowerLevel() << " " << suit->getDurability() << " " << suit->getEnergyStorage() << " " << suit->getHeatLevel() << endl; 
        }

        // getter methods for private members

        string getName(){
            return name;
        }

        QNS* getSuit(){
            return suit;
        }


        int getAttackStrength(){
            return attackStrength;
        }

        void setAttackStrength(int x){
            attackStrength = x;
        }
};


class Battle{
    private:
        vector<Avenger*> heroes;
        vector<Avenger*> enemies;
        vector<string> battleLog;

        queue<QNS*> suits;

        map<string,Avenger*> avenger_map;
        map<string,QNS*> suit_map;

        bool battle;
    
    public:

        //start battle fn
        void startBattle(){
            battle = true;
        }

        //pushes suit pointer onto queue
        void addSuit(QNS* s){
            suits.push(s);
        }

        //pushes ptr to avenger object into heroes' queue
        void addHero(Avenger* av){
            heroes.push_back(av);
            avenger_map[av->getName()] = av;
        }

        //pushes ptr to avenger object onto enemies' queue
        void addEnemy(Avenger* av){
            enemies.push_back(av);
            avenger_map[av->getName()] = av;
        }

        //fn which simulates one avenger attacking another
        // takes two avenger names from stdin and calls a->avenger->attack(b)
        // pushes status statements into log
        void attack(){
            string ava, avb;
            cin >> ava >> avb;
            Avenger* a = avenger_map[ava];
            Avenger* b = avenger_map[avb];
            a->attack(b);
            string lg = a->getName() + " attacks " + b->getName();
            battleLog.push_back(lg);
            if (!b->isAlive()){
                string lg = b->getName() + " suit destroyed";
                battleLog.push_back(lg);
            }
            else if (b->isOverheated()){
                string lg = b->getName() + " suit overheated";
                battleLog.push_back(lg);
            }
        }


        //fn which simulates avenger boosting suit
        //calls boost() fn of corresponding avenger's suit
        //pushes status stmts into log
        void boost(){
            string avname;
            int p,d,e,h;
            cin >> avname >> p >> d >> e >> h;
            Avenger* a = avenger_map[avname];
            QNS * s = new QNS(p,d,e,h);
            a->getSuit()->boostPower(s);
            string lg = a->getName() + " boosted";
            battleLog.push_back(lg);
            if (a->isOverheated()){
                string lg = a->getName() + " suit overheated";
                battleLog.push_back(lg);
            }
        }
        
        //fn which simulates avenger boosting suit
        //calls boost() fn of corresponding avenger's suit
        //pushes status stmts into log
        void boostByFactor(){
            string avname;
            int x;
            cin >> avname >> x;
            Avenger* a = avenger_map[avname];
            a->getSuit()->boostPower(x);
            string lg = a->getName() + " boosted";
            battleLog.push_back(lg);
            if (a->isOverheated()){
                string lg = a->getName() + " suit overheated";
                battleLog.push_back(lg);
            }
        }

        //fn which simulates upgrading a current suit using another available suit if available
        void upgradeSuit(){
            string avname;
            cin >> avname;
            Avenger * a = avenger_map[avname];
            if (suitAvailable()){
                QNS* s = getAvlSuit();
                a->upgradeSuit(s);
                string lg = a->getName() + " upgraded";
                battleLog.push_back(lg);
            }
            else {
                string lg = a->getName() + " upgrade Fail";
                battleLog.push_back(lg);
            }
        }

        //fn for repairing suit
        void repair(){
            string avname;
            int x;
            cin >> avname >> x;
            Avenger * a = avenger_map[avname];
            a->repair(x);
            string lg = a->getName() + " repaired";
            battleLog.push_back(lg);
        }


        //prints battle log statements
        void printBattleLog(){
            for (string s : battleLog){
                cout << s << endl;
            }
        }

        //prints current status of avenger
        void avengerStatus(){
            string avname;
            cin >> avname;
            Avenger * a = avenger_map[avname];
            a->printStatus();
        }   

        //sums durability and powerlevel of each hero/enemy and compares to compute the result of battle
        int Result(){
            int hsum = 0;
            int esum = 0;
            for (Avenger* av : heroes){
                if (av->isAlive()){
                    hsum += (av->getSuit()->getDurability() + av->getSuit()->getPowerLevel());
                }
                else continue;
            }
            for (Avenger* av : enemies){
                if (av->isAlive()){
                    esum += (av->getSuit()->getDurability() + av->getSuit()->getPowerLevel());
                }
                else continue;
            }
            if (hsum > esum){
                return 1;
            }
            else if (hsum == esum){
                return 0;
            }
            else { return -1; }
        }

        //checks if queue of suits is empty or not
        bool suitAvailable(){
            if (!suits.empty()){
                return true;
            }
            return false;
        }

        //if suits are available, returns the first avl suit
        QNS* getAvlSuit(){
            if (suitAvailable()){
                QNS* s = suits.front();
                suits.pop();
                return s;
            }
            else {return nullptr;}
        }

        //prints out current battle status
        void battleStatus(){
            if (Result() == 1){
                cout << "heroes are winning" << endl;
            }
            else if (Result() == -1){
                cout << "enemies are winning" << endl;
            }
            else {
                cout << "tie" << endl;
            }
        }

    friend class Avenger;

};


int main(){

    Battle* battle = new Battle;

    int k,n,m;
    cin >> k >> n >> m;


    // creates k suit and pushes them onto queue 
    for (int i=0; i<k; i++){
        int p,d,e,h;
        cin >> p >> d >> e >> h;
        QNS* qns = new QNS(p,d,e,h);
        battle->addSuit(qns);
    }

    //adds heroes one by one
    for (int j=0; j<n; j++){
        string avname;
        int att;
        cin >> avname >> att;
        if (battle->suitAvailable()){
            QNS* as = battle->getAvlSuit();
            Avenger* av = new Avenger(avname,as,att);
            battle->addHero(av);
        }
        else {
            cout << avname << " is out of fight" << endl;
        }
    }

    //adds enemies one by one
    for (int l=0; l<m; l++){
        string avname;
        int att;
        cin >> avname >> att;
        if (battle->suitAvailable()){
            QNS* as = battle->getAvlSuit();
            Avenger* av = new Avenger(avname,as,att);
            battle->addEnemy(av);
        }
        else {
            cout << avname << " is out of fight" << endl;
        }
    }
    

    //takes commands one by one and executes corresponding instruction one by one
    string cmd;

    while (cin >> cmd){
        
        if (cmd == "End"){
            break;
        }
        else if (cmd == "BattleBegin"){
            battle->startBattle();
        }
        else if (cmd == "Attack"){
            battle->attack();
        }
        else if (cmd == "Repair"){
            battle->repair();
        }
        else if (cmd == "BoostPower"){
            battle->boost();
        }
        else if (cmd == "BoostPowerByFactor"){
            battle->boostByFactor();
        }
        else if (cmd == "Upgrade"){
            battle->upgradeSuit();
        }
        else if (cmd == "AvengerStatus"){
            battle->avengerStatus();
        }
        else if (cmd == "PrintBattleLog"){
            battle->printBattleLog();
        }
        else if (cmd == "BattleStatus"){
            battle->battleStatus();
        }
    }
}
