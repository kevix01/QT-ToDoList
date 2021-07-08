#ifndef TASKLIST_H
#define TASKLIST_H
#include <iostream>
#include <list>
#include "task.h"
using namespace std;

class TaskList
{
public:
    explicit TaskList(const string& name = "") : name(name){}
    void reset(){
        list<Task*>::iterator itr = tasks.begin();
        while(itr != tasks.end()){
            delete *itr;
            itr = tasks.erase(itr);
        }
        this->name = "";
    }
    void addNewTask(const string* data){
        tasks.emplace_back(new Task(data[1], data[3], data[2], data[0]));
    }
    const string& getName() const{
        return this->name;
    }
    void setName(const string& name){
        this->name = name;
    }
    //void resetList();
private:
    string name;
    list<Task*> tasks;
};

#endif // TASKLIST_H
