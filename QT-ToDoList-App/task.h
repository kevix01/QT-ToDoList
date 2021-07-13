#ifndef TASK_H
#define TASK_H
#include <iostream>
using namespace std;

class Task
{
public:
    Task(const string& title, const string& description, const string& completePercent, const string& date);
    const string& getTitle(){return this->title;}
    const string& getDescription(){return this->description;}
    const string& getCompletePercent(){return this->completePercent;}
    const string& getDate(){return this->date;}
    void modify(const string &duedate, const string &title, const string &percent, const string &description); //modify task's data

private:
    string title;
    string description;
    string completePercent;
    string date;
};

#endif // TASK_H
