#include "judgename.h"

//#include <QDebug>

/*************************************************
 * 参数说明：
 * list:    文件名列表
 * isJPEG:  是否是JPEG文件
 * format:  名字格式列表   第一项是：拼接字段，第二项是：标识（0 字段,0< 自增量 且代表前导零位数再加1）
 * path:    文件上一级路径
 * seg:     分隔符
 * mark:    标识位(不变位)
 *
 *************************************************/
judgename::judgename(QList<QString> *list,QList<QString> *isJPEG,
                     QList<QList<QString> > *format,QString path,
                     QString seg,int mark,
                     QWidget *parent):
    m_filename(list),
    m_JPEGResult(isJPEG),
    m_format(format),
    m_path(path),
    m_seg(seg),
    m_mark(mark),
    QThread(parent)
{
    qRegisterMetaType<QHash<int,QList<QString> > >("QHash<int,QList<QString> >");
    m_file = new QFile();
}

judgename::~judgename()
{
    delete m_file;
}

void judgename::run()
{
//判断jpeg文件
    foreach (QString filename, *m_filename) {
        m_file->setFileName(m_path+"/"+filename);
        m_file->open(QIODevice::ReadOnly);
        QByteArray temp = m_file->read(8);

        if(temp[0] == 0xFF && temp[1] == 0xD8)
            m_JPEGResult->append("Yes");
        else
            m_JPEGResult->append("No");
        m_file->close();
    }

    emit sendPNGResult();
//修改文件名
    int len=10;
    int tempnum = 0;
    int formatsize;

    formatsize=m_format->size();

    foreach (QList<QString> list, *m_format) {
        tempnum = list.at(1).toInt();
        if(0 != tempnum && tempnum<len)
            len = tempnum;
    }

    QHash<int,QList<QString> > newfilename_list;
    QList<QString>::const_iterator iter;
    QList<QString>::const_iterator iter2;
    QList<QString> file_name;
    QList<QString> oldfilename_item;
    QString oldfilename;
    QString newfilename;

    int i,j,k;
    for(iter=m_filename->constBegin(),iter2=m_JPEGResult->constBegin(),i=0,k=0;
        iter != m_filename->constEnd();
        ++iter2,++iter,i++)
    {
        if(*iter2 == "No"){
            k++;
            continue;
        }
        oldfilename = *iter;
        if(0 != m_mark)
        {
            QString tempfilename;
            tempfilename = oldfilename.split(".").at(0);
            oldfilename_item.clear();
            oldfilename_item=tempfilename.split(m_seg);
            //oldfilename_item=oldfilename.split(m_seg);
            j=0;
        }
        newfilename.clear();
        foreach (QList<QString> list, *m_format) {
            if(0 != m_mark)
            {
                j++;
                if(j == m_mark)
                {
                    newfilename.append(oldfilename_item.at(j-1)).append(m_seg);
                }
            }
            tempnum = list.at(1).toInt();
            if(list != m_format->back())
            {
                if(0 == tempnum)
                {
                    newfilename.append(list.at(0)).append(m_seg);
                }
                else
                {
                    QString str = QString("%1").arg(i+1-k,len,10,QChar('0'));
                    QString strp = list.at(0);
                    int strplen = strp.length();
                    strp = strp.left(strplen-tempnum);
                    newfilename.append(strp).append(str).append(m_seg);
                }
            }
            else
            {
                if(!m_seg.isEmpty())
                {
                    int templen = newfilename.lastIndexOf(m_seg);
                    newfilename = newfilename.left(templen);
                }
                newfilename.append(list.at(0));
            }
        }
        if(formatsize < m_mark && m_mark <= oldfilename_item.size())
        {
            QString tempname=oldfilename_item.at(m_mark-1);
            tempname.append(newfilename);
            newfilename=tempname;
        }
        file_name.clear();
        file_name.append(newfilename);
        bool temp = QFile::rename(m_path+"/"+oldfilename,m_path+"/"+newfilename);
        if(temp)
            file_name.append("Ok");
        else
            file_name.append("NO");
        newfilename_list.insert(i,file_name);
    }

    emit sendRNAMEResult(newfilename_list);
}
