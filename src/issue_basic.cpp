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

class UserData {
public:
    vector<RouteUser> data={};

    UserData(){}
    int ParseCSV(string file_name);
    void PrintData();
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

void UserData::SortData() {
    sort(data.begin(), data.end(), [&](RouteUser x, RouteUser y) { return x.timestamp < y.timestamp; });
}

int lower_bound(UserData &user, time_t timestamp) {
    int l = 0, h = user.data.size() - 1;
    while (l <= h) {
        int m = l + (h - l) / 2;
        if (user.data[m].timestamp < timestamp) {
            l = m + 1;
        } else if (user.data[m].timestamp >= timestamp) {
            h = m - 1;
        }
    }
    return l;
}

int upper_bound(UserData &user, time_t timestamp) {
    int l = 0, h = user.data.size() - 1;
    while (l <= h) {
        int m = l + (h - l) / 2;
        if (user.data[m].timestamp <= timestamp) {
            l = m + 1;
        } else if (user.data[m].timestamp > timestamp) {
            h = m - 1;
        }
    }
    return l;
}

UserData LessThan(UserData &user, time_t timestamp) {
    UserData ret;
    int pos = lower_bound(user,timestamp);
    for (int i = 0; i < pos; i++) {
        ret.data.push_back(user.data[i]);
    }
    return ret;
}

UserData GreaterThan(UserData &user, time_t timestamp) {
    UserData ret;
    int pos = upper_bound(user,timestamp);
    for (int i = pos; i < user.data.size(); i++) {
        ret.data.push_back(user.data[i]);
    }
    return ret;
}

UserData Between(UserData &user, time_t ltimestamp , time_t utimestamp) {
    UserData ret;
    int lpos = lower_bound(user,ltimestamp);
    int upos = upper_bound(user,utimestamp);
    for (int i = lpos; i < upos; i++) {
        ret.data.push_back(user.data[i]);
    }
    return ret;
}

int main() {
    UserData user[2]; // 0:route_user_1.csv  1:route_user2.csv
    vector<string> filename = {"../data/route_user_1.csv", "../data/route_user_2.csv"};
    for (int i = 0; i < 2; i++) {
        if (user[i].ParseCSV(filename[i]) < 0) {
            cout << "Parse " << filename[i] << " failed.\n";
        } else {
            user[i].SortData(); // Spec says the data are sorted, but I found route_user_1.csv is not sorted
        }
    }
    UserData user_Less, user_Greater, user_Between;
    user_Less = LessThan(user[0],1511398745);
    user_Greater = GreaterThan(user[0],1511398787);
    user_Between = Between(user[0],1511398745, 1511398787);
    //user_Less.PrintData();
    //user_Between.PrintData();
    //user_Greater.PrintData();

    return 0;
}