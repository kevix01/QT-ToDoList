#include "task.h"

Task::Task(const string& title, const string& description, int completePercent, const Date& date)
         : title(title), description(description), completePercent(completePercent), date(date.year, date.month, date.day){
    /*this->date.year = date.year;
    this->date.month = date.month;
    this->date.day = date.day;*/
}

void Task::modify(const Date& duedate, const string &title, int percent, const string &description)
{
    this->title = title;
    this->description = description;
    this->date.year = duedate.year;
    this->date.month = duedate.month;
    this->date.day = duedate.day;
    this->completePercent = percent;
}

const Task::Date &Task::getDate() const{
    return date;
}

const string Task::getStringDate() const{
    if(date.month >= 10 && date.day <= 10)
        return to_string(date.year)+'/'+to_string(date.month)+"/0"+to_string(date.day);
    else if(date.month <= 10 && date.day >= 10)
        return to_string(date.year)+"/0"+to_string(date.month)+'/'+to_string(date.day);
    else if(date.month <= 10 && date.day <= 10)
        return to_string(date.year)+"/0"+to_string(date.month)+"/0"+to_string(date.day);
    else
        return to_string(date.year)+'/'+to_string(date.month)+'/'+to_string(date.day);
}

const int &Task::getCompletePercent() const{
    return completePercent;
}

