#ifndef TASK_H
#define TASK_H
#include <iostream>
using namespace std;

class Task
{
public:
    struct Date{
    public:
        Date() {}
        Date(int year, int month, int day) : year(year), month(month), day(day) {}
        bool operator ==(const Date& right) const{
            if(this->year == right.year && this->month == right.month && this->day == right.day)
                return true;
            else
                return false;
        }
        int year;
        int month;
        int day;
    };
    Task(const string& title, const string& description, int completePercent, const Date& date);
    const string& getTitle(){return this->title;}
    const string& getDescription(){return this->description;}
    const int& getCompletePercent() const;
    const Date &getDate() const;
    const string getStringDate() const;
    void modify(const Date& duedate, const string &title, int percent, const string &description); //modify task's data

private:
    string title;
    string description;
    int completePercent;
    Date date;
};

#endif // TASK_H
