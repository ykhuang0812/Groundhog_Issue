#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
#include <iomanip>
#include <algorithm>

using namespace std;

class RouteUser {
public:
    time_t timestamp;  // GMT+8 timestamp
    double lon;
    double lat;
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


int ParseCSV(string file_name, vector<RouteUser> &data) {
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

void PrintData(vector<RouteUser> &data) {
    for (int i = 0; i < data.size(); i++) {
        cout << data[i].timestamp << " " << fixed << setprecision(6) << data[i].lon << " " << data[i].lat << " \n";
    }
    return;
}

int SearchPosition(vector<RouteUser> &data, time_t timestamp, int op) { // op -> 0: return lower bound , 1: return upper bound
    int l = 0, h = data.size() - 1;
    switch(op){
        case 0:
            while(l <= h){
                int m = l + (h - l) / 2;
                if (data[m].timestamp < timestamp) {
                    l = m + 1;
                } else if (data[m].timestamp >= timestamp){
                    h = m - 1;
                }
            }
            break;
        case 1:
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

void LessThan(vector<RouteUser> &data, time_t timestamp) {
    int pos = SearchPosition(data, timestamp, 0);
    for (int i = 0; i < pos; i++) {
        cout << data[i].timestamp << " " << fixed << setprecision(6) << data[i].lon << " " << data[i].lat << " \n";
    }
    return;
}

void GreaterThan(vector<RouteUser> &data, time_t timestamp) {
    int pos = SearchPosition(data, timestamp, 1);
    for (int i = pos; i < data.size(); i++) {
        cout << data[i].timestamp << " " << fixed << setprecision(6) << data[i].lon << " " << data[i].lat << " \n";
    }
    return;
}

void Between(vector<RouteUser> &data, time_t ltimestamp , time_t utimestamp) {
    int lpos = SearchPosition(data, ltimestamp, 0);
    int upos = SearchPosition(data, utimestamp, 1);
    for (int i = lpos; i < upos; i++) {
        cout << data[i].timestamp << " " << fixed << setprecision(6) << data[i].lon << " " << data[i].lat << " \n";
    }
    return;
}

int main(){
    vector<RouteUser> data[2]; // 0:route_user_1.csv  1:route_user2.csv
    vector<string> filename = {"route_user_1.csv", "route_user_2.csv"};
    for (int i = 0; i < 2; i++) {
        if (ParseCSV(filename[i],data[i]) < 0) {
            cout << "Parse " << filename[i] << " failed.\n";
        }
        sort(data[i].begin(), data[i].end(), [&](RouteUser x, RouteUser y) { return x.timestamp < y.timestamp; }); // Spec says the data is sorted, but I found route_user_1.csv is not sorted
    }

    //PrintData(data[0]);
    //LessThan(data[0], 1511398745);
    //GreaterThan(data[0], 1511398787);
    //Between(data[0], 1511398745, 1511398787);

    return 0;
}