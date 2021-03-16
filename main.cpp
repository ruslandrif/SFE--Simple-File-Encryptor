#include "test_task.h"

#include <QTextStream>
#include <QLabel>
#include <QApplication>
#include <QWidget>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    w.setGeometry(0, 0, 500, 500);
    w.show();
    QLabel lbl(&w);

    lbl.setText("Some content coming soon");
    lbl.setGeometry(200, 200, w.width() / 2, w.height() / 2);
    lbl.show();

    return a.exec();
}
