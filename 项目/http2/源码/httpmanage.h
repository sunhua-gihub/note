#ifndef HTTPMANAGE_H
#define HTTPMANAGE_H

#include <QtNetwork>

class HttpManage : public QObject
{
    Q_OBJECT
public:
    HttpManage();
    static HttpManage* instance();
    void connecttarget(QString);
    QNetworkReply *postinfo(const QByteArray &data);
    QString text() {return m_url;}
signals:
    void sendRecvInfo(QString);
private slots:
    void replyFinished(QNetworkReply *);
private:
    static HttpManage* m_http;
    QString m_url;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};

#define g_httpmanage HttpManage::instance()
#endif // HTTPMANAGE_H
