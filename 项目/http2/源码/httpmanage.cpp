#include "httpmanage.h"

//#include <QDebug>
HttpManage* HttpManage::m_http = NULL;
HttpManage::HttpManage()
{
   manager = new QNetworkAccessManager();
   QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
}

HttpManage* HttpManage::instance()
{
    if(m_http == NULL) {
        m_http = new HttpManage();
    }

    return m_http;
}

void HttpManage::replyFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        emit sendRecvInfo(reply->errorString());
    }
    else
    {
        QString all = reply->readAll();
        emit sendRecvInfo(all);
        reply->deleteLater();
    }
}

void HttpManage::connecttarget(QString url)
{
    m_url = url;

    request.setUrl(m_url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Connection","Keep-Alive");
}

QNetworkReply* HttpManage::postinfo(const QByteArray &data)
{
    return manager->post(request,data);
}
