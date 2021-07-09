#include <memory>
#include "taskmanager.h"
#include "ui_taskmanager.h"

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

            if(origin->l1->getName() == membershipList){
                origin->l1->addNewTask(sdatepicker,stitle,spercent,sdescr);
            }
            else if(origin->l2->getName() == membershipList){
                origin->l2->addNewTask(sdatepicker,stitle,spercent,sdescr);
            }
            else if(origin->l3->getName() == membershipList){
                origin->l3->addNewTask(sdatepicker,stitle,spercent,sdescr);
            }
            else if(origin->l4->getName() == membershipList){
                origin->l4->addNewTask(sdatepicker,stitle,spercent,sdescr);
            }
            else if(origin->l5->getName() == membershipList){
                origin->l5->addNewTask(sdatepicker,stitle,spercent,sdescr);
            }
        }else{
            for(i=0; i<lines.size(); i++){
                if(lines.at(i)[0].compare(oldDuedate) == 0 && lines.at(i)[1].compare(oldTitle) == 0 &&
                        lines.at(i)[2].compare(oldPercent) == 0 && lines.at(i)[3].compare(oldDescription) == 0 && lines.at(i)[4].compare(oldList) == 0){
                    s += sdatepicker+IOManager::regexChar+stitle+IOManager::regexChar+spercent+IOManager::regexChar+sdescr+IOManager::regexChar+membershipList+"\n";
                }else{
                    s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+IOManager::regexChar+lines.at(i)[4]+"\n";
                }
            }
            if(oldList == membershipList)
                editTaskNoListChange(sdatepicker,stitle,spercent,sdescr,membershipList);
            else
                editTaskWithListChange(sdatepicker,stitle,spercent,sdescr,membershipList);
        }
        IOManager::writeFile(path, s);
        lines.clear();
        //origin->filter();
        this->close();
    }
}

void TaskManager::editTaskNoListChange(const string &duedate, const string &title, const string &percent, const string &description, const string &list) const{
    if(origin->l1->getName() == list){
        origin->l1->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
    else if(origin->l2->getName() == list){
        origin->l2->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
    else if(origin->l3->getName() == list){
        origin->l3->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
    else if(origin->l4->getName() == list){
        origin->l4->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
    else if(origin->l5->getName() == list){
        origin->l5->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
}

void TaskManager::editTaskWithListChange(const string &duedate, const string &title, const string &percent, const string &description, const string &list) const{
    Task* temp;

    //remove task from old list
    if(origin->l1->getName() == oldList){
        temp = origin->l1->removeTask(oldDuedate,oldTitle,oldPercent,oldDescription);
    }
    else if(origin->l2->getName() == oldList){
        temp = origin->l2->removeTask(oldDuedate,oldTitle,oldPercent,oldDescription);
    }
    else if(origin->l3->getName() == oldList){
        temp = origin->l3->removeTask(oldDuedate,oldTitle,oldPercent,oldDescription);
    }
    else if(origin->l4->getName() == oldList){
        temp = origin->l4->removeTask(oldDuedate,oldTitle,oldPercent,oldDescription);
    }
    else if(origin->l5->getName() == oldList){
        temp = origin->l5->removeTask(oldDuedate,oldTitle,oldPercent,oldDescription);
    }

    //add task in destination list
    if(origin->l1->getName() == list){
        origin->l1->insertTask(temp);
    }
    else if(origin->l2->getName() == list){
        origin->l2->insertTask(temp);
    }
    else if(origin->l3->getName() == list){
        origin->l3->insertTask(temp);
    }
    else if(origin->l4->getName() == list){
        origin->l4->insertTask(temp);
    }
    else if(origin->l5->getName() == list){
        origin->l5->insertTask(temp);
    }

    //updates task
    if(origin->l1->getName() == list){
        origin->l1->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
    else if(origin->l2->getName() == list){
        origin->l2->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
    else if(origin->l3->getName() == list){
        origin->l3->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
    else if(origin->l4->getName() == list){
        origin->l4->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
    }
    else if(origin->l5->getName() == list){
        origin->l5->modifyTask(oldDuedate,oldTitle,oldPercent,oldDescription,duedate,title,percent,description);
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
        if(!(lines.at(i)[0].compare(oldDuedate) == 0 && lines.at(i)[1].compare(oldTitle) == 0 &&
                lines.at(i)[2].compare(oldPercent) == 0 && lines.at(i)[3].compare(oldDescription) == 0 && lines.at(i)[4].compare(oldList) == 0)){
            s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+IOManager::regexChar+lines.at(i)[4]+"\n";
        }
    }

    IOManager::writeFile(path, s);
    lines.clear();
    origin->deleteSingleTask(oldList,oldDuedate,oldTitle,oldPercent,oldDescription);
    origin->updateOutput();
    this->close();
}
