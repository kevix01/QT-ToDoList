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
    void enableAddTask();
    inline void setPath(string path)
    {
        ofstream configFile;
        configFile.open(configDBFile);
        if (!configFile.is_open()) {
               cerr << "Could not open the configuration file! (" << configDBFile <<")" << endl;
               exit(EXIT_FAILURE);
        }
        configFile << path;
        configFile.close();
        this->path = path;
    }
    string path;
    static constexpr const char* configDBFile = "DefaultDBPath.cfg";
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
    Ui::MainWindow *ui;
    void createMenus();
    void createFilters(QHBoxLayout *layout);
    void initializeTable();
    static string readDefaultPath();
    static bool defaultPathFileExists(const char* name);
    int getWeekNumber(tm t);
    void updateTable(vector<string*> data);
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
