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