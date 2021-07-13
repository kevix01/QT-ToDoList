#ifndef TASKLIST_H
#define TASKLIST_H
#include <iostream>
#include <list>
#include "task.h"
#include "observer.h"
#include "iomanager.h"
using namespace std;

class TaskList
{
public:
    explicit TaskList(const string& name = "") : name(name){} //constructor of tasklist (default name of "" if unused)
    void reset();
    void addNewTask(const string* data); //allocate and insert in the list a new task with input data of an array of strings
    void addNewTask(const string &duedate, const string &title, const string &percent, const string &description); //allocate and insert in the list a new task with input data of strings
    void registerObserver(Observer* observer);
    void removeObserver();
    const string& getName() const{ return this->name; }
    void loadTasksData(IOManager& mng) const;
    void setName(const string& name){ this->name = name; }
    void modifyTask(const string& oldDueDate, const string& oldTitle, const string& oldPercent, const string& oldDescription,
            const string &duedate, const string &title, const string &percent, const string &description);
    Task* removeTask(const string& oldDueDate, const string& oldTitle, const string& oldPercent, const string& oldDescription); //remove task from list and return a pointer to it
    void insertTask(Task* task){ tasks.push_back(task); }                                                                       //in order to transfer it into another list
    void deleteTask(const string& oldDueDate, const string& oldTitle, const string& oldPercent, const string& oldDescription); //delete task from list freeing memory allocated
    list<Task*>& getTasksList(){ return this->tasks; }
    ~TaskList();
private:
    void notifyGui(); //call the update method of observer
private:
    string name;
    list<Task*> tasks;
    Observer* gui {nullptr};
};

#endif // TASKLIST_H
