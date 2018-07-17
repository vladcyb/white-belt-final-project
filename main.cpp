#include <iostream>
#include <map>
#include <set>
#include <iomanip>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

class Date {
public:
    Date(int y, int m, int d){
        year = y;
        month = m;
        day = d;
    }
    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }
    void SetYear(int y){
        year = y;
    }
    void SetMonth(int m){
        month = m;
    }
    void SetDay(int d){
        day = d;
    }

private:
    int year;
    int month;
    int day;
};

ostream& operator<<(ostream& stream, const Date& date){
    stream << setw(4) << setfill('0') << date.GetYear()
     << '-' << setw(2) << date.GetMonth() << '-' <<
     setw(2) << date.GetDay();
    return stream;
}

void EnsureNextSymbolAndSkip(stringstream& ss){
    if (ss.peek() != '-'){
        throw runtime_error("Wrong date format: " + ss.str());
    }
    ss.ignore(1);
}

void ThrowDateFormatError(const string& s){
    throw runtime_error("Wrong date format: " + s);
}

Date ParseDate(const string& s){
    stringstream ss(s);
    int y, m, d;
    if(!(ss >> y)){
        ThrowDateFormatError(ss.str());
    }
    EnsureNextSymbolAndSkip(ss);
    if(!(ss >> m)){
        ThrowDateFormatError(ss.str());
    }
    EnsureNextSymbolAndSkip(ss);
    if(!(ss >> d)){
        ThrowDateFormatError(ss.str());
    }
    if(!ss.eof()){
        throw runtime_error("Wrong date format: " + ss.str());
    }
    if (m < 1 || m > 12){
        throw runtime_error("Month value is invalid: " + to_string(m) + "\n");
    }
    if (d < 1 || d > 31){
        throw runtime_error("Day value is invalid: " + to_string(d) + "\n");
    }
    return Date(y, m, d);
}

bool operator<(const Date& lhs, const Date& rhs){
    return make_tuple(lhs.GetYear(), lhs.GetMonth(), lhs.GetDay()) < make_tuple(rhs.GetYear(), rhs.GetMonth(), rhs.GetDay());
}

class Database {
public:
    void AddEvent(const Date& date, const string& event){
        calendar[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event){
        if(calendar[date].find(event) != calendar[date].end()){
            calendar[date].erase(event);
            return true;
        }
        return false;
    }
    int DeleteDate(const Date& date){
        int sz = calendar[date].size();
        calendar.erase(date);
        return sz;
    }

    vector<string> Find(const Date& date) const {
        vector<string> v;
        if(calendar.find(date) != calendar.end()){
            for(const string& event : calendar.at(date)){
                v.push_back(event);
            }
        }
        return v;
    }

    void Print() const {
        for(const auto& [date, events] : calendar){
            for(const string& event : events){
                cout << date << ' ' << event << endl;
            }
        }
    }

private:
    map<Date, set<string>> calendar;
};

int main() {
    Database db;
    string command;
    while(getline(cin, command)){
        try{
            stringstream ss(command);
            ss >> command;
            if(command.empty()){
                continue;
            }
            if(command == "Add"){
                string s;
                ss >> s;
                string event;
                ss >> event;
                db.AddEvent(ParseDate(s), event);
            }else if(command == "Print"){
                db.Print();
            }else if(command == "Del"){
                string s1, s2;
                ss >> s1 >> s2;
                Date date = ParseDate(s1);
                if(s2.empty()){
                    int res = db.DeleteDate(date);
                    cout << "Deleted " << res << " events." << endl;
                }else{
                    if(db.DeleteEvent(date, s2)){
                        cout << "Deleted successfully" << endl;
                    }else{
                        cout << "Event not found" << endl;
                    }
                }
            }else if(command == "Find"){
                string s;
                ss >> s;
                auto events = db.Find(ParseDate(s));
                for(const auto& event : events){
                    cout << event << endl;
                }
            }else{
                throw runtime_error("Unknown command: " + command);
            }
        }catch(runtime_error& e){
            cout << e.what() << endl;
        }
    }


    return 0;
}