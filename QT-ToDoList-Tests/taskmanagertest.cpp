#include "taskmanagertest.h"
#include "ui_taskmanager.h"

TaskManagerTest::~TaskManagerTest(){
    if(a)
        delete a;
}
void TaskManagerTest::taskLoadingTest()
{
    TaskManager dlg(QString("Task Managing - Testing"));
    dlg.setModal(true);
    dlg.setOrigin(m);
    dlg.show();

    //importing data of first task trough setData() method
    dlg.loadData(m->table->item(0, 2)->text().toUtf8().constData(), m->table->item(0, 3)->text().toUtf8().constData(),
                 m->table->item(0, 4)->text().toUtf8().constData(), m->table->item(0, 5)->text().toUtf8().constData(), m->table->item(0, 1)->text().toUtf8().constData());
    //dlg.setPath("RegularDbTest.cfg");

    //cheking dialog's components
    QVERIFY2(dlg.ui->savebtn, "'Save' button not created.");
    QVERIFY2(dlg.ui->savebtn->isEnabled(), "'Save' button not enabled (it should be).");
    QVERIFY2(dlg.ui->deletebtn, "'Delete' button not created.");
    QVERIFY2(dlg.ui->deletebtn->isVisible(), "'Delete' button not visible (it should be).");
    QVERIFY2(dlg.ui->datepicker, "'Date picker' component not created.");
    QVERIFY2(dlg.ui->title_et, "'Title' textbox not created.");
    QVERIFY2(dlg.ui->description_ed, "'Description' textbox not created.");
    QVERIFY2(dlg.ui->slider, "'Slider' component not created.");

    //verifing the correctness of data imported in the dialog
    QCOMPARE(dlg.ui->datepicker->date(), QDate(2023,06,19));
    QCOMPARE(dlg.ui->title_et->text(), "Testing title 1");
    QCOMPARE(dlg.ui->description_ed->toPlainText(), "Description test of task 1");
    QCOMPARE(dlg.ui->slider->value(), 25);

    //testing checkFields() integrity on description data change
    dlg.ui->description_ed->clear();
    QVERIFY2(!dlg.ui->savebtn->isEnabled(), "'Modify' button is enabled (should be disabled).");

    //repopulating description textbox
    dlg.ui->description_ed->setPlainText(m->table->item(0, 4)->text().toUtf8().constData());
    QVERIFY2(dlg.ui->savebtn->isEnabled(), "'Modify' button is disabled (should be enabled).");

    //testing checkFields() integrity on title data change
    dlg.ui->title_et->clear();
    QVERIFY2(!dlg.ui->savebtn->isEnabled(), "'Modify' button is enabled (should be disabled).");

    //closing dlg
    dlg.hide();
    QVERIFY2(dlg.close(), "Error during dialog close.");

}

void TaskManagerTest::taskAddingTest(){
    TaskManager dlg(QString("Task Adding - Testing"));
    dlg.setModal(true);
    dlg.setOrigin(m);

    //checking if savebutton (Add task) is disabled
    QVERIFY2(!dlg.ui->savebtn->isEnabled(), "'Add task' button is enabled (it should be disabled).");

    //Adding test to the title texbox and checking if savebtn is still disabled
    dlg.ui->title_et->setText(QString("Testing title text"));
    QVERIFY2(!dlg.ui->savebtn->isEnabled(), "'Add task' button is enabled (it should be disabled).");

    //Adding test to the description textbox and checking if savebtn is now enabled
    dlg.ui->description_ed->setPlainText(QString("Testing title text"));
    QVERIFY2(dlg.ui->savebtn->isEnabled(), "'Add task' button is disabled (it should be enabled).");

    //closing dlg
    QVERIFY2(dlg.close(), "Error during dialog close.");
}
