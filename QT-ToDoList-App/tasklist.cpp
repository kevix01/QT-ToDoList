#include "tasklist.h"

void TaskList::reset(){
    list<unique_ptr<Task>>::iterator itr = tasks.begin();
    while(itr != tasks.end()){
        Task* task = itr->release();
        delete task;
        itr = tasks.erase(itr);
    }
    this->unDoneTasks = 0;
    this->name = "";
}

void TaskList::insertTask(unique_ptr<Task>& task){
    if(task->getCompletePercent() != 100)
        unDoneTasks++;
    tasks.push_back(move(task));
    notifyGui();
}

void TaskList::addNewTask(unique_ptr<Task>& task){
    if(task->getCompletePercent() != 100)
        unDoneTasks++;
    tasks.push_back(move(task));
}

void TaskList::registerObserver(Observer *observer){
    this->gui = observer;
}

void TaskList::removeObserver(){
    this->gui = nullptr;
}

void TaskList::loadTasksData(IOManager &mng) const{
    for(auto& itr : tasks){
        string* temp = new string[5];
        temp[0] = itr->getStringDate();
        temp[1] = itr->getTitle();
        temp[2] = to_string(itr->getCompletePercent());
        temp[3] = itr->getDescription();
        temp[4] = this->name;
        mng.data.push_back(temp);
    }
}

void TaskList::modifyTask(unique_ptr<Task>& task, const Task::Date& duedate, const string& title, int percent, const string& description){
    int oldCompletePercent = task->getCompletePercent();
    task->modify(duedate,title,percent,description);
    if(oldCompletePercent == 100 && task->getCompletePercent() != 100)
        unDoneTasks++;
    else if(oldCompletePercent != 100 && task->getCompletePercent() == 100)
        unDoneTasks--;
    notifyGui();
}

void TaskList::removeTask(unique_ptr<Task> &task){
    if(task->getCompletePercent() != 100)
        unDoneTasks--;
    tasks.remove(task);
    if(tasks.size() == 0){
        this->name = "";
        gui->decreaseActiveSubject();
    }
}

void TaskList::deleteTask(unique_ptr<Task>& task){
    if(task->getCompletePercent() != 100)
        unDoneTasks--;
    tasks.remove(task);
    if(tasks.size() == 0){
        gui->decreaseActiveSubject(this->name);
    }
}

void TaskList::notifyGui(){
    if(gui)
        gui->updateOutput();
}

TaskList::~TaskList(){}
