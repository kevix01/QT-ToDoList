#ifndef CREATEDBFILE_H
#define CREATEDBFILE_H

#include <QDialog>
#include <string>
#include <iostream>
#include "iomanager.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class CreateDatafileDialog;
}

class CreateDbFile : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDbFile(QWidget *parent = 0);
    inline void setOrigin(MainWindow *origin){ this->origin = origin; }
    ~CreateDbFile();

private slots:
    void on_name_ed_textChanged();
    void on_cancelbtn_clicked();
    void on_folder_ed_textChanged();
    void on_createbtn_clicked();
    void on_browsebtn_clicked();

private:
    Ui::CreateDatafileDialog *ui;
    void checkFields();
    void updateOutputLabel();
    MainWindow *origin;
};

#endif // CREATEDBFILE_H
