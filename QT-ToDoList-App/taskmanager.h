#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QDialog>
#include <string>
#include <vector>
#include <QMessageBox>
#include "iomanager.h"
#include "mainwindow.h"

using namespace std;

namespace Ui { //semplified namespace
class NewTaskDialog;
}

class TaskManager : public QDialog  //class used for task adding or managing (change/delete)
{
    Q_OBJECT

public:
    explicit TaskManager(QString dialogTitle, QWidget *parent = 0); //class constructor that takes as parameter the Dialog title to set
    void setData(string duedate, string title, string percent, string description);  //method used to update data on a certain table's cell
    void setPath(string path); //take the path from MainWindow
    inline void setOrigin(MainWindow *origin){ this->origin = origin; } //method to set the origin dialog (MainWindow)
    //string values to compare fields changes with the old ones
    string oldDuedate;
    string oldTitle;
    string oldPercent;
    string oldDescription;
    ~TaskManager();

private slots: //methods for dialog's components actions (input by user)
    void on_slider_valueChanged(int value);
    void on_description_ed_textChanged();
    void on_title_et_textChanged();
    void on_datepicker_userDateChanged();
    void on_savebtn_clicked();
    void on_cancelbtn_clicked();
    void on_deletebtn_clicked();

private:
    friend class TaskManagerTest;
    void checkFields(); //method to check field of the "task-add" dialog to check validity
    Ui::NewTaskDialog *ui;
    bool newTask;
    string path; //path of database file where to apply modifications (took from MainWindow class)
    MainWindow *origin; //MainWindow object (main program dialog)
};

#endif // TASKMANAGER_H
