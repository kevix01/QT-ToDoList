#include "listdeleter.h"
#include "ui_listdeleter.h"

ListDeleter::ListDeleter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListDeleterDialog)
{
    ui->setupUi(this);
    ui->savebtn->setEnabled(true);
    this->setWindowTitle("Delete List");
}

ListDeleter::~ListDeleter()
{
    delete ui;
}

void ListDeleter::loadLists(){
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

void ListDeleter::on_cancelbtn_clicked()
{
    this->close();
}

void ListDeleter::on_savebtn_clicked()
{
    string slist(ui->comboBox->currentText().toUtf8().constData());
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Delete list", "Are you sure you want to permanently delete this list with all related tasks?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::No) {
          return;
      }
    origin->resetSingleList(slist);
    origin->updateOutput();
    this->close();
}
