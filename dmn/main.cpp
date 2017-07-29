#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    if(w.noPermissions(QCoreApplication::applicationDirPath() + "/" + "dual_monitor_nitrogen"))
    {
        w.close();
        return 1;
    }

    w.show();

    return a.exec();
}
