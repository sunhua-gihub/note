#include "judgepic.h"

//#include <QDebug>
#include <QVariant>
#include <QList>
#include <QMetaType>

Judgepic::Judgepic(QList<QString> *list,QList<QString> *isPNG,
                   QString path,QList<QList<QVariant> > *datas,
                   QString seg,int item,
                   QWidget *parent):
    m_filename(list),
    m_path(path),
    m_PNGResult(isPNG),
    m_datas(datas),
    m_seg(seg),
    m_item(item),
    QThread(parent)
{
    qRegisterMetaType<QHash<int,QList<QString> > >("QHash<int,QList<QString> >");
    m_file = new QFile();
}

Judgepic::~Judgepic()
{
    delete m_file;
}

void Judgepic::run()
{
//判断jpeg文件
    foreach (QString filename, *m_filename) {
        m_file->setFileName(m_path+"/"+filename);
        m_file->open(QIODevice::ReadOnly);
        QByteArray temp = m_file->read(8);

        if(temp[0] == 0xFF && temp[1] == 0xD8)
            m_PNGResult->append("Yes");
        else
            m_PNGResult->append("No");
        m_file->close();
    }

    emit sendPNGResult();
//修改文件名
    QList<QString>::const_iterator iter;
    QList<QString>::const_iterator iter2;
    QHash<int,QList<QString> > newfilename_list;
    QList<QString> file_list;
    QString item;
    int i=0;

    if(-1 == m_item)
    {
        iter=m_filename->constBegin();
        iter2=m_PNGResult->constBegin();
        int i=0;
        foreach (QList<QVariant> list, *m_datas) {
            if("No" == *iter2)
            {
                iter++;
                iter2++;
                i++;
            }
            QString filename = *iter;

            QString newfilename;
            int size = list.size();

            for(int j=0;j<size;j++)
            {
                item = list.at(j).toString();

                if(!item.isEmpty())
                {
                    if(j != 0)
                        newfilename.append(m_seg).append(item);
                    else
                        newfilename.append(item);
                }
            }
            newfilename.append(".jpg");
            file_list.clear();
            file_list.append(newfilename);
            bool temp = QFile::rename(m_path+"/"+filename,m_path+"/"+newfilename);

            if(temp)
                file_list.append("Ok");
            else
                file_list.append("NO");
            newfilename_list.insert(i,file_list);
            iter++;
            iter2++;
            i++;
            if(iter == m_filename->constEnd())
                break;

        }
    }
    else
    {
        foreach (QList<QVariant> list, *m_datas) {

            QString Mark = list.at(m_item).toString();
            if(Mark.isEmpty())
                continue;
            for(iter=m_filename->constBegin(),iter2=m_PNGResult->constBegin(),i=0;
                iter!=m_filename->constEnd();
                ++iter2,++iter,++i)
            {
                if(*iter2 == "No")
                    continue;

                QString filename = *iter;
                if(filename.contains(Mark))
                {
                    QString newfilename;
                    int size = list.size();

                    for(int j=0;j<size;j++)
                    {
                        item = list.at(j).toString();

                        if(!item.isEmpty())
                        {
                            if(j != 0)
                                newfilename.append(m_seg).append(item);
                            else
                                newfilename.append(item);
                        }
                    }
                    newfilename.append(".jpg");
                    file_list.clear();
                    file_list.append(newfilename);
                    bool temp = QFile::rename(m_path+"/"+filename,m_path+"/"+newfilename);

                    if(temp)
                        file_list.append("Ok");
                    else
                        file_list.append("NO");
                    newfilename_list.insert(i,file_list);

                    break;
                }
            }
        }
    }

    emit sendRNAMEResult(newfilename_list);
}
