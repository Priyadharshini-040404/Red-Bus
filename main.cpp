#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;

// ---------- Safe Input Helpers ----------
int readInt(string prompt, int minv = INT_MIN, int maxv = INT_MAX) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= minv && x <= maxv) return x;
        cout << "Invalid input! Enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
string readString(string prompt) {
    string s;
    while (true) {
        cout << prompt;
        if (cin >> s && !s.empty()) return s;
        cout << "Invalid input! Enter again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
char readChar(string prompt, string allowed = "ynYN") {
    char c;
    while (true) {
        cout << prompt;
        if (cin >> c && allowed.find(c) != string::npos) return c;
        cout << "Invalid choice! Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ---------- Utility ----------
string genOTP() { srand(time(0)); return to_string(1000 + rand()%9000); }
string nowTime() {
    time_t t = time(0); char buf[64];
    strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",localtime(&t));
    return buf;
}
vector<string> split(string s, char d=','){
    vector<string> out; string x; stringstream ss(s);
    while(getline(ss,x,d)) out.push_back(x);
    return out;
}
string join(vector<string> v,char d=','){
    string s; for(size_t i=0;i<v.size();i++){if(i) s+=d; s+=v[i];}
    return s;
}
string lower(string s){ for(char &c:s) c=tolower(c); return s; }
string normalizeDate(string s){ for(char &c:s) if(c=='/') c='-'; return s; }
// ---------- Date Compare ----------
bool isPastDate(string dateStr) {
    // format: dd-mm-yyyy
    int d, m, y;
    replace(dateStr.begin(), dateStr.end(), '-', ' ');
    stringstream ss(dateStr);
    ss >> d >> m >> y;

    time_t t = time(0);
    tm *now = localtime(&t);
    int cd = now->tm_mday, cm = now->tm_mon + 1, cy = now->tm_year + 1900;

    if(y < cy) return true;
    if(y > cy) return false;
    if(m < cm) return true;
    if(m > cm) return false;
    return (d < cd);
}

// ---------- Seed Data ----------
void seedData(){
    if(!ifstream("buses.csv").good()){
        ofstream f("buses.csv");
        for(int i=1;i<=20;i++)
            f<<"B"<<i<<",Bus"<<i<<",Chennai,Bangalore,01-09-2025,"<<(6+i)<<":00,"
             <<(i%2?"AC":"Non-AC")<<","<<(i%2?"Yes":"No")<<","<<(i%2?"Sleeper":"Seater")<<","<<500+(i*10)<<"\n";
    }
    if(!ifstream("seats.csv").good()){
        ofstream f("seats.csv");
        for(int i=1;i<=20;i++) for(int s=1;s<=24;s++) f<<"B"<<i<<","<<s<<",0\n";
    }
    if(!ifstream("coupon.csv").good()){
        ofstream f("coupon.csv");
        f<<"SAVE50,50\nBUS100,100\nRB75,75\nFEST200,200\nFIRST150,150\n"
         <<"WEEKEND80,80\nTRAVEL60,60\nMEGA250,250\nOFF120,120\nTRY30,30\n";
    }
    if(!ifstream("user.csv").good()) ofstream("user.csv");
    if(!ifstream("bookings.csv").good()) ofstream("bookings.csv");
    if(!ifstream("profiles.csv").good()) ofstream("profiles.csv");
}

// ---------- Login/Register ----------
bool login(string &mob){
    mob = readString("Enter Mobile Number: ");

    // check if already exists
    ifstream f("user.csv");
    string line;
    while(getline(f,line)){
        if(line==mob){
            cout<<"You are already logged in.\n";
            return true; // skip OTP
        }
    }

    // if new user, verify OTP
    string otp=genOTP();
    cout<<"Your Redbus login OTP is "<<otp<<"\n";
    string in = readString("Enter OTP: ");
    if(in==otp){
        ofstream("user.csv",ios::app)<<mob<<"\n";
        return true;
    }
    return false;
}
string init(){
    string mob;
    while(true){
        int c = readInt("\n1. Login\n2. Register\nChoice: ", 1, 2);
        if(login(mob)) { 
            cout<<"OTP Verified. Login Success!\n"; 
            break; 
        }
        else cout<<"Wrong OTP, try again.\n";
    }
    return mob;
}

// ---------- Booking ----------
struct Bus {string id,name,src,dst,date,time,type,ac,seat; int price;};

vector<Bus> readBuses(){
    vector<Bus> v; ifstream f("buses.csv"); string l;
    while(getline(f,l)){auto c=split(l); if(c.size()>=10)
        v.push_back({c[0],c[1],c[2],c[3],c[4],c[5],c[6],c[7],c[8],stoi(c[9])});}
    return v;
}

map<int,int> seatMap(string bid){
    map<int,int> m; ifstream f("seats.csv"); string l;
    while(getline(f,l)){auto c=split(l); if(c[0]==bid) m[stoi(c[1])]=stoi(c[2]);}
    return m;
}

void updateSeats(string bid,vector<int> sel,int occ){
    ifstream f("seats.csv"); string l; vector<string> out;
    while(getline(f,l)){auto c=split(l); if(c[0]==bid){
        int s=stoi(c[1]); if(find(sel.begin(),sel.end(),s)!=sel.end()) c[2]=to_string(occ);
        l=join(c);
    } out.push_back(l);}
    ofstream o("seats.csv"); for(auto &x:out) o<<x<<"\n";
}

map<string,int> coupons(){map<string,int> m;ifstream f("coupon.csv");string l;
    while(getline(f,l)){auto c=split(l);m[c[0]]=stoi(c[1]);} return m;}
void booking(string mob){
    auto buses=readBuses();
    string s = readString("Source: ");
    string d = readString("Destination: ");
    string date = readString("Date (dd-mm-yyyy): ");
    s=lower(s); d=lower(d); date=normalizeDate(date);

    vector<Bus> res; 
    for(auto &b:buses) 
        if(lower(b.src)==s && lower(b.dst)==d && normalizeDate(b.date)==date) res.push_back(b);

    if(res.empty()){cout<<"No buses.\n";return;}
    cout<<"\nAvailable Buses:\n";
    for(auto &b:res) cout<<b.id<<" "<<b.name<<" "<<b.time<<" "<<b.type<<" "<<b.seat<<" Rs."<<b.price<<"\n";

    string bid = readString("Select Bus ID: ");
    Bus sel; bool found=false; 
    for(auto &b:res) if(b.id==bid){ sel=b; found=true; }
    if(!found){cout<<"Invalid Bus ID.\n"; return;}

    int n = readInt("Seats to book: ",1,24);
    auto m=seatMap(bid); cout<<"Available:"; 
    for(auto &p:m) if(p.second==0) cout<<p.first<<" "; 
    cout<<"\nSelect "<<n<<" seats: ";
    vector<int> cho(n); 
    for(int i=0;i<n;i++) cho[i]=readInt("Seat: ",1,24);

    string board = readString("Boarding: ");
    string drop = readString("Dropping: ");

    for(int i=0;i<n;i++){string nm,g;int age;
        nm=readString("Passenger "+to_string(i+1)+" Name: ");
        age=readInt("Age: ",1,120);
        g=readString("Gender: ");
    }

    int subtotal = sel.price * n;
    int discount = 0;
    char chc = readChar("Have coupon(y/n)? ");
    if(chc=='y' || chc=='Y'){
        auto cp=coupons(); 
        cout<<"Available Coupons:\n";
        for(auto &p:cp) cout<<p.first<<" -> Rs."<<p.second<<" off\n";
        string code = readString("Enter Coupon Code: ");
        if(cp.count(code)){ 
            discount = cp[code];
            cout<<"Coupon applied! Discount = "<<discount<<"\n";
        } else {
            cout<<"Invalid coupon.\n";
        }
    }
    int price = max(0, subtotal - discount);
    cout<<"Subtotal:"<<subtotal<<" Discount:"<<discount<<" Total:"<<price<<"\n";

    char ready;
    while(true){
        ready = readChar("Ready to pay? (y/n): ");
        if(ready=='y'||ready=='Y') break;
        if(ready=='n'||ready=='N'){ cout<<"Returning to booking again...\n"; booking(mob); return; }
    }

    int pm = readInt("Payment Mode 1.UPI 2.NetBanking 3.Card\nChoice: ",1,3);
    if(pm==1){string upi=readString("Enter UPI Id: "); cout<<"Authenticating...\n";}
    else if(pm==2){string bank=readString("Bank: "); string otp=readString("Enter 4-digit OTP: "); }
    else if(pm==3){string card=readString("Card No: "); string otp=readString("Enter OTP: "); }

    cout<<"Processing...\n"; this_thread::sleep_for(1s);

    updateSeats(bid,cho,1);
    ofstream("bookings.csv",ios::app)
        <<rand()%10000<<","<<mob<<","<<s<<","<<d<<","<<date<<","<<bid<<","<<sel.name<<","<<price<<","<<nowTime()<<"\n";
    cout<<"Booking Successful!\n";
}
void booking(string mob){
    auto buses=readBuses();
    string s = readString("Source: ");
    string d = readString("Destination: ");
    string date = readString("Date (dd-mm-yyyy): ");
    s=lower(s); d=lower(d); date=normalizeDate(date);

    vector<Bus> res; 
    for(auto &b:buses) 
        if(lower(b.src)==s && lower(b.dst)==d && normalizeDate(b.date)==date) res.push_back(b);

    if(res.empty()){cout<<"No buses.\n";return;}
    cout<<"\nAvailable Buses:\n";
    for(auto &b:res) cout<<b.id<<" "<<b.name<<" "<<b.time<<" "<<b.type<<" "<<b.seat<<" Rs."<<b.price<<"\n";

    string bid = readString("Select Bus ID: ");
    Bus sel; bool found=false; 
    for(auto &b:res) if(b.id==bid){ sel=b; found=true; }
    if(!found){cout<<"Invalid Bus ID.\n"; return;}

    int n = readInt("Seats to book: ",1,24);
    auto m=seatMap(bid); cout<<"Available:"; 
    for(auto &p:m) if(p.second==0) cout<<p.first<<" "; 
    cout<<"\nSelect "<<n<<" seats: ";
    vector<int> cho(n); 
    for(int i=0;i<n;i++) cho[i]=readInt("Seat: ",1,24);

    string board = readString("Boarding: ");
    string drop = readString("Dropping: ");

    for(int i=0;i<n;i++){string nm,g;int age;
        nm=readString("Passenger "+to_string(i+1)+" Name: ");
        age=readInt("Age: ",1,120);
        g=readString("Gender: ");
    }

    int subtotal = sel.price * n;
    int discount = 0;
    char chc = readChar("Have coupon(y/n)? ");
    if(chc=='y' || chc=='Y'){
        auto cp=coupons(); 
        cout<<"Available Coupons:\n";
        for(auto &p:cp) cout<<p.first<<" -> Rs."<<p.second<<" off\n";
        string code = readString("Enter Coupon Code: ");
        if(cp.count(code)){ 
            discount = cp[code];
            cout<<"Coupon applied! Discount = "<<discount<<"\n";
        } else {
            cout<<"Invalid coupon.\n";
        }
    }
    int price = max(0, subtotal - discount);
    cout<<"Subtotal:"<<subtotal<<" Discount:"<<discount<<" Total:"<<price<<"\n";

    char ready;
    while(true){
        ready = readChar("Ready to pay? (y/n): ");
        if(ready=='y'||ready=='Y') break;
        if(ready=='n'||ready=='N'){ cout<<"Returning to booking again...\n"; booking(mob); return; }
    }

    int pm = readInt("Payment Mode 1.UPI 2.NetBanking 3.Card\nChoice: ",1,3);
    if(pm==1){string upi=readString("Enter UPI Id: "); cout<<"Authenticating...\n";}
    else if(pm==2){string bank=readString("Bank: "); string otp=readString("Enter 4-digit OTP: "); }
    else if(pm==3){string card=readString("Card No: "); string otp=readString("Enter OTP: "); }

    cout<<"Processing...\n"; this_thread::sleep_for(1s);

    updateSeats(bid,cho,1);
    ofstream("bookings.csv",ios::app)
        <<rand()%10000<<","<<mob<<","<<s<<","<<d<<","<<date<<","<<bid<<","<<sel.name<<","<<price<<","<<nowTime()<<"\n";
    cout<<"Booking Successful!\n";
}

int main(){
    seedData();
    string mob=init();
    while(true){
        int c = readInt("\n1.Booking 2.Booked Tickets 3.Edit Profile\nChoice: ",1,3);
        if(c==1) booking(mob);
        else if(c==2) booked(mob);
        else if(c==3) edit(mob);

        char a = readChar("Again go to menu? (y/n): ");
        if(a!='y' && a!='Y'){ 
            char x = readChar("Logout? (y/n): ");
            if(x=='y'||x=='Y') cout<<"Logged out successfully!!\n"; 
            else cout<<"Program Ended.\n"; 
            break; 
        }