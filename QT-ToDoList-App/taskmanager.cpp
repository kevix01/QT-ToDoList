#include <memory>
#include "taskmanager.h"
#include "ui_taskmanager.h"
#include "task.h"

TaskManager::TaskManager(QString dialogTitle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewTaskDialog)
{
    ui->setupUi(this);

    ui->datepicker->setDisplayFormat("yyyy/MM/dd");

    QDate date = QDate::currentDate();
    ui->datepicker->setDate(date);

    ui->slider->setRange(0, 100);
    ui->slider->setSingleStep(5);

    ui->savebtn->setEnabled(false);
    ui->deletebtn->setVisible(false);

    this->setWindowTitle(dialogTitle);
    this->newTask = true;
}

void TaskManager::setPath(const string& path)
{
    this->path = path;
}

void TaskManager::loadData(const string& duedate, const string& title, const string& percent, const string& description, const string& list)
{
    this->oldDuedate = duedate;
    this->oldTitle = title;
    this->oldPercent = percent;
    this->oldDescription = description;
    this->oldList = list;
    vector<string> v = IOManager::split(duedate, '/');
    int i=0;
    unique_ptr<string[]> sdate(new string[v.size()]);
    for(auto it : v){
        sdate[i] = v[i];
        i++;
    }
    unique_ptr<QDate> sd(new QDate(stoi(sdate[0]), stoi(sdate[1]), stoi(sdate[2])));
    ui->comboBox->setCurrentIndex(ui->comboBox->findText(QString::fromStdString(list)));
    ui->datepicker->setDate(*sd);
    ui->title_et->setText(QString::fromStdString(title));
    ui->slider->setValue(stoi(percent));
    ui->description_ed->setPlainText(QString::fromStdString(description));

    this->newTask = false;
    ui->savebtn->setText("Modify");
    ui->deletebtn->setVisible(true);
}

void TaskManager::loadLists(){
    if(origin->l1->getName() != "")
        ui->comboBox->addItem(QString::fromStdString(origin->l1->getName()));
    if(origin->l2->getName() != "")
        ui->comboBox->addItem(QString::fromStdString(origin->l2->getName()));
    if(origin->l3->getName() != "")
        ui->comboBox->addItem(QString::fromStdString(origin->l3->getName()));
    if(origin->l4->getName() != "")
        ui->comboBox->addItem(QString::fromStdString(origin->l4->getName()));
    if(origin->l5->getName() != "")
        ui->comboBox->addItem(QString::fromStdString(origin->l5->getName()));
}


TaskManager::~TaskManager()
{
    delete ui;
}


void TaskManager::on_slider_valueChanged(int value)
{
    ui->percent_lbl->setText(QString::fromStdString(to_string(value)+"% Compl."));
    checkFields();
}


void TaskManager::checkFields(){
    string sdatepicker(ui->datepicker->text().toUtf8().constData());
    string stitle(ui->title_et->text().toUtf8().constData());
    string spercent(ui->percent_lbl->text().toUtf8().constData());
    spercent = spercent.substr(0, spercent.find('%'));
    string sdescr(ui->description_ed->toPlainText().toUtf8().constData());

    //Checks if all fields are completed
    if(sdatepicker.length()!=10 || stitle.length()==0 || spercent.length()==0 || sdescr.length()==0)
        ui->savebtn->setEnabled(false);
    else
        ui->savebtn->setEnabled(true);
}
void TaskManager::on_description_ed_textChanged()
{
    checkFields();
}

void TaskManager::on_title_et_textChanged()
{
    checkFields();
}

void TaskManager::on_datepicker_userDateChanged()
{
    checkFields();
}

void TaskManager::on_savebtn_clicked()
{
    IOManager mng;
    vector<string*> lines = IOManager::readFile(path,mng);
    unsigned int i;
    string membershipList(ui->comboBox->currentText().toUtf8().constData());
    string sdatepicker(ui->datepicker->text().toUtf8().constData());
    string stitle(ui->title_et->text().toUtf8().constData());
    string spercent(ui->percent_lbl->text().toUtf8().constData());
    spercent = spercent.substr(0, spercent.find('%'));
    string sdescr(ui->description_ed->toPlainText().toUtf8().constData());
    string s = "";

    if(sdescr.find(IOManager::regexChar) != std::string::npos || stitle.find(IOManager::regexChar) != std::string::npos){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Saving failed");
        QString errorText = QString("'%1' character not allowed! \nPlease remove it and retry.").arg(IOManager::regexChar);
        msgBox.setText(errorText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        if(msgBox.exec() == QMessageBox::Ok)
          return;
    }
    else if(sdescr.find("\n") != std::string::npos){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Saving failed");
        msgBox.setText("'Enter'/'New Line' character not allowed! \nWrite description in the same line and retry.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        if(msgBox.exec() == QMessageBox::Ok)
          return;
    }
    else{
        if(newTask){
            for(i=0; i<lines.size(); i++){
                s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+IOManager::regexChar+lines.at(i)[4]+"\n";
            }
            s += sdatepicker+IOManager::regexChar+stitle+IOManager::regexChar+spercent+IOManager::regexChar+sdescr+IOManager::regexChar+membershipList;
            IOManager dateMng;
            dateMng.dates = IOManager::split(sdatepicker, '/');
            Task::Date date(stoi(dateMng.dates[0]), stoi(dateMng.dates[1]), stoi(dateMng.dates[2]));

            std::unique_ptr<Task> task(new Task(stitle,sdescr,stoi(spercent),date));

            if(origin->l1->getName() == membershipList){
                origin->l1->insertTask(task);
            }
            else if(origin->l2->getName() == membershipList){
                origin->l2->insertTask(task);
            }
            else if(origin->l3->getName() == membershipList){
                origin->l3->insertTask(task);
            }
            else if(origin->l4->getName() == membershipList){
                origin->l4->insertTask(task);
            }
            else if(origin->l5->getName() == membershipList){
                origin->l5->insertTask(task);
            }
        }else{
            for(i=0; i<lines.size(); i++){
                if(lines.at(i)[0].compare(oldDuedate) == 0 && lines.at(i)[1].compare(oldTitle) == 0 && lines.at(i)[2].compare(oldPercent) == 0 &&
                        lines.at(i)[3].compare(oldDescription) == 0 && lines.at(i)[4].compare(oldList) == 0){
                    s += sdatepicker+IOManager::regexChar+stitle+IOManager::regexChar+spercent+IOManager::regexChar+sdescr+IOManager::regexChar+membershipList+"\n";
                }else{
                    s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+IOManager::regexChar+lines.at(i)[4]+"\n";
                }
            }
            if(oldList == membershipList)
                editTaskNoListChange(sdatepicker,stitle,spercent,sdescr);
            else
                editTaskWithListChange(sdatepicker,stitle,spercent,sdescr,membershipList);
        }
        IOManager::writeFile(path, s);
        lines.clear();
        this->close();
    }
}

void TaskManager::editTaskNoListChange(const string &duedate, const string &title, const string& percent, const string &description) const{
    IOManager dateMng;
    dateMng.dates = IOManager::split(duedate, '/');
    Task::Date date(stoi(dateMng.dates[0]), stoi(dateMng.dates[1]), stoi(dateMng.dates[2]));

    IOManager oldDateMng;
    dateMng.dates = IOManager::split(oldDuedate, '/');
    Task::Date oldDate(stoi(dateMng.dates[0]), stoi(dateMng.dates[1]), stoi(dateMng.dates[2]));

    if(origin->l1->getName() == oldList){
        for(auto& itr : origin->l1->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l1->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
    else if(origin->l2->getName() == oldList){
        for(auto& itr : origin->l2->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l2->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
    else if(origin->l3->getName() == oldList){
        for(auto& itr : origin->l3->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l3->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
    else if(origin->l4->getName() == oldList){
        for(auto& itr : origin->l4->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l4->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
    else if(origin->l5->getName() == oldList){
        for(auto& itr : origin->l5->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l5->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
}

void TaskManager::moveTaskToDestinationList(unique_ptr<Task> &task, const string &listName) const{
    //add task in destination list
    if(origin->l1->getName() == listName){
        origin->l1->addNewTask(task);
    }
    else if(origin->l2->getName() == listName){
        origin->l2->addNewTask(task);
    }
    else if(origin->l3->getName() == listName){
        origin->l3->addNewTask(task);
    }
    else if(origin->l4->getName() == listName){
        origin->l4->addNewTask(task);
    }
    else if(origin->l5->getName() == listName){
        origin->l5->addNewTask(task);
    }
}

void TaskManager::editTaskWithListChange(const string &duedate, const string &title, const string &percent, const string &description, const string &list) const{
    IOManager dateMng;
    dateMng.dates = IOManager::split(duedate, '/');
    Task::Date date(stoi(dateMng.dates[0]), stoi(dateMng.dates[1]), stoi(dateMng.dates[2]));

    IOManager oldDateMng;
    oldDateMng.dates = IOManager::split(oldDuedate, '/');
    Task::Date oldDate(stoi(oldDateMng.dates[0]), stoi(oldDateMng.dates[1]), stoi(oldDateMng.dates[2]));

    //remove task from old list
    if(origin->l1->getName() == oldList){
        for(auto& itr : origin->l1->getTasksList())
                if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                    moveTaskToDestinationList(itr,list);
                    origin->l1->removeTask(itr);
                    break;
                }
    }
    else if(origin->l2->getName() == oldList){
        for(auto& itr : origin->l2->getTasksList())
                if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                    moveTaskToDestinationList(itr,list);
                    origin->l2->removeTask(itr);
                    break;
                }
    }
    else if(origin->l3->getName() == oldList){
        for(auto& itr : origin->l3->getTasksList())
                if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                    moveTaskToDestinationList(itr,list);
                    origin->l3->removeTask(itr);
                    break;
                }
    }
    else if(origin->l4->getName() == oldList){
        for(auto& itr : origin->l4->getTasksList())
                if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                    moveTaskToDestinationList(itr,list);
                    origin->l4->removeTask(itr);
                    break;
                }
    }
    else if(origin->l5->getName() == oldList){
        for(auto& itr : origin->l5->getTasksList())
                if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                    moveTaskToDestinationList(itr,list);
                    origin->l5->removeTask(itr);
                    break;
                }
    }

    //Task modification
    if(origin->l1->getName() == list){
        for(auto& itr : origin->l1->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l1->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
    else if(origin->l2->getName() == list){
        for(auto& itr : origin->l2->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l2->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
    else if(origin->l3->getName() == list){
        for(auto& itr : origin->l3->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l3->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
    else if(origin->l4->getName() == list){
        for(auto& itr : origin->l4->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l4->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
    else if(origin->l5->getName() == list){
        for(auto& itr : origin->l5->getTasksList())
            if(itr->getTitle() == oldTitle && itr->getDate() == oldDate && itr->getCompletePercent() == stoi(oldPercent) && itr->getDescription() == oldDescription){
                origin->l5->modifyTask(itr,date,title,stoi(percent),description);
                break;
            }
    }
}
void TaskManager::on_cancelbtn_clicked()
{
    this->close();
}

void TaskManager::on_deletebtn_clicked()
{
    IOManager mng;
    vector<string*> lines = IOManager::readFile(path,mng);
    unsigned int i;

    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Delete task", "Are you sure you want to permanently delete this task?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No) {
          return;
      }

    string s = "";

    for(i=0; i<lines.size(); i++){
        if(!(lines.at(i)[0].compare(oldDuedate) == 0 && lines.at(i)[1].compare(oldTitle) == 0 && lines.at(i)[2].compare(oldPercent) == 0 &&
             lines.at(i)[3].compare(oldDescription) == 0 && lines.at(i)[4].compare(oldList) == 0)){
            s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+IOManager::regexChar+lines.at(i)[4]+"\n";
        }
    }
    IOManager::writeFile(path, s);
    lines.clear();
    origin->deleteSingleTask(oldList,oldDuedate,oldTitle,oldPercent,oldDescription);
    origin->updateOutput();
    this->close();
}
