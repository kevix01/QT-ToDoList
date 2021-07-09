#ifndef LISTADDER_H
#define LISTADDER_H

#include <QDialog>
#include <iostream>
#include <string>
#include "mainwindow.h"

using namespace std;

namespace Ui {
class ListAdderDialog;
}

class ListAdder : public QDialog
{
    Q_OBJECT

public:
    explicit ListAdder(QWidget *parent = 0);
    inline void setOrigin(MainWindow *origin){ this->origin = origin; }
    ~ListAdder();

private slots:
    void on_name_ed_textChanged();
    void on_cancelbtn_clicked();
    void on_savebtn_clicked();

private:
    Ui::ListAdderDialog *ui;
    void checkField();
    MainWindow *origin;
};

#endif // LISTADDER_H
