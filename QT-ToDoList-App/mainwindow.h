#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QTableWidget>
#include <QMessageBox>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <QFileDialog>
#include "iomanager.h"
#include "tasklist.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public Observer
{
    Q_OBJECT

public:
    explicit MainWindow();
    void enableAddList(); //method to enable Add List button in certain conditions (calls enableAddTask too)
    void enableAddTask();//method to enable Add Task button in certain conditions
    inline void setPath(string path) //method to change the default path, updating the configDBFile too
    {
        ofstream configFile;
        configFile.open(configDBFile);
        if (!configFile.is_open()) {
               //cerr << "Could not open the configuration file! (" << configDBFile <<")" << endl;
               QMessageBox msgBox;
               msgBox.setWindowTitle("Error");
               msgBox.setText(QString("Cannot update config file (%1) \nThere may be a permissions problem.\nSolutions:\n"
                                      "1.Try again or restart the app;"
                                      "\n2.Close any other app related to the file mentioned above;"
                                      "\n3.Delete the config file to reset program settings \n(NOTE: this will not delete your database files!)").arg(configDBFile));
               msgBox.setStandardButtons(QMessageBox::Ok);
               if(msgBox.exec() == QMessageBox::Ok)
                 return;
        }
        configFile << path;
        configFile.close();
        this->path = path;
    }
    //lists of tasks declaration
    TaskList* l1 {new TaskList()};
    TaskList* l2 {new TaskList()};
    TaskList* l3 {new TaskList()};
    TaskList* l4 {new TaskList()};
    TaskList* l5 {new TaskList()};

    static constexpr const char* configDBFile = "DefaultDBPath.cfg"; //global static variable for the configDBFile default database path
    void updateOutput() override; //virtual method called when applied changes in lists (and tasks)
    void decreaseActiveSubject() override; //decrease the number of actives lists
    void decreaseActiveSubject(const string& listName) override; //decrease the number of actives lists given a list name in order to disable it in output
    void createNewList(const string& name); //create a new list incrementing activeLists counter and updating the output
    void resetLists(); //reset all lists freeing memory of all the related tasks (used when changed data file)
    void resetSingleList(const string& name); //reset a single list and all related tasks, decrementing activeLists counter (update the data file too)
    void deleteSingleTask(const string& list, const string& dueDate, const string& title, const string& percent, const string& description);
                        //calls respective list method in order to delete the related task with same data as given in input (freeing memory)
    ~MainWindow();

private slots:
    void addTask();
    void addList();
    void deleteList();
    void changeDB();
    void exitProgram();
    void newDatafile();
    void cellSelected(int nRow);
    void setViewList(const string& selection);

public slots:
    void filter();

private:
    friend class MainWindowTest;
    friend class TaskManagerTest;
    Ui::MainWindow *ui;
    string path; //string were is saved the default database file path (read by the configDBFile) on program startup, new db file creation or db change
    string selectionList {"&All"};
    int activeLists {0}; //counter that represent the number of active lists (max 5)
    void createMenus(); //method for menus creation
    void createFilters(QHBoxLayout *layout); //method for filters creation
    void initializeTable(); //method to initialize the output table on the program startup
    const vector<string*> getOutput(IOManager& mng); //load tasks from each list and return it as a vector of strings' array
    void updateTable(const vector<string*>& data); //method that is in charge of updating the output table when something changes (es. task add, db file change)
    static string readDefaultPath(); //static method to read the default path of database file from the configDBFile configuration file.
                                    //return the path as a string
    static bool defaultPathConfigFileExists(const char* name); //static method that checks if the configDBFile exists
    int getWeekNumber(tm t);
    void initializeLists(const string& path, IOManager& mng); //initialize lists with data given by data file
    void initializeTasks(const vector<string*>& data); //initialize tasks with data took from data file (given in input as a vector of strings' array)
    void updateUnDoneTasksCounters(); //update output of undone tasks counters for each list
    //Ui components
    QRadioButton *rb_all;
    QRadioButton *rb_overdue;
    QRadioButton *rb_today;
    QRadioButton *rb_thisweek;
    QCheckBox *cb_completed;
    QTableWidget *table;
    QStringList tableHeader;
    QMenu *EditMenu;
    QMenu *OptionsMenu;
    QMenu *ListMenu;
    QAction *newDatafileAct;
    QAction *addTaskAct;
    QAction *addListAct;
    QAction *deleteListAct;
    QAction *changeDBAct;
    QAction *exitProgramAct;
    QAction *allLists;
    QAction *list1;
    QAction *list2;
    QAction *list3;
    QAction *list4;
    QAction *list5;
};

#endif // MAINWINDOW_H
