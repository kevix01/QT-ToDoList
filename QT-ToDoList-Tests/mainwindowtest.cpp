#include "mainwindowtest.h"
#include <QMenu>
#include "iomanager.h"

void MainWindowTest::initTestCase()
{
    qDebug() << "Starting tests about main app's components...";
    int argc = 0;
    char *argv[100];
    a = new QApplication(argc, argv);
    m = new MainWindow;
    //m->show();
}

MainWindowTest::~MainWindowTest()
{
    if(a)
        delete a;
}

void MainWindowTest::UiComponentsConstructionTest(){
    //testing components construction integrity
    m->show();

    QVERIFY2(m->OptionsMenu, "Menu 'Options' not created");
    QVERIFY2(m->EditMenu, "Menu 'Edit' not created");

    //testing filters' components integrity
    QVERIFY2(m->rb_all, "Radio button 'All' not created (filters' component)");
    QVERIFY2(m->rb_thisweek, "Radio button 'This week' not created (filters' component)");
    QVERIFY2(m->rb_overdue, "Radio button 'Overdue' not created (filters' component)");
    QVERIFY2(m->rb_today, "Radio button 'Today' not created (filters' component)");
    QVERIFY2(m->cb_completed, "Checkbox 'Not completed' not created (filters' component)");

    //testing table integrity
    QVERIFY2(m->table, "TableWidget 'Table' not created");

    //testing menus' actions integrity
    QVERIFY2(m->addTaskAct, "QAction 'Add task' not created ('Edit' menu action");
    QVERIFY2(m->newDatafileAct, "QAction 'Create new data file' not created ('Options' menu action");
    QVERIFY2(m->changeDBAct, "QAction 'Change data file' not created ('Options' menu action");
    QVERIFY2(m->exitProgramAct, "QAction 'Exit' not created ('Options' menu action");
}

void MainWindowTest::TableComponentTest(){
    //testing with a non corrupted db file
    m->setPath("RegularDbTest.cfg");
    QVERIFY2(!m->cb_completed->isChecked(), "'Not completed' checkbox is wrongly checked.");
    m->filter();

    //tables rows number checking
    QCOMPARE(m->table->rowCount(), 3);
    //compairing task 1
    QCOMPARE(m->table->item(0,0)->text(), "Test List 1");
    QCOMPARE(m->table->item(0,2)->text(), "2023/06/19");
    QCOMPARE(m->table->item(0,3)->text(), "Testing title 1");
    QCOMPARE(m->table->item(0,4)->text(), "25");
    QCOMPARE(m->table->item(0,5)->text(), "Description test of task 1");
    //compairing task 2
    QCOMPARE(m->table->item(1,0)->text(), "Test List 2");
    QCOMPARE(m->table->item(1,2)->text(), "2022/12/28");
    QCOMPARE(m->table->item(1,3)->text(), "Testing title 2");
    QCOMPARE(m->table->item(1,4)->text(), "48");
    QCOMPARE(m->table->item(1,5)->text(), "Description test of task 2");
    //compairing task 3
    QCOMPARE(m->table->item(2,0)->text(), "Test List 3");
    QCOMPARE(m->table->item(2,2)->text(), "2021/05/31");
    QCOMPARE(m->table->item(2,3)->text(), "Testing title 3");
    QCOMPARE(m->table->item(2,4)->text(), "100");
    QCOMPARE(m->table->item(2,5)->text(), "Description test of task 3");

    QTest::mouseClick(m->cb_completed, Qt::LeftButton);
    QVERIFY2(m->cb_completed->isChecked(), "'Not completed' checkbox is still not checked after simulating mouse left click.");

    //tables rows number checking
    QCOMPARE(m->table->rowCount(), 2);

    //compairing task 1
    QCOMPARE(m->table->item(0,0)->text(), "Test List 1");
    QCOMPARE(m->table->item(0,2)->text(), "2023/06/19");
    QCOMPARE(m->table->item(0,3)->text(), "Testing title 1");
    QCOMPARE(m->table->item(0,4)->text(), "25");
    QCOMPARE(m->table->item(0,5)->text(), "Description test of task 1");

    //compairing task 2
    QCOMPARE(m->table->item(1,0)->text(), "Test List 2");
    QCOMPARE(m->table->item(1,2)->text(), "2022/12/28");
    QCOMPARE(m->table->item(1,3)->text(), "Testing title 2");
    QCOMPARE(m->table->item(1,4)->text(), "48");
    QCOMPARE(m->table->item(1,5)->text(), "Description test of task 2");
}
