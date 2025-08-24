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