#include "tasklist.h"
/*
string& TaskList::getName() {
    return this->name;
}
*/
/*void TaskList::setName(const string& name){
    this->name = name;
}*/

void TaskList::reset(){
    list<Task*>::iterator itr = tasks.begin();
    while(itr != tasks.end()){
        delete *itr;
        itr = tasks.erase(itr);
    }
    this->name = "";
}

void TaskList::addNewTask(const string *data){
    tasks.emplace_back(new Task(data[1], data[3], data[2], data[0]));
}

void TaskList::addNewTask(const string &duedate, const string &title, const string &percent, const string &description){
    tasks.emplace_back(new Task(title,description,percent,duedate));
    notifyGui();
}

void TaskList::registerObserver(Observer *observer){
    this->gui = observer;
}

void TaskList::removeObserver(){
    this->gui = nullptr;
}

void TaskList::loadTasksData(IOManager &mng) const{
    for(auto itr : tasks){
        string* temp = new string[5];
        temp[0] = itr->getDate();
        temp[1] = itr->getTitle();
        temp[2] = itr->getCompletePercent();
        temp[3] = itr->getDescription();
        temp[4] = this->name;
        mng.data.push_back(temp);
    }
}

void TaskList::modifyTask(const string &oldDueDate, const string &oldTitle, const string &oldPercent, const string &oldDescription,
                          const string &duedate, const string &title, const string &percent, const string &description){
    for(auto itr : tasks)
        if(itr->getTitle() == oldTitle && itr->getDate() == oldDueDate && itr->getCompletePercent() == oldPercent && itr->getDescription() == oldDescription){
            itr->modify(duedate,title,percent,description);
            break;
        }
    notifyGui();
}

Task* TaskList::removeTask(const string &oldDueDate, const string &oldTitle, const string &oldPercent, const string &oldDescription){
    for(auto itr : tasks)
        if(itr->getTitle() == oldTitle && itr->getDate() == oldDueDate && itr->getCompletePercent() == oldPercent && itr->getDescription() == oldDescription){
            tasks.remove(itr);
            if(tasks.size() == 0){
                this->name = "";
                gui->decreaseActiveSubject();
            }
            return itr;
        }
    return nullptr;
}

void TaskList::deleteTask(const string &oldDueDate, const string &oldTitle, const string &oldPercent, const string &oldDescription){
    for(auto itr : tasks)
        if(itr->getTitle() == oldTitle && itr->getDate() == oldDueDate && itr->getCompletePercent() == oldPercent && itr->getDescription() == oldDescription){
            tasks.remove(itr);
            delete itr;
            if(tasks.size() == 0){
                gui->decreaseActiveSubject(this->name);
            }
            break;
        }
}

void TaskList::notifyGui(){
    if(gui)
        gui->updateOutput();
}

TaskList::~TaskList(){
    for(auto itr : tasks)
        delete itr;
}
