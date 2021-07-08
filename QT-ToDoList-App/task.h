#ifndef TASK_H
#define TASK_H
#include <iostream>
using namespace std;

class Task
{
public:
    Task(const string& title, const string& description, const string& completePercent, const string& date)
        : title(title), description(description), completePercent(completePercent), date(date){}
private:
    string title;
    string description;
    string completePercent;
    string date;
};

#endif // TASK_H
