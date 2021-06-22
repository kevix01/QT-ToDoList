#include "newtaskdialog.h"
#include "ui_newtaskdialog.h"

NewTaskDialog::NewTaskDialog(QString dialogTitle, QWidget *parent) :
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

void NewTaskDialog::setPath(string path)
{
    this->path = path;
}

void NewTaskDialog::setData(string duedate, string title, string percent, string description)
{
    this->oldDuedate = duedate;
    this->oldTitle = title;
    this->oldPercent = percent;
    this->oldDescription = description;
    vector<string> v = IOManager::split(duedate, '/');
    unsigned int i=0;
    string *sdate = new string[v.size()];
    for(auto it : v){
        sdate[i] = v[i];
        i++;
    }
    QDate *sd = new QDate(stoi(sdate[0]), stoi(sdate[1]), stoi(sdate[2]));
    ui->datepicker->setDate(*sd);
    ui->title_et->setText(QString::fromStdString(title));
    ui->slider->setValue(stoi(percent));
    ui->description_ed->setPlainText(QString::fromStdString(description));
    this->newTask = false;
    ui->savebtn->setText("Modify");
    ui->deletebtn->setVisible(true);
}


NewTaskDialog::~NewTaskDialog()
{
    delete ui;
}


void NewTaskDialog::on_slider_valueChanged(int value)
{
    ui->percent_lbl->setText(QString::fromStdString(to_string(value)+"% Compl."));
    checkFields();
}


void NewTaskDialog::checkFields(){
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
void NewTaskDialog::on_description_ed_textChanged()
{
    checkFields();
}

void NewTaskDialog::on_title_et_textChanged()
{
    checkFields();
}

void NewTaskDialog::on_datepicker_userDateChanged()
{
    checkFields();
}

void NewTaskDialog::on_savebtn_clicked()
{
    IOManager mng;
    vector<string*> lines = IOManager::readFile(path,mng);
    unsigned int i;
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
                s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+"\n";
            }
            s += sdatepicker+IOManager::regexChar+stitle+IOManager::regexChar+spercent+IOManager::regexChar+sdescr;
        }else{
            for(i=0; i<lines.size(); i++){
                if(lines.at(i)[0].compare(oldDuedate) == 0 && lines.at(i)[1].compare(oldTitle) == 0 &&
                        lines.at(i)[2].compare(oldPercent) == 0 && lines.at(i)[3].compare(oldDescription) == 0){
                    s += sdatepicker+IOManager::regexChar+stitle+IOManager::regexChar+spercent+IOManager::regexChar+sdescr+"\n";
                }else{
                    s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+"\n";
                }
            }
        }
        IOManager::writeFile(path, s);
        lines.clear();
        origin->filter();
        this->close();
    }
}

void NewTaskDialog::on_cancelbtn_clicked()
{
    this->close();
}

void NewTaskDialog::on_deletebtn_clicked()
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
                lines.at(i)[2].compare(oldPercent) == 0 && lines.at(i)[3].compare(oldDescription) == 0)){
            s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+"\n";
        }
    }

    IOManager::writeFile(path, s);
    lines.clear();
    origin->filter();
    this->close();
}
