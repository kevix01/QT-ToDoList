#include "task.h"

Task::Task(const string& title, const string& description, const string& completePercent, const string& date)
         : title(title), description(description), completePercent(completePercent), date(date){}

void Task::modify(const string &duedate, const string &title, const string &percent, const string &description)
{
    this->title = title;
    this->description = description;
    this->date = duedate;
    this->completePercent = percent;
}
