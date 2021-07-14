#ifndef TASKLIST_H
#define TASKLIST_H
#include <iostream>
#include <list>
#include <memory>
#include <algorithm>
#include "task.h"
#include "observer.h"
#include "iomanager.h"
using namespace std;

class TaskList
{
public:
    explicit TaskList(const string& name = "") : name(name){} //constructor of tasklist (default name of "" if unused)
    void reset(); //reset list deleting all related tasks (freeing allocated memory)
    void addNewTask(unique_ptr<Task>& task);
    void registerObserver(Observer* observer);
    void removeObserver();
    const string& getName() const{ return this->name; }
    void loadTasksData(IOManager& mng) const; //load all tasks' data of the list
    void setName(const string& name){ this->name = name; }
    void modifyTask(unique_ptr<Task>& task, const Task::Date& duedate, const string& title, int percent, const string& description);
    void removeTask(unique_ptr<Task>& task); //remove task from list for list change
    void insertTask(unique_ptr<Task>& task); //insert task and notify the observer
    void deleteTask(unique_ptr<Task>& task); //delete task from list for permanent delete
    list<unique_ptr<Task>>& getTasksList(){ return this->tasks; }
    const int& getUndoneTasks(){ return this->unDoneTasks; }
    ~TaskList();
private:
    void notifyGui(); //call the update method of observer
private:
    string name;
    list<unique_ptr<Task>> tasks;
    int unDoneTasks {0};
    Observer* gui {nullptr};
};

#endif // TASKLIST_H
