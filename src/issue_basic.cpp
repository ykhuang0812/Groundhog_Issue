#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
#include <iomanip>
#include <algorithm>

#define LOWER_BOUND 0
#define UPPER_BOUND 1
using namespace std;

class RouteUser {
public:
    time_t timestamp;  // GMT+8 timestamp
    double lon;
    double lat;
};

class UserData{
private:
    vector<RouteUser> data={};
public:
    UserData(){}
    int ParseCSV(string file_name);
    void PrintData();
    int SearchPosition(time_t timestamp, int op);
    void LessThan(time_t timestamp);
    void GreaterThan(time_t timestamp);
    void Between(time_t ltimestamp, time_t utimestamp);
    void SortData();
};

time_t StrtoTimeStamp(string datetime) {
    struct tm tm;

    if(datetime.length()<19){
        cout << "datetime format wrong !!" << endl;
        return -1;
    }

    tm.tm_year = stoi(datetime.substr(0, 4)) - 1900;
    tm.tm_mon = stoi(datetime.substr(5, 2)) - 1;
	tm.tm_mday = stoi(datetime.substr(8, 2));
	tm.tm_hour = stoi(datetime.substr(11, 2));
	tm.tm_min = stoi(datetime.substr(14, 2));
	tm.tm_sec = stoi(datetime.substr(17, 2));

    return mktime(&tm);
}


int UserData::ParseCSV(string file_name) {
    ifstream user(file_name);
    if (!user.is_open()) {
        return -1;
    }
    string line;
    getline(user, line); //discard first line;
    while (getline(user,line)) {
        RouteUser route_user;
        stringstream ss;
        ss << line;
        string datetime, time, lon, lat;
        ss >> datetime >> time >> lon >> lat;
        datetime += " " + time;
        route_user.timestamp = StrtoTimeStamp(datetime);
        route_user.lon = stod(lon);
        route_user.lat = stod(lat);
        data.push_back(route_user);
    }
    return 0;
}

void UserData::PrintData() {
    for (int i = 0; i < data.size(); i++) {
        cout << data[i].timestamp << " " << fixed << setprecision(6) << data[i].lon << " " << data[i].lat << " \n";
    }
    return;
}

int UserData::SearchPosition(time_t timestamp, int bound) { // op -> 0: return lower bound , 1: return upper bound
    int l = 0, h = data.size() - 1;
    switch(bound){
        case LOWER_BOUND:
            while(l <= h){
                int m = l + (h - l) / 2;
                if (data[m].timestamp < timestamp) {
                    l = m + 1;
                } else if (data[m].timestamp >= timestamp){
                    h = m - 1;
                }
            }
            break;
        case UPPER_BOUND:
            while(l <= h){
                int m = l + (h - l) / 2;
                if (data[m].timestamp <= timestamp) {
                    l = m + 1;
                } else if (data[m].timestamp > timestamp){
                    h = m - 1;
                }
            }
            break;
    }
    return l;
}

void UserData::LessThan(time_t timestamp) {
    int pos = SearchPosition(timestamp, LOWER_BOUND);
    for (int i = 0; i < pos; i++) {
        cout << data[i].timestamp << " " << fixed << setprecision(6) << data[i].lon << " " << data[i].lat << " \n";
    }
    return;
}

void UserData::GreaterThan(time_t timestamp) {
    int pos = SearchPosition(timestamp, UPPER_BOUND);
    for (int i = pos; i < data.size(); i++) {
        cout << data[i].timestamp << " " << fixed << setprecision(6) << data[i].lon << " " << data[i].lat << " \n";
    }
    return;
}

void UserData::Between(time_t ltimestamp , time_t utimestamp) {
    int lpos = SearchPosition(ltimestamp, 0);
    int upos = SearchPosition(utimestamp, 1);
    for (int i = lpos; i < upos; i++) {
        cout << data[i].timestamp << " " << fixed << setprecision(6) << data[i].lon << " " << data[i].lat << " \n";
    }
    return;
}

void UserData::SortData(){
    sort(data.begin(), data.end(), [&](RouteUser x, RouteUser y) { return x.timestamp < y.timestamp; });
}

int main(){
    UserData UserData[2]; // 0:route_user_1.csv  1:route_user2.csv
    vector<string> filename = {"../data/route_user_1.csv", "../data/route_user_2.csv"};
    for (int i = 0; i < 2; i++) {
        if (UserData[i].ParseCSV(filename[i]) < 0) {
            cout << "Parse " << filename[i] << " failed.\n";
        } else {
            UserData[i].SortData(); // Spec says the data are sorted, but I found route_user_1.csv is not sorted
        }
    }

    //UserData[0].PrintData();
    //UserData[0].LessThan(1511398745);
    //UserData[0].GreaterThan(1511398787);
    //UserData[0].Between(1511398745, 1511398787);

    return 0;
}