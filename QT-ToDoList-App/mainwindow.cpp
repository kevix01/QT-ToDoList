#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iomanager.h"
#include <QMainWindow>
#include "taskmanager.h"
#include "createdbfile.h"
#include <sstream>
#include <fstream>
#include <QHeaderView>
#include <sys/stat.h>
#include <unistd.h>

MainWindow::MainWindow()
{
    // read the default path from the <configDBFile> file
    path = readDefaultPath();

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    // MenuBar
    createMenus();

    // Filters
    QHBoxLayout *filtersLayout = new QHBoxLayout;
    createFilters(filtersLayout);

    //Table
    table = new QTableWidget(this);
    initializeTable();

    QVBoxLayout *layout = new QVBoxLayout;
    layout -> setMargin(8);
    layout -> addLayout(filtersLayout);
    layout -> addWidget(table);
    layout -> setStretch(0, 0);
    layout -> setStretch(1, 0);
    layout -> setStretch(2, 1);

    widget -> setLayout(layout);

    setWindowTitle("ToDo List App - powered by QT");
    setMinimumSize(620, 320);
    resize(680, 480);
}

void MainWindow::createMenus()
{
    addTaskAct = new QAction(tr("&Add Task"), this);

    allLists = new QAction(tr("&All"), this);
    connect(allLists, &QAction::triggered, this, &MainWindow::setViewList);

    list1 = new QAction(tr("&Empty"), this);
    connect(list1, &QAction::triggered, this, &MainWindow::setViewList);

    list2 = new QAction(tr("&Empty"), this);
    connect(list2, &QAction::triggered, this, &MainWindow::setViewList);

    list3 = new QAction(tr("&Empty"), this);
    connect(list3, &QAction::triggered, this, &MainWindow::setViewList);

    list4 = new QAction(tr("&Empty"), this);
    connect(list4, &QAction::triggered, this, &MainWindow::setViewList);

    list5 = new QAction(tr("&Empty"), this);
    connect(list5, &QAction::triggered, this, &MainWindow::setViewList);

    if(IOManager::exists(path) == false){
        addTaskAct->setEnabled(false);
        allLists->setEnabled(false);
        list1->setEnabled(false);
        list2->setEnabled(false);
        list3->setEnabled(false);
        list4->setEnabled(false);
        list5->setEnabled(false);
    }
    addTaskAct -> setShortcuts(QKeySequence::New);
    connect(addTaskAct, &QAction::triggered, this, &MainWindow::addTask);

    newDatafileAct = new QAction(tr("&Create New Data File"), this);
    connect(newDatafileAct, &QAction::triggered, this, &MainWindow::newDatafile);

    changeDBAct = new QAction(tr("&Change Data File"), this);
    connect(changeDBAct, &QAction::triggered, this, &MainWindow::changeDB);

    exitProgramAct = new QAction(tr("&Exit"), this);
    connect(exitProgramAct, &QAction::triggered, this, &MainWindow::exitProgram);

    EditMenu = menuBar()->addMenu(tr("&Edit"));
    OptionsMenu = menuBar()->addMenu(tr("&Options"));
    ListMenu = menuBar()->addMenu(tr("&List Selection"));
    EditMenu->addAction(addTaskAct);
    OptionsMenu->addAction(newDatafileAct);
    OptionsMenu->addAction(changeDBAct);
    OptionsMenu->addAction(exitProgramAct);
    ListMenu->addAction(allLists);
    ListMenu->addAction(list1);
    ListMenu->addAction(list2);
    ListMenu->addAction(list3);
    ListMenu->addAction(list4);
    ListMenu->addAction(list5);
}


void MainWindow::createFilters(QHBoxLayout *layout)
{
    rb_all = new QRadioButton;
    rb_overdue = new QRadioButton;
    rb_today = new QRadioButton;
    rb_thisweek = new QRadioButton;
    cb_completed = new QCheckBox;

    rb_all->setText(tr("&All"));
    rb_all->setChecked(true);
    rb_overdue->setText(tr("&Overdue"));
    rb_today->setText(tr("&Today"));
    rb_thisweek->setText(tr("&This Week"));
    cb_completed->setText(tr("&Not Completed"));

    //connect actions of filters' components to filter() method
    //in order to update the output
    connect(rb_all,SIGNAL(clicked()),this,SLOT(filter()));
    connect(rb_overdue,SIGNAL(clicked()),this,SLOT(filter()));
    connect(rb_today,SIGNAL(clicked()),this,SLOT(filter()));
    connect(rb_thisweek,SIGNAL(clicked()),this,SLOT(filter()));
    connect(cb_completed,SIGNAL(clicked()),this,SLOT(filter()));

    QHBoxLayout *cbLayout = new QHBoxLayout;
    cbLayout -> setAlignment(Qt::AlignRight);
    cbLayout -> addWidget(cb_completed);

    layout->addWidget(rb_all);
    layout->addWidget(rb_overdue);
    layout->addWidget(rb_today);
    layout->addWidget(rb_thisweek);
    layout->addLayout(cbLayout);

    layout -> setStretch(0, 0);
    layout -> setStretch(1, 0);
    layout -> setStretch(2, 0);
    layout -> setStretch(3, 0);
    layout -> setStretch(4, 1);
}

void MainWindow::initializeTable()
{
    table -> setColumnCount(6);
    tableHeader << "List" << "Finished" << "DueDate" << "Title" << "% Complete" << "Description";
    table -> setHorizontalHeaderLabels(tableHeader);
    table -> horizontalHeader()->setStretchLastSection(true);
    table -> verticalHeader() -> setVisible(false);
    table -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    table -> setSelectionBehavior(QAbstractItemView::SelectRows);
    table -> setSelectionMode(QAbstractItemView::SingleSelection);
    table -> setStyleSheet("QTableView {selection-background-color: #E0F7FA; selection-color: #000000;}");

    IOManager mng;
    //inserting data
   // updateTable(IOManager::readFile(path,mng));
    initializeLists(path,mng);

    connect( table, SIGNAL( cellDoubleClicked (int, int) ),
     this, SLOT( cellSelected( int ) ) );
}

void MainWindow::addTask()
{
    TaskManager dlg(QString("Task Adding"));
    dlg.setModal(true);
    dlg.setPath(path);
    dlg.setOrigin(this);
    dlg.exec();
}

void MainWindow::setViewList(){

}

void MainWindow::newDatafile()
{
    CreateDbFile dlg;
    dlg.setModal(true);
    dlg.setOrigin(this);
    dlg.exec();
}

string MainWindow::readDefaultPath(){
    if(defaultPathConfigFileExists(configDBFile)){
        ifstream configFile(configDBFile);
        if (!configFile.is_open()) {
                //cerr << "Could not open the configuration file! (" << configDBFile <<")" << endl;
                QMessageBox msgBox;
                msgBox.setWindowTitle("Critical Error");
                msgBox.setText(QString("Cannot read config file (%1) \nThere may be a permissions problem.\nSolutions:\n"
                                       "1.Restart the app;"
                                       "\n2.Close any other app related to the file mentioned above;"
                                       "\n3.Delete the config file to reset program settings \n(NOTE: this will not delete your database files!)").arg(configDBFile));
                msgBox.setStandardButtons(QMessageBox::Ok);
                if(msgBox.exec() == QMessageBox::Ok)
                  exit(EXIT_FAILURE);
        }
        string temp;
        getline (configFile,temp);
        configFile.close();
        return temp;
    } else {
        ofstream configFile;
        configFile.open(configDBFile);
        if (!configFile.is_open()) {
                //cerr << "Could not open the configuration file! (" << configDBFile <<")" << endl;
                QMessageBox msgBox;
                msgBox.setWindowTitle("Critical Error");
                msgBox.setText(QString("Cannot read config file (%1) \nThere may be a permissions problem.\nSolutions:\n"
                                       "1.Restart the app;"
                                       "\n2.Close any other app related to the file mentioned above;"
                                       "\n3.Delete the config file to reset program settings \n(NOTE: this will not delete your database files!)").arg(configDBFile));
                msgBox.setStandardButtons(QMessageBox::Ok);
                if(msgBox.exec() == QMessageBox::Ok)
                  exit(EXIT_FAILURE);
        }
        configFile << "";
        return "";
    }
}

bool MainWindow::defaultPathConfigFileExists(const char* name){
    struct stat fileInfo;
    return stat(name, &fileInfo) == 0;
}

void MainWindow::changeDB()
{
    bool valid = false;
    QFileDialog dlg(this);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setNameFilter(tr("Database - .cfg (*.cfg)"));
    dlg.setViewMode(QFileDialog::List);

    QStringList fileNames;
    if (dlg.exec()){
        fileNames = dlg.selectedFiles();
        string path = ((QString)fileNames.at(0)).toUtf8().constData();
        if (path.find(configDBFile) != std::string::npos) {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Error");
            msgBox.setText(QString("Wrong database file (%1) \nSelect a valid one!").arg(configDBFile));
            msgBox.setStandardButtons(QMessageBox::Ok);
            if(msgBox.exec() == QMessageBox::Ok)
              return;
        } else {
            this->setPath(path);
            this->resetLists();
            this->filter();
            valid = true;

        }
   }
    if(valid){
        addTaskAct->setEnabled(true);
        allLists->setEnabled(true);
    }
}

void MainWindow::enableAddList(){
    allLists->setEnabled(true);
    enableAddTask();
}

void MainWindow::enableAddTask(){
    addTaskAct->setEnabled(true);
}

void MainWindow::exitProgram()
{
    //cout << addTaskAct->text().toUtf8().constData();
    exit(0);
}

void MainWindow::cellSelected(int nRow)
{
    TaskManager dlg(QString("Task Managing"));
    dlg.setModal(true);
    dlg.loadData(table->item(nRow, 1)->text().toUtf8().constData(), table->item(nRow, 2)->text().toUtf8().constData(), table->item(nRow, 3)->text().toUtf8().constData(), table->item(nRow, 4)->text().toUtf8().constData());
    dlg.setPath(path);
    dlg.setOrigin(this);
    dlg.exec();
}

void MainWindow::initializeLists(const string& path, IOManager& mng){
    vector<string*> data = IOManager::readFile(path,mng);
    string temp;
    /*l1->setName("");
    l2->setName("");
    l3->setName("");
    l4->setName("");
    l5->setName("");*/
    for(unsigned int i = 0; i < data.size(); i++){
        temp = data.at(i)[4];
        if(temp != l1->getName() && l1->getName() == ""){
            l1->setName(temp);
            list1->setText(QString::fromStdString(string("&") + temp));
            list1->setEnabled("true");
        } else if(temp != l2->getName() && l2->getName() == ""){
            l2->setName(temp);
            list2->setText(QString::fromStdString(string("&") + temp));
            list2->setEnabled("true");
        } else if(temp != l3->getName() && l3->getName() == ""){
            l3->setName(temp);
            list3->setText(QString::fromStdString(string("&") + temp));
            list3->setEnabled("true");
        } else if(temp != l4->getName() && l4->getName() == ""){
            l4->setName(temp);
            list4->setText(QString::fromStdString(string("&") + temp));
            list4->setEnabled("true");
        } else if(temp != l5->getName() && l5->getName() == ""){
            l5->setName(temp);
            list5->setText(QString::fromStdString(string("&") + temp));
            list5->setEnabled("true");
        }
    }

    if(l1->getName() == ""){
        list1->setText(QString::fromStdString(string("&Empty")));
        list1->setEnabled(false);
    }
    if(l2->getName() == ""){
        list2->setText(QString::fromStdString(string("&Empty")));
        list2->setEnabled(false);
    }
    if(l3->getName() == ""){
        list3->setText(QString::fromStdString(string("&Empty")));
        list3->setEnabled(false);
    }
    if(l4->getName() == ""){
        list4->setText(QString::fromStdString(string("&Empty")));
        list4->setEnabled(false);
    }
    if(l5->getName() == ""){
        list5->setText(QString::fromStdString(string("&Empty")));
        list5->setEnabled(false);
    }

    initializeTasks(data);
}

void MainWindow::initializeTasks(const vector<string*>& data){
    string temp;
    for(unsigned int i = 0; i < data.size(); i++){
        temp = data.at(i)[4];
        if(temp == l1->getName()){
            l1->addNewTask(data.at(i));
        } else if(temp == l2->getName()){
            l2->addNewTask(data.at(i));
        } else if(temp == l3->getName()){
            l3->addNewTask(data.at(i));
        } else if(temp == l4->getName()){
            l4->addNewTask(data.at(i));
        } else if(temp == l5->getName()){
            l5->addNewTask(data.at(i));
        }
    }
    updateTable(data);
}

void MainWindow::resetLists(){
    IOManager mng;
    l1->reset();
    l2->reset();
    l3->reset();
    l4->reset();
    l5->reset();
    initializeLists(path, mng);
}
void MainWindow::updateTable(const vector<string*>& data){

    table -> clearContents();
    table -> setRowCount(data.size());
    unsigned int i;
    for(i=0; i<data.size(); i++){
        QCheckBox *qcb = new QCheckBox;
        qcb->setStyleSheet(QStringLiteral("QCheckBox::indicator {subcontrol-position: center;}"));
        // Set the checkbox as not editable and check it if necessary
        qcb -> setAttribute(Qt::WA_TransparentForMouseEvents, true);
        qcb -> setFocusPolicy(Qt::NoFocus);
        qcb -> setChecked(stoi(data.at(i)[2]) == 100);

        table -> setAlternatingRowColors(true);
        table -> setItem(i, 0, new QTableWidgetItem(QString::fromStdString(data.at(i)[4])));
        table -> setCellWidget(i, 1, qcb);
        QTableWidgetItem *qdd = new QTableWidgetItem(QString::fromStdString(data.at(i)[0]));
        qdd -> setTextAlignment(Qt::AlignCenter);
        table -> setItem(i, 2, qdd);
        table -> setItem(i, 3, new QTableWidgetItem(QString::fromStdString(data.at(i)[1])));
        QTableWidgetItem *qtwi = new QTableWidgetItem(QString::fromStdString(data.at(i)[2]));
        qtwi -> setTextAlignment(Qt::AlignCenter);
        table -> setItem(i, 4, qtwi);
        table -> setItem(i, 5, new QTableWidgetItem(QString::fromStdString(data.at(i)[3])));
    }
}


void MainWindow::filter()
{
    IOManager mng;
    vector<string*> data = IOManager::readFile(path,mng);
    bool completed = cb_completed->isChecked();

    time_t now = time(0);
    tm cur = *gmtime(&now);

    unsigned int i;
    for(i=0; i<data.size(); i++){
        int year = stoi((mng.dates = IOManager::split(data.at(i)[0], '/'))[0]);
        int month = stoi((mng.dates = IOManager::split(data.at(i)[0], '/'))[1]);
        int day = stoi((mng.dates = IOManager::split(data.at(i)[0], '/'))[2]);

        if(completed){
            if(stoi(data.at(i)[2]) == 100){
                data.erase(data.begin()+i);
                i--;
                continue;
            }
        }

        if(rb_today->isChecked()){
            if(cur.tm_year+1900 != year || cur.tm_mon+1 != month || cur.tm_mday != day){
                data.erase(data.begin()+i);
                i--;
            }
        }else if(rb_thisweek->isChecked()){
            time_t auxd = time(0);
            tm *auxdate = gmtime(&auxd);
            auxdate->tm_mday = day;
            auxdate->tm_mon = month - 1;
            auxdate->tm_year = year - 1900;
            auxd = mktime(auxdate);
            auxdate =  gmtime(&auxd);
            tm cp = *auxdate;

            if(getWeekNumber(cur) != getWeekNumber(cp) || cur.tm_year != cp.tm_year){
                data.erase(data.begin()+i);
                i--;
            }
        }else if(rb_overdue->isChecked()){
            if(cur.tm_year+1900 < year || (cur.tm_year+1900 == year && cur.tm_mon+1 < month) || (cur.tm_year+1900 == year && cur.tm_mon+1 == month && cur.tm_mday <= day)){
                data.erase(data.begin()+i);
                i--;
            }
        }
    }
    updateTable(data);
}


int MainWindow::getWeekNumber(tm t)
{
    time_t nnow = time(0);
    tm *mauxdate = gmtime(&nnow);
    mauxdate->tm_mday = 0;
    mauxdate->tm_mon = 0;
    mauxdate->tm_year = mauxdate->tm_year;
    nnow = mktime(mauxdate);
    mauxdate =  gmtime(&nnow);

    int julian = t.tm_yday;  // Jan 1 = 1, Jan 2 = 2, etc...
    int dow = t.tm_wday;  // Sun = 0, Mon = 1, etc...
    int weekNum = ((julian + 6) / 7);
    if (dow == 0){  // adjust for being after Saturday of week #1
        --weekNum;
    }
    return weekNum;
}

MainWindow::~MainWindow()
{
    delete rb_all;
    delete rb_overdue;
    delete rb_today;
    delete rb_thisweek;
    delete cb_completed;
    delete table;
    delete addTaskAct;
    delete changeDBAct;
    delete exitProgramAct;
    delete EditMenu;
    delete OptionsMenu;
}
