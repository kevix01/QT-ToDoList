#include "listadder.h"
#include "ui_listadder.h"

ListAdder::ListAdder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListAdderDialog)
{
    ui->setupUi(this);
    ui->savebtn->setEnabled(true);
    this->setWindowTitle("New List");
}

ListAdder::~ListAdder()
{
    delete ui;
}


void ListAdder::on_cancelbtn_clicked()
{
    this->close();
}


void ListAdder::on_name_ed_textChanged()
{
    checkField();
}

void ListAdder::checkField(){
    string sname(ui->name_ed->text().toUtf8().constData());

    if(sname.length()>0){
        ui->savebtn->setEnabled(true);
    }else{
        ui->savebtn->setEnabled(false);
    }
}

void ListAdder::on_savebtn_clicked()
{
    string sname(ui->name_ed->text().toUtf8().constData());
    if(sname.find(IOManager::regexChar) != std::string::npos){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Saving failed");
        QString errorText = QString("'%1' character not allowed! \nPlease remove it and retry.").arg(IOManager::regexChar);
        msgBox.setText(errorText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        if(msgBox.exec() == QMessageBox::Ok)
          return;
    } else if(sname == origin->l1->getName() || sname == origin->l2->getName() || sname == origin->l3->getName()
              || sname == origin->l4->getName() || sname == origin->l5->getName()){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Saving failed");
        QString errorText = QString("A list with this name already exists! \nTry again using a different name.");
        msgBox.setText(errorText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        if(msgBox.exec() == QMessageBox::Ok)
          return;
    } else {
        origin->createNewList(sname);
        origin->updateOutput();
        this->close();
    }
}
