#ifndef JUDGENAME_H
#define JUDGENAME_H

#include <QThread>
#include <QList>
#include <QWidget>
#include <QFile>
#include <QHash>

class judgename : public QThread
{
    Q_OBJECT
public:
    explicit judgename(QList<QString> *list=0,QList<QString> *isJPEG=0,
                       QList<QList<QString> > *format=0,QString path=NULL,
                       QString seg=NULL,int mark=0,
                       QWidget *parent=0);
    ~judgename();
signals:
    void sendPNGResult();
    void sendRNAMEResult(QHash<int,QList<QString> >);
protected:
    void run();
private:
    QList<QList<QString> > *m_format;
    QList<QString> *m_filename;
    QList<QString> *m_JPEGResult;
    QFile *m_file;
    QString m_path;
    QString m_seg;
    int m_mark;
};

#endif // JUDGENAME_H
