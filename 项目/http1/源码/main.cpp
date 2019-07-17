#include "mainui.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainui w;
    w.show();

    QFile *file = new QFile("://qss/psblack.css");
    file->open(QFile::ReadOnly);
    QString style = QString(file->readAll());
    file->close();
    qApp->setStyleSheet(style);

    return a.exec();
}
