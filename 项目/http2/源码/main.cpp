#include "widget.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile *file = new QFile("://qss/psblack.css");
    file->open(QFile::ReadOnly);
    QString style = QString(file->readAll());
    file->close();
    qApp->setStyleSheet(style);

    Widget w;
    w.show();

    return a.exec();
}

