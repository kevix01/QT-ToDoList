#include "createdbfile.h"
#include "ui_createdbfile.h"

CreateDbFile::CreateDbFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDatafileDialog)
{
    ui->setupUi(this);
    ui->folder_ed->setReadOnly(true);
    ui->createbtn->setEnabled(false);
    this->setWindowTitle("New Database File");
}

CreateDbFile::~CreateDbFile()
{
    delete ui;
}


void CreateDbFile::on_cancelbtn_clicked()
{
    this->close();
}


void CreateDbFile::on_name_ed_textChanged()
{
    updateOutputLabel();
    checkFields();
}

void CreateDbFile::on_folder_ed_textChanged()
{
    updateOutputLabel();
    checkFields();
}

void CreateDbFile::updateOutputLabel(){
    string sfolder(ui->folder_ed->text().toUtf8().constData());
    string stitle(ui->name_ed->text().toUtf8().constData());
    QString outputstringlbl = QString::fromStdString(sfolder + QString(QDir::separator()).toUtf8().constData() + stitle + ".cfg");
    ui->outputlbl->setText(outputstringlbl);
}


void CreateDbFile::checkFields(){
    string sfolder(ui->folder_ed->text().toUtf8().constData());
    string stitle(ui->name_ed->text().toUtf8().constData());

    if(sfolder.length()>0 && stitle.length()>0){
        ui->createbtn->setEnabled(true);
    }else{
        ui->createbtn->setEnabled(false);
    }
}

void CreateDbFile::on_createbtn_clicked()
{
    string sfolder(ui->folder_ed->text().toUtf8().constData());
    string stitle(ui->name_ed->text().toUtf8().constData());
    origin->setPath(sfolder + QString(QDir::separator()).toUtf8().constData() + stitle + ".cfg");
    origin->resetLists();
    origin->enableAddList();
    this->close();
}

void CreateDbFile::on_browsebtn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"), NULL, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->folder_ed->setText(dir);
}
