#include <QMainWindow>
#include <sstream>
#include <fstream>
#include <string>
#include <QHeaderView>
#include <sys/stat.h>
#include <unistd.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iomanager.h"
#include "taskmanager.h"
#include "createdbfile.h"
#include "listadder.h"
#include "listdeleter.h"

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

    // Models
    IOManager mng;
    l1->registerObserver(this);
    l2->registerObserver(this);
    l3->registerObserver(this);
    l4->registerObserver(this);
    l5->registerObserver(this);
    initializeLists(path,mng);

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

    addListAct = new QAction(tr("&Add List"), this);

    deleteListAct = new QAction(tr("&Delete List"), this);

    allLists = new QAction(tr("&All"), this);
    connect(allLists, &QAction::triggered, this, [this]{MainWindow::setViewList(allLists->text().toUtf8().constData());});

    list1 = new QAction(tr("&Empty"), this);
    connect(list1, &QAction::triggered, this, [this]{MainWindow::setViewList(list1->text().toUtf8().constData());});

    list2 = new QAction(tr("&Empty"), this);
    connect(list2, &QAction::triggered, this, [this]{MainWindow::setViewList(list2->text().toUtf8().constData());});

    list3 = new QAction(tr("&Empty"), this);
    connect(list3, &QAction::triggered, this, [this]{MainWindow::setViewList(list3->text().toUtf8().constData());});

    list4 = new QAction(tr("&Empty"), this);
    connect(list4, &QAction::triggered, this, [this]{MainWindow::setViewList(list4->text().toUtf8().constData());});

    list5 = new QAction(tr("&Empty"), this);
    connect(list5, &QAction::triggered, this, [this]{MainWindow::setViewList(list5->text().toUtf8().constData());});

    if(IOManager::exists(path) == false){
        addTaskAct->setEnabled(false);
        addListAct->setEnabled(false);
        deleteListAct->setEnabled(false);
        allLists->setEnabled(false);
        list1->setEnabled(false);
        list2->setEnabled(false);
        list3->setEnabled(false);
        list4->setEnabled(false);
        list5->setEnabled(false);
    }
    addTaskAct -> setShortcuts(QKeySequence::New);
    connect(addTaskAct, &QAction::triggered, this, &MainWindow::addTask);

    addListAct -> setShortcuts(QKeySequence::New);
    connect(addListAct, &QAction::triggered, this, &MainWindow::addList);

    deleteListAct -> setShortcuts(QKeySequence::Delete);
    connect(deleteListAct, &QAction::triggered, this, &MainWindow::deleteList);

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
    EditMenu->addAction(addListAct);
    EditMenu->addAction(deleteListAct);
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

    //IOManager mng;
    //inserting data
   // updateTable(IOManager::readFile(path,mng));

    connect( table, SIGNAL( cellDoubleClicked (int, int) ),
     this, SLOT( cellSelected( int ) ) );
}

void MainWindow::addTask()
{
    TaskManager dlg(QString("Task Adding"));
    dlg.setModal(true);
    dlg.setPath(path);
    dlg.setOrigin(this);
    dlg.loadLists();
    dlg.exec();
}

void MainWindow::addList()
{
    ListAdder dlg;
    dlg.setModal(true);
    dlg.setOrigin(this);
    dlg.exec();
}

void MainWindow::createNewList(const string& name){
    if(l1->getName() == ""){
        l1->setName(name);
        list1->setText(QString::fromStdString(string("&") + name));
        list1->setEnabled(true);
        activeLists++;
    } else if(l2->getName() == ""){
        l2->setName(name);
        list2->setText(QString::fromStdString(string("&") + name));
        list2->setEnabled(true);
        activeLists++;
    } else if(l3->getName() == ""){
        l3->setName(name);
        list3->setText(QString::fromStdString(string("&") + name));
        list3->setEnabled(true);
        activeLists++;
    } else if(l4->getName() == ""){
        l4->setName(name);
        list4->setText(QString::fromStdString(string("&") + name));
        list4->setEnabled(true);
        activeLists++;
    } else if(l5->getName() == ""){
        l5->setName(name);
        list5->setText(QString::fromStdString(string("&") + name));
        list5->setEnabled(true);
        activeLists++;
    }
}
void MainWindow::deleteList()
{
    ListDeleter dlg;
    dlg.setModal(true);
    dlg.setOrigin(this);
    dlg.loadLists();
    dlg.exec();
}

void MainWindow::resetSingleList(const string &name){
    if(l1->getName() == name){
        l1->reset();
        list1->setText(QString::fromStdString("&Empty"));
        list1->setEnabled(false);
        activeLists--;
    } else if(l2->getName() == name){
        l2->reset();
        list2->setText(QString::fromStdString("&Empty"));
        list2->setEnabled(false);
        activeLists--;
    } else if(l3->getName() == name){
        l3->reset();
        list3->setText(QString::fromStdString("&Empty"));
        list3->setEnabled(false);
        activeLists--;
    } else if(l4->getName() == name){
        l4->reset();
        list4->setText(QString::fromStdString("&Empty"));
        list4->setEnabled(false);
        activeLists--;
    } else if(l5->getName() == name){
        l5->reset();
        list5->setText(QString::fromStdString("&Empty"));
        list5->setEnabled(false);
        activeLists--;
    }

    IOManager mng;
    vector<string*> lines = IOManager::readFile(path,mng);
    unsigned int i;

    string s = "";

    for(i=0; i<lines.size(); i++){
        if(!(lines.at(i)[4].compare(name) == 0)){
            s+=lines.at(i)[0]+IOManager::regexChar+lines.at(i)[1]+IOManager::regexChar+lines.at(i)[2]+IOManager::regexChar+lines.at(i)[3]+IOManager::regexChar+lines.at(i)[4]+"\n";
        }
    }

    IOManager::writeFile(path, s);
    lines.clear();
}

void MainWindow::deleteSingleTask(const string &list, const string &dueDate, const string &title, const string &percent, const string &description){
    if(l1->getName() == list){
        l1->deleteTask(dueDate,title,percent,description);
    }
    else if(l2->getName() == list){
        l2->deleteTask(dueDate,title,percent,description);
    }
    else if(l3->getName() == list){
        l3->deleteTask(dueDate,title,percent,description);
    }
    else if(l4->getName() == list){
        l4->deleteTask(dueDate,title,percent,description);
    }
    else if(l5->getName() == list){
        l5->deleteTask(dueDate,title,percent,description);
    }
}

void MainWindow::setViewList(const string& selection){
    selectionList = selection;
    filter();
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
        resetLists();
        enableAddList();
    }
}

void MainWindow::enableAddList(){
    if(activeLists < 5)
        addListAct->setEnabled(true);
    else
        addListAct->setEnabled(false);
    enableAddTask();
}

void MainWindow::enableAddTask(){
    if(activeLists > 0)
        addTaskAct->setEnabled(true);
    else
        addTaskAct->setEnabled(false);
}

void MainWindow::exitProgram()
{
    exit(0);
}

void MainWindow::cellSelected(int nRow)
{
    TaskManager dlg(QString("Task Managing"));
    dlg.setModal(true);
    dlg.setPath(path);
    dlg.setOrigin(this);
    dlg.loadLists();
    dlg.loadData(table->item(nRow, 2)->text().toUtf8().constData(), table->item(nRow, 3)->text().toUtf8().constData(),
                 table->item(nRow, 4)->text().toUtf8().constData(), table->item(nRow, 5)->text().toUtf8().constData(), table->item(nRow, 0)->text().toUtf8().constData());
    dlg.exec();
}

void MainWindow::initializeLists(const string& path, IOManager& mng){
    vector<string*> data = IOManager::readFile(path,mng);
    string temp;
    for(unsigned int i = 0; i < data.size(); i++){
        temp = data.at(i)[4];
        if(temp != l1->getName() && l1->getName() == ""){
            l1->setName(temp);
            list1->setText(QString::fromStdString(string("&") + temp));
            list1->setEnabled(true);
            activeLists++;
        } else if(temp != l2->getName() && l2->getName() == "" && temp != l1->getName()){
            l2->setName(temp);
            list2->setText(QString::fromStdString(string("&") + temp));
            list2->setEnabled(true);
            activeLists++;
        } else if(temp != l3->getName() && l3->getName() == "" && temp != l1->getName()
                  && temp != l2->getName()){
            l3->setName(temp);
            list3->setText(QString::fromStdString(string("&") + temp));
            list3->setEnabled(true);
            activeLists++;
        } else if(temp != l4->getName() && l4->getName() == "" && temp != l1->getName()
                  && temp != l2->getName() && temp != l3->getName()){
            l4->setName(temp);
            list4->setText(QString::fromStdString(string("&") + temp));
            list4->setEnabled(true);
            activeLists++;
        } else if(temp != l5->getName() && l5->getName() == "" && temp != l1->getName()
                  && temp != l2->getName() && temp != l3->getName() && temp != l4->getName()){
            l5->setName(temp);
            list5->setText(QString::fromStdString(string("&") + temp));
            list5->setEnabled(true);
            activeLists++;
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

    if(activeLists < 5 && IOManager::exists(path))
        addListAct->setEnabled(true);
    else
        addListAct->setEnabled(false);

    if(activeLists == 0)
        deleteListAct->setEnabled(false);
    else
        deleteListAct->setEnabled(true);

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
    activeLists = 0;
    initializeLists(path, mng);
}

const vector<string*> MainWindow::getOutput(IOManager& mng){
    l1->loadTasksData(mng);
    l2->loadTasksData(mng);
    l3->loadTasksData(mng);
    l4->loadTasksData(mng);
    l5->loadTasksData(mng);
    return mng.data;
}

void MainWindow::decreaseActiveSubject(){
    activeLists--;
}

void MainWindow::decreaseActiveSubject(const string& listName){
    if(l1->getName() == listName){
        l1->setName("");
        list1->setText(QString::fromStdString(string("&Empty")));
        list1->setEnabled(false);
    }
    else if(l2->getName() == listName){
        l2->setName("");
        list2->setText(QString::fromStdString(string("&Empty")));
        list2->setEnabled(false);
    }
    else if(l3->getName() == listName){
        l3->setName("");
        list3->setText(QString::fromStdString(string("&Empty")));
        list3->setEnabled(false);
    }
    else if(l4->getName() == listName){
        l4->setName("");
        list4->setText(QString::fromStdString(string("&Empty")));
        list4->setEnabled(false);
    }
    else if(l5->getName() == listName){
        l5->setName("");
        list5->setText(QString::fromStdString(string("&Empty")));
        list5->setEnabled(false);
    }
    activeLists--;
}

void MainWindow::updateOutput(){
    if(activeLists < 5)
        addListAct->setEnabled(true);
    else
        addListAct->setEnabled(false);

    if(activeLists == 0)
        deleteListAct->setEnabled(false);
    else {
        deleteListAct->setEnabled(true);
        allLists->setEnabled(true);
    }

    enableAddTask();

    filter();
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
    vector<string*> data = getOutput(mng);
    bool completed = cb_completed->isChecked();

    time_t now = time(0);
    tm cur = *gmtime(&now);

    unsigned int i;
    for(i=0; i<data.size(); i++){
        int year = stoi((mng.dates = IOManager::split(data.at(i)[0], '/'))[0]);
        int month = stoi((mng.dates = IOManager::split(data.at(i)[0], '/'))[1]);
        int day = stoi((mng.dates = IOManager::split(data.at(i)[0], '/'))[2]);

        if(selectionList != "&All"){
            if((selectionList.find(data.at(i)[4])) == std::string::npos){
                data.erase(data.begin()+i);
                i--;
                continue;
            }
        }

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
    delete addListAct;
    delete changeDBAct;
    delete exitProgramAct;
    delete list1;
    delete list2;
    delete list3;
    delete list4;
    delete list5;
    delete allLists;
    delete EditMenu;
    delete OptionsMenu;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    delete l5;
}
