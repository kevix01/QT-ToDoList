#include "mainwindowtest.h"

MainWindowTest::MainWindowTest()
{
    int argc = 0;
    char *argv[100];
    a = new QApplication(argc, argv);
    m = new MainWindow;
}

MainWindowTest::~MainWindowTest()
{
    if(a)
        delete a;
}

void MainWindowTest::UiComponentsConstructionTest(){
    /*qDebug() << "State before click: " << m->cb_completed->checkState();
    QTest::mouseClick(m->cb_completed, Qt::LeftButton);
    qDebug() << "State after click: " << m->cb_completed->checkState();*/
    QVERIFY2(m->OptionsMenu, "Menu 'Options' not created");
    QVERIFY2(m->EditMenu, "Menu 'Edit' not created");

    QVERIFY2(m->rb_all, "Radio button 'All' not created (filters' component)");
    QVERIFY2(m->rb_thisweek, "Radio button 'This week' not created (filters' component)");
    QVERIFY2(m->rb_overdue, "Radio button 'Overdue' not created (filters' component)");
    QVERIFY2(m->rb_today, "Radio button 'Today' not created (filters' component)");
    QVERIFY2(m->cb_completed, "Checkbox 'Not completed' not created (filters' component)");

    QVERIFY2(m->table, "TableWidget 'Table' not created");

    QVERIFY2(m->addTaskAct, "QAction 'Add task' not created ('Edit' menu action");
    QVERIFY2(m->newDatafileAct, "QAction 'Create new data file' not created ('Options' menu action");
    QVERIFY2(m->changeDBAct, "QAction 'Change data file' not created ('Options' menu action");
    QVERIFY2(m->exitProgramAct, "QAction 'Exit' not created ('Options' menu action");


}
