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
#include <QFileDialog>
#include "iomanager.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    void enableAddTask(); //method to enable Add Task button on new database file creation (from createdatafiledialog class)
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

    static constexpr const char* configDBFile = "DefaultDBPath.cfg"; //global static variable for the configDBFile default database path
    ~MainWindow();

private slots:
    void addTask();
    void changeDB();
    void exitProgram();
    void newDatafile();
    void cellSelected(int nRow);

public slots:
    void filter();

private:
    friend class MainWindowTest;
    friend class TaskManagerTest;
    Ui::MainWindow *ui;
    string path; //string were is saved the default database file path (read by the configDBFile) on program startup, new db file creation or db change
    void createMenus(); //method for menus creation
    void createFilters(QHBoxLayout *layout); //method for filters creation
    void initializeTable(); //method to initialize the output table on the program startup
    static string readDefaultPath(); //static method to read the default path of database file from the configDBFile configuration file.
                                    //return the path as a string
    static bool defaultPathConfigFileExists(const char* name); //static method that checks if the configDBFile exists
    int getWeekNumber(tm t);
    void updateTable(vector<string*> data); //method that is in charge of updating the output table when something changes (es. task add, db file change)
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
    QAction *newDatafileAct;
    QAction *addTaskAct;
    QAction *changeDBAct;
    QAction *exitProgramAct;
};

#endif // MAINWINDOW_H
