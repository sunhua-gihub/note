#ifndef JUDGEPIC_H
#define JUDGEPIC_H

#include <QThread>
#include <QList>
#include <QWidget>
#include <QFile>
#include <QHash>

class Judgepic : public QThread
{
    Q_OBJECT
public:
    explicit Judgepic(QList<QString> *list=0, QList<QString> *isPNG=0,
                      QString path=NULL, QList<QList<QVariant> > *datas=0,
                      QString seg=NULL, int item=0,
                      QWidget *parent=0);
    ~Judgepic();
signals:
    void sendPNGResult();
    void sendRNAMEResult(QHash<int,QList<QString> >);
protected:
    void run();
private:
    QList<QString> *m_filename;
    QList<QString> *m_PNGResult;
    QList<QList<QVariant> > *m_datas;
    QString m_path;
    QString m_seg;
    QFile *m_file;
    int m_item;
};

#endif // JUDGEPIC_H
