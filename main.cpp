
#include "widget.h"
#include <QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("myFirstChat");
    a.setApplicationDisplayName("myFirstChat");

    Widget w;
    w.setWindowTitle("чат");
    w.show();

    QTimer::singleShot(1000, &w, &Widget::connectToServer);

    return a.exec();
}
