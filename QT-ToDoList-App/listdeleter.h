#ifndef LISTDELETER_H
#define LISTDELETER_H

#include <QDialog>
#include <iostream>
#include <string>
#include "mainwindow.h"

using namespace std;

namespace Ui {
class ListDeleterDialog;
}

class ListDeleter : public QDialog
{
    Q_OBJECT

public:
    explicit ListDeleter(QWidget *parent = 0);
    inline void setOrigin(MainWindow *origin){ this->origin = origin; }
    void loadLists();
    ~ListDeleter();

private slots:
    void on_cancelbtn_clicked();
    void on_savebtn_clicked();

private:
    Ui::ListDeleterDialog *ui;
    MainWindow *origin;
};

#endif // LISTDELETER_H
