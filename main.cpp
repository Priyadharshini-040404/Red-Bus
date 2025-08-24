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