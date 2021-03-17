
#include "Main_interface.h"
#include <QTextStream>
#include <QLabel>
#include <QApplication>
#include <QWidget>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Main_interface mI;
    mI.show();
   

    return a.exec();
}
