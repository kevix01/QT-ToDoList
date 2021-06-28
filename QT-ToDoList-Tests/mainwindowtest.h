#include <QtTest>
#include <QApplication>
#include <QAction>
#include <mainwindow.h>

class MainWindowTest: public QObject
{
    Q_OBJECT

public:
    MainWindowTest() = default;
    ~MainWindowTest();

private:
    QApplication* a {nullptr};
    MainWindow* m {nullptr};

private slots:
    void initTestCase();

    void UiComponentsConstructionTest();

    void TableComponentTest();

    void cleanupTestCase()
    {
        qDebug("Components' integrity check finished.");
        m->hide();
    }
};
