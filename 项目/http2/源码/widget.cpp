#include "widget.h"
#include "ui_widget.h"

#include "httpmanage.h"
#include "filechoose.h"
#include "personmanage.h"

#include <time.h>
//#include <QDebug>

bool PersonManageTemp = true;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);

    max = false;
    mDragWindow = false;
    location = geometry();

    connect(g_httpmanage,HttpManage::sendRecvInfo,[ = ](QString info){
        ui->textBrowser->append(info);
    });

    QList<QPushButton *> btns = ui->leftwidget->findChildren<QPushButton *>();
    foreach(QPushButton *btn, btns) {
        connect(btn,&QPushButton::clicked,this,&Widget::Slot_BtnGrounpClicked);
        btn->setCheckable(true);
    }

    QList<QToolButton *> tbtns = ui->toolwidget->findChildren<QToolButton *>();
    foreach(QToolButton *btn, tbtns) {
        connect(btn,&QToolButton::clicked,this,&Widget::Slot_TBtnGrounpClicked);
    }

    PersonManage::instance()->setParent(this);
    //PersonManage::instance()->setFixedSize(600,380);
    //PersonManage::instance()->setGeometry(0,height()-420,600,380);
    PersonManage::instance()->hide();
    connect(PersonManage::instance(),&PersonManage::timetohide,this,&Widget::Slot_PersonMangeHide);
    connect(PersonManage::instance(),&PersonManage::sendErr,this,&Widget::showErr);
    connect(PersonManage::instance(),&PersonManage::sendJsonData,this,&Widget::Slot_RecvJsonData);

    CaptureEnabledbox.addButton(ui->SetParam_Unablerbtn,0);
    CaptureEnabledbox.addButton(ui->SetParam_Enablerbtn,1);
    ui->SetParam_Unablerbtn->setChecked(true);

    FaceInfobox.addButton(ui->SetParam_NoFaceInforbtn,0);
    FaceInfobox.addButton(ui->SetParam_FaceInforbtn,1);
    ui->SetParam_NoFaceInforbtn->setChecked(true);

    CompareInfobox.addButton(ui->SetParam_NoCompareInforbtn,0);
    CompareInfobox.addButton(ui->SetParam_CompareInforbtn,1);
    ui->SetParam_NoCompareInforbtn->setChecked(true);

    FacePicturebox.addButton(ui->SetParam_NoFaceDatarbtn,0);
    FacePicturebox.addButton(ui->SetParam_FaceDatarbtn,1);
    ui->SetParam_NoFaceDatarbtn->setChecked(true);

    BackgroundPicturebox.addButton(ui->SetParam_NoBackDatarbtn,0);
    BackgroundPicturebox.addButton(ui->SetParam_BackDatarbtn,1);
    ui->SetParam_NoBackDatarbtn->setChecked(true);

    PersonPhotobox.addButton(ui->SetParam_NoPersonDatarbtn,0);
    PersonPhotobox.addButton(ui->SetParam_PersonDatarbtn,1);
    ui->SetParam_NoPersonDatarbtn->setChecked(true);

    RegisterEnabledbox.addButton(ui->SetParam_RegisterUnabledrbtn,0);
    RegisterEnabledbox.addButton(ui->SetParam_RegisterEnabledrbtn,1);
    ui->SetParam_RegisterUnabledrbtn->setChecked(true);

    HeartbeatEnabledbox.addButton(ui->SetParam_HeartbeatUnabledrbtn,0);
    HeartbeatEnabledbox.addButton(ui->SetParam_HeartbeatEnabledrbtn,1);
    ui->SetParam_HeartbeatUnabledrbtn->setChecked(true);

    SignCheckbox.addButton(ui->SetParam_SignUnCheckrbtn,0);
    SignCheckbox.addButton(ui->SetParam_SignCheckrbtn,1);
    ui->SetParam_SignUnCheckrbtn->setChecked(true);

    GetPhotobox.addButton(ui->GetPersonInfo_NoReturnPicrbtn,0);
    GetPhotobox.addButton(ui->GetPersonInfo_ReturnPicrbtn,1);
    ui->GetPersonInfo_NoReturnPicrbtn->setChecked(true);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if (!max && e->button() == Qt::LeftButton) {
        mMousePoint = e->globalPos() - this->pos();
        e->accept();
        mDragWindow = true;
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    mDragWindow = false;
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if (mDragWindow && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mMousePoint);
        e->accept();
    }
}

void Widget::on_closebtn_clicked()
{
    close();
}

void Widget::on_connectbtn_clicked()
{
    QString url = ui->addresslineEdit->text();
    if(url.isEmpty()) {
        showErr(ERR_COLOR," error: url is null.");
        return;
    }

    g_httpmanage->connecttarget(url);
    showErr(NO_COLOR," : url set success.");
}

void Widget::showErr(int type,QString err)
{
    m_time = QTime::currentTime();
    QString time_str = m_time.toString("hh:mm:ss");
    switch(type)
    {
        case ERR_COLOR:
            ui->errortextbrowser->setTextColor(QColor(255,0,0));
        break;
        case WARN_COLOR:
            ui->errortextbrowser->setTextColor(QColor(255,255,0));
        break;
        default:
            ui->errortextbrowser->setTextColor(QColor(0,200,0));
        break;
    }

    ui->errortextbrowser->append("["+ time_str + "]" +err);
     ui->errortextbrowser->setTextColor(QColor(0,200,0));
}

void Widget::on_sendbtn_clicked()
{
    if(g_httpmanage->text().isEmpty()) {
        showErr(ERR_COLOR, " error: url is null.");
        return;
    }
    int flag = 0;
    QJsonObject Jdata;
    int page = ui->bottom_stW->currentIndex();
    switch(page) {
        case 0:
            http_Register(&Jdata);
        break;
        case 1:
            http_HeartBeat(&Jdata);
        break;
        case 2:
            http_GetPersonList(&Jdata);
        break;
        case 3:
            http_GetPersonInfo(&Jdata);
        break;
        case 4:
            showErr(WARN_COLOR,"Nothing to do.");
            return;
        break;
        case 5:
            showErr(WARN_COLOR,"Nothing to do.");
            return;
        break;
        case 6:
            http_DelPersonList(&Jdata);
        break;
        case 7:
            http_DelPersonInfo(&Jdata);
        break;
        case 8:
            http_GetDeviceInfo(&Jdata);
        break;
        case 9:
            http_IOControl(&Jdata);
        break;
        case 10:
            http_GetParam(&Jdata);
        break;
        case 11:
            http_SetParam(&Jdata);
        break;
        case 12:
            http_SynTime(&Jdata);
        break;

    }

    if(flag != 0) {
        return;
    }

    QJsonDocument JSonDoc;
    JSonDoc.setObject(Jdata);

    QNetworkReply *reply = g_httpmanage->postinfo(JSonDoc.toJson());

    if(reply->error() != QNetworkReply::NoError)
    {
        showErr(ERR_COLOR, reply->errorString());
    }
    else
    {
        showErr(NO_COLOR," :send success.");
    }
}

int Widget::http_Register(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QJsonObject obj1,obj2;
    data->insert("Name","registerRequest");
    data->insert("TimeStamp",QString::number(TimeStamp));
    obj1.insert("DeviceId",ui->devidEdit->text());
    obj1.insert("DeviceUUID",ui->devuuidEdit->text());
    obj1.insert("DeviceMac",ui->devmacEdit->text());
    obj1.insert("DeviceType",ui->devtypeEdit->text().toInt());
    obj1.insert("ChannelNum",ui->channelnumEdit->text().toInt());
    obj1.insert("WebVersion",ui->webversionEdit->text());
    obj1.insert("CoreVersion",ui->coreversionEdit->text());
    obj1.insert("VersionDate",ui->versiondateEdit->text());
    obj2.insert("DeviceInfo",obj1);
    obj2.insert("HTTPVersion",ui->httpversionEdit->text());
    obj2.insert("HTTPDate",ui->httpdateEdit->text());
    data->insert("Data",obj2);

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"registerRequest\" \n"
                "   \"TimeStamp\" : \"" + QString::number(TimeStamp) + "\"\n"
                "   \"Data\" : \n"
                "   { \n"
                "       \"DeviceInfo\" :\n"
                "       {\n"
                "           \"DeviceId\" : \"" + ui->devidEdit->text() + "\"\n"
                "           \"DeviceUUID\" : \"" + ui->devuuidEdit->text() + "\"\n"
                "           \"DeviceMac\" : \"" + ui->devmacEdit->text() + "\"\n"
                "           \"DeviceType\" : " + ui->devtypeEdit->text() + "\n"
                "           \"ChannelNum\" : " + ui->channelnumEdit->text() + "\n"
                "           \"WebVersion\" : \"" + ui->webversionEdit->text() + "\"\n"
                "           \"CoreVersion\" : \"" + ui->coreversionEdit->text() + "\"\n"
                "           \"VersionDate\" : \"" + ui->versiondateEdit->text() + "\"\n"
                "       }\n"
                "       \"HTTPVersion\" : \"" + ui->httpversionEdit->text() + "\"\n"
                "       \"HTTPDate\" : \"" + ui->httpdateEdit->text() + "\"\n"
                "   } \n"
                "} \n");
    return 0;
}

int Widget::http_HeartBeat(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QJsonObject obj1,obj2;
    data->insert("Name","heartbeatRequest");
    data->insert("TimeStamp",QString::number(TimeStamp));
    data->insert("Session",ui->heartbeat_seesionEdit->text());
    obj1.insert("DeviceId",ui->heartbeat_devidEdit->text());
    obj1.insert("DeviceUUID",ui->heartbeat_devuuidEdit->text());
    obj1.insert("DeviceMac",ui->heartbeat_devmacEdit->text());
    obj1.insert("ChannelNo",ui->heartbeat_channelEdit->text().toInt());
    obj1.insert("WebVersion",ui->heartbeat_webversionEdit->text());
    obj1.insert("CoreVersion",ui->heartbeat_coreversionEdit->text());
    obj1.insert("VersionDate",ui->heartbeat_versiondateEdit->text());
    obj2.insert("DeviceInfo",obj1);
    obj2.insert("HTTPVersion",ui->heartbeat_httpversionEdit->text());
    obj2.insert("HTTPDate",ui->heartbeat_httpdateEdit->text());
    obj2.insert("HeartbeatCount",ui->heartbeat_countEdit->text());
    obj2.insert("CaptureCount",ui->heartbeat_capturecountEdit->text());
    data->insert("Data",obj2);

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"heartbeatRequest\" \n"
                "   \"TimeStamp\" : " + QString::number(TimeStamp) + "\n"
                "   \"Session\" : \"" + ui->heartbeat_seesionEdit->text() + "\"\n"
                "   \"Data\" : \n"
                "   {\n"
                "       \"DeviceInfo\" :\n"
                "       {\n"
                "           \"DeviceId\" : \"" + ui->heartbeat_devidEdit->text() + "\"\n"
                "           \"DeviceUUID\" : \"" + ui->heartbeat_devuuidEdit->text() + "\"\n"
                "           \"DeviceMac\" : \"" + ui->heartbeat_devmacEdit->text() + "\"\n"
                "           \"ChannelNo\" : " + ui->heartbeat_channelEdit->text() + "\n"
                "           \"WebVersion\" : \"" + ui->heartbeat_webversionEdit->text() + "\"\n"
                "           \"CoreVersion\" : \"" + ui->heartbeat_coreversionEdit->text() + "\"\n"
                "           \"VersionDate\" : \"" + ui->heartbeat_versiondateEdit->text() + "\"\n"
                "       } \n"
                "       \"HTTPVersion\" : \"" + ui->heartbeat_httpversionEdit->text() + "\"\n"
                "       \"HTTPDate\" : \"" + ui->heartbeat_httpdateEdit->text() + "\"\n"
                "       \"HeartbeatCount\" : " + ui->heartbeat_countEdit->text() + "\n"
                "       \"CaptureCount \" : " + ui->heartbeat_capturecountEdit->text() + "\n"
                "   }\n"
                "} \n");
    return 0;
}

int Widget::http_GetPersonList(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QJsonObject obj1;

    QString Session = ui->GetPersonList_seesionEdit->text();
    QStringList sessions = Session.split("_");
    QString UUID = nullptr;
    if(sessions.size() != 0) {
        UUID = sessions.at(0);
    }
    data->insert("Name","personListRequest");
    data->insert("UUID",UUID);
    data->insert("Session",Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign","");
    obj1.insert("Action","getPersonList");
    obj1.insert("PersonType",ui->GetPersonList_typeComboBox->currentIndex()+1);
    data->insert("Data",obj1);

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"personListRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\" \n"
                "   \"Session\" : \"" + Session + "\" \n"
                "   \"TimeStamp\" : " + QString::number(TimeStamp) + " \n"
                "   \"Sign\" : \"\" \n"
                "   \"Data\" : \n"
                "   {\n"
                "       \"Action\" : \"getPersonList\"\n"
                "       \"PersonType\" : " + QString::number(ui->GetPersonList_typeComboBox->currentIndex()+1) + "\n"
                "   }\n"
                "} \n");
    return 0;
}

int Widget::http_GetPersonInfo(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QJsonObject obj1;

    QString Session = ui->GetPersonInfo_seesionEdit->text();
    QStringList sessions = Session.split("_");
    QString UUID = nullptr;
    if(sessions.size() != 0) {
        UUID = sessions.at(0);
    }
    data->insert("Name","personListRequest");
    data->insert("UUID",UUID);
    data->insert("ChannelNo","");
    data->insert("Session",Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign","");
    obj1.insert("Action","getPerson");
    obj1.insert("PersonType",ui->GetPersonInfo_typeComboBox->currentIndex()+1);
    obj1.insert("PersonId",ui->GetPersonInfo_idEidt->text());
    obj1.insert("GetPhoto",GetPhotobox.checkedId());
    data->insert("Data",obj1);

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"personListRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\" \n"
                "   \"ChannelNo\" : \n"
                "   \"Session\" : \"" + Session + "\" \n"
                "   \"TimeStamp\" : " + QString::number(TimeStamp) + " \n"
                "   \"Sign\" : \"\" \n"
                "   \"Data\" : \n"
                "   {\n"
                "       \"Action\" : \"getPerson\"\n"
                "       \"PersonType\" : " + QString::number(ui->GetPersonInfo_typeComboBox->currentIndex()+1) + "\n"
                "       \"PersonId\" : \"" + ui->GetPersonInfo_idEidt->text() + "\"\n"
                "       \"GetPhoto\" : " + QString::number(GetPhotobox.checkedId()) + "\n"
                "   }\n"
                "} \n");
    return 0;
}

int Widget::http_AddPersonInfo(QJsonObject *data)
{
    return 0;
}

int Widget::http_EditPersonInfo(QJsonObject *data)
{
    return 0;
}

int Widget::http_DelPersonList(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QJsonObject obj1;

    QString Session = ui->DelPersonList_seesionEdit->text();
    QStringList Sessions = Session.split("_");
    QString UUID = nullptr;
    if(Sessions.size() != 0) {
        UUID = Sessions.at(0);
    }
    data->insert("Name","personListRequest");
    data->insert("UUID",UUID);
    data->insert("Session",Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign","");
    obj1.insert("Action","deletePersonList");
    obj1.insert("PersonType",ui->DelPersonList_typeComboBox->currentIndex()+1);

    data->insert("Data",obj1);

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"personListRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\"\n"
                "   \"Session\" : \"" + Session + "\"\n"
                "   \"TimeStamp\" : \"" + QString::number(TimeStamp) + "\"\n"
                "   \"Sign\" : \"\"\n"
                "   \"Data\" : \n"
                "   {\n"
                "       \"Action\" : \"deletePersonList\"\n"
                "       \"PersonType\" : " + QString::number(ui->DelPersonList_typeComboBox->currentIndex()+1) + "\n"
                "   }\n"
                "} \n");
    return 0;
}

int Widget::http_DelPersonInfo(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QJsonObject obj1;

    QString Session = ui->DelPersonInfo_seesionEdit->text();
    QStringList Sessions = Session.split("_");
    QString UUID = nullptr;
    if(Sessions.size() != 0) {
        UUID = Sessions.at(0);
    }
    data->insert("Name","personListRequest");
    data->insert("UUID",UUID);
    data->insert("Session",Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign","");
    obj1.insert("Action","deletePerson");
    obj1.insert("PersonType",ui->DelPersonInfo_typeComboBox->currentIndex()+1);
    obj1.insert("PersonId",ui->DelPersonInfo_idEdit->text());
    data->insert("Data",obj1);

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"personListRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\"\n"
                "   \"Session\" : \"" + Session + "\"\n"
                "   \"TimeStamp\" : \"" + QString::number(TimeStamp) + "\"\n"
                "   \"Sign\" : \"\"\n"
                "   {\n"
                "       \"Action\" : \"deletePerson\"\n"
                "       \"PersonType\" : " + QString::number(ui->DelPersonInfo_typeComboBox->currentIndex()+1) + "\n"
                "       \"PersonId\" : \"" + ui->DelPersonInfo_idEdit->text() + "\"\n"
                "   }\n"
                "} \n");
    return 0;
}

int Widget::http_GetDeviceInfo(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QString Session = ui->GetDeviceInfo_seesionEdit->text();
    QStringList Sessions = Session.split("_");
    QString UUID = nullptr;
    if(Sessions.size() != 0) {
        UUID = Sessions.at(0);
    }
    data->insert("Name","deviceInfoRequest");
    data->insert("UUID",UUID);
    data->insert("Session", Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign", "");

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"deviceInfoRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\" \n"
                "   \"Session\" : \"" + Session + "\"\n"
                "   \"TimeStamp\" : \"" + QString::number(TimeStamp) + "\" \n"
                "   \"Sign\" : \"\" \n"
                "} \n");
    return 0;
}

int Widget::http_IOControl(QJsonObject *data)
{
#if 0
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QJsonObject obj1;

    QString Session = ui->GetDeviceInfo_seesionEdit->text();
    QStringList Sessions = Session.split("_");
    QString UUID = nullptr;
    if(Sessions.size() != 0) {
        UUID = Sessions.at(0);
    }
    data->insert("Name","deviceInfoRequest");
    data->insert("UUID",UUID);
    data->insert("Session", Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign", "");
    obj1.insert("ContinueSeconds",ui->IO_DurationtimeEdit->text().toInt());
    data->insert("Data",obj1);
    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"deviceInfoRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\" \n"
                "   \"Session\" : \"" + Session + "\"\n"
                "   \"TimeStamp\" : \"" + QString::number(TimeStamp) + "\" \n"
                "   \"Sign\" : \"\" \n"
                "   \"Data\" : \n"
                "   {\n"
                "       \"ContinueSeconds\" : " + ui->IO_DurationtimeEdit->text() +"\n"
                "   }\n"
                "} \n");
    return 0;
#endif
    QString starttime = ui->IO_starttimeEdit->text();
    QString endtime = ui->IO_endtimeEdit->text();

    if(ui->IO_typebox->currentIndex() == 0) {
        data->insert("Action","GetCompareRecordNum");
        data->insert("starttime",starttime);
        data->insert("endtime", endtime);
        ui->errortextbrowser->append(
                    "{ \n"
                    "   \"Action\" : \"GetCompareRecordNum\" \n"
                    "   \"starttime\" : \"" + starttime + "\" \n"
                    "   \"endtime\" : \"" + endtime + "\"\n"
                    "} \n");
    }else if(ui->IO_typebox->currentIndex() == 1) {
        int number = ui->IO_numberEdit->text().toInt();
        data->insert("Action","GetCompareRecordInfo");
        data->insert("starttime",starttime);
        data->insert("endtime", endtime);
        data->insert("Number",number);
        ui->errortextbrowser->append(
                    "{ \n"
                    "   \"Action\" : \"GetCompareRecordInfo\" \n"
                    "   \"starttime\" : \"" + starttime + "\" \n"
                    "   \"endtime\" : \"" + endtime + "\"\n"
                    "   \"Number\" : " + QString::number(number) + "\n"
                    "} \n");
    }
}

int Widget::http_GetParam(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QString Session = ui->GetParam_seesionEdit->text();
    QStringList Sessions = Session.split("_");
    QString UUID = nullptr;
    if(Sessions.size() != 0) {
        UUID = Sessions.at(0);
    }
    data->insert("Name","getHTTPParametersRequest");
    data->insert("UUID",UUID);
    data->insert("Session", Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign", "");

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"getHTTPParametersRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\" \n"
                "   \"Session\" : \"" + Session + "\"\n"
                "   \"TimeStamp\" : \"" + QString::number(TimeStamp) + "\" \n"
                "   \"Sign\" : \"\" \n"
                "} \n");
    return 0;
}

int Widget::http_SetParam(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QJsonObject obj1,obj2,obj3;

    QString Session = ui->GetParam_seesionEdit->text();
    QStringList Sessions = Session.split("_");
    QString UUID = nullptr;
    if(Sessions.size() != 0) {
        UUID = Sessions.at(0);
    }
    data->insert("Name","getHTTPParametersRequest");
    data->insert("UUID",UUID);
    data->insert("ChannelNo", ui->SetParam_channelEdit->text().toInt());
    data->insert("Session", Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign", "");
    obj1.insert("CaptureEnabled",CaptureEnabledbox.checkedId());
    obj1.insert("CaptureAddress", ui->SetParam_CaptureAddressEdit->text());
    obj1.insert("CaptureType",ui->SetParam_CaptureTypeBox->currentIndex());
    obj2.insert("FaceInfo", FaceInfobox.checkedId());
    obj2.insert("CompareInfo", CompareInfobox.checkedId());
    obj1.insert("CaptureContent",obj2);
    obj3.insert("FacePicture",FacePicturebox.checkedId());
    obj3.insert("BackgroundPicture",BackgroundPicturebox.checkedId());
    obj3.insert("PersonPhoto",PersonPhotobox.checkedId());
    obj1.insert("PictureData",obj3);
    obj1.insert("ResendTimes",ui->SetParam_ResendTimesEdit->text().toInt());
    obj1.insert("RegisterEnabled",RegisterEnabledbox.checkedId());
    obj1.insert("RegisterAddress",ui->SetParam_RegisterAddressEdit->text());
    obj1.insert("HeartbeatEnabled",HeartbeatEnabledbox.checkedId());
    obj1.insert("HeartbeatAddress",ui->SetParam_HeartbeatAddressEdit->text());
    obj1.insert("HeartbeatInterval",ui->SetParam_HeartbeatIntervalEdit->text().toInt());
    obj1.insert("SignCheck",SignCheckbox.checkedId());
    obj1.insert("HTTPVersion",ui->SetParam_HTTPVersionEdit->text().toInt());
    data->insert("Data",obj1);
    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"setHTTPParametersRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\" \n"
                "   \"ChannelNo\" : " + ui->SetParam_channelEdit->text() + "\n"
                "   \"Session\" : \"" + Session + "\"\n"
                "   \"TimeStamp\" : \"" + QString::number(TimeStamp) + "\" \n"
                "   \"Sign\" : \""  "\" \n"
                "   \"Data\" : \n"
                "   {\n"
                "       \"CaptureEnabled\" : " +  QString::number(CaptureEnabledbox.checkedId()) + "\n"
                "       \"CaptureAddress\" : \"" + ui->SetParam_CaptureAddressEdit->text() + "\"\n"
                "       \"CaptureType\" : " + QString::number(ui->SetParam_CaptureTypeBox->currentIndex()) + "\n"
                "       \"CaptureContent\" : \n"
                "       {\n"
                "           \"FaceInfo\" : " +  QString::number(FaceInfobox.checkedId()) + "\n"
                "           \"CompareInfo\" : " +  QString::number(CompareInfobox.checkedId()) + "\n"
                "       }\n"
                "       \"PictureData\" : \n"
                "       {\n"
                "           \"FacePicture\" : " +  QString::number(FacePicturebox.checkedId()) +"\n"
                "           \"BackgroundPicture\" : " +  QString::number(BackgroundPicturebox.checkedId()) + "\n"
                "           \"PersonPhoto\" : " +  QString::number(PersonPhotobox.checkedId()) + "\n"
                "       }\n"
                "   }\n"
                "   \"ResendTimes\" : " + ui->SetParam_ResendTimesEdit->text() + "\n"
                "   \"RegisterEnabled\" : " + QString::number(RegisterEnabledbox.checkedId()) + "\n"
                "   \"RegisterAddress\" : \"" + ui->SetParam_RegisterAddressEdit->text() + "\" \n"
                "   \"HeartbeatEnabled\" : " + QString::number(HeartbeatEnabledbox.checkedId()) + "\n"
                "   \"HeartbeatAddress\" : \"" + ui->SetParam_HeartbeatAddressEdit->text() + "\" \n"
                "   \"HeartbeatInterval\" : " + ui->SetParam_HeartbeatIntervalEdit->text() + "\n"
                "   \"SignCheck\" : " + ui->SetParam_HTTPVersionEdit->text() + "\n"
                "   \"HTTPVersion\" : " + QString::number(ui->SetParam_HTTPVersionEdit->text().toInt()) + " \n"
                "} \n");
    return 0;
}

int Widget::http_SynTime(QJsonObject *data)
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);

    QString Session = ui->GetParam_seesionEdit->text();
    QStringList Sessions = Session.split("_");
    QString UUID = nullptr;
    if(Sessions.size() != 0) {
        UUID = Sessions.at(0);
    }
    data->insert("Name","timeSynchronizationRequest");
    data->insert("UUID",UUID);
    data->insert("Session", Session);
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign", "");

    ui->errortextbrowser->append(
                "{ \n"
                "   \"Name\" : \"timeSynchronizationRequest\" \n"
                "   \"UUID\" : \"" + UUID + "\" \n"
                "   \"Session\" : \"" + Session + "\"\n"
                "   \"TimeStamp\" : \"" + QString::number(TimeStamp) + "\" \n"
                "   \"Sign\" : \"\" \n"
                "} \n");
    return 0;
}

void Widget::on_clearbtn_clicked()
{
    ui->textBrowser->clear();
    ui->errortextbrowser->clear();
}

QString Widget::getBase64(QString path)
{
    showErr(NO_COLOR,path);
    if(path.isEmpty()) {
        showErr(WARN_COLOR,"[warning]: path is null.");
        return NULL;
    }
    QImage image(path);
    if(image.isNull()) {
        showErr(ERR_COLOR,"[error]: pic is null.");
        return NULL;
    }
    QByteArray ba;
    QBuffer buf(&ba);

    image.save(&buf,"JPG");
    buf.close();
    return ba.toBase64();
}

void Widget::Slot_BtnGrounpClicked()
{
    QPushButton *btn = (QPushButton *)sender();

    btn->setChecked(true);

    QList<QPushButton *> btns = ui->leftwidget->findChildren<QPushButton *>();
    foreach(QPushButton *obtn, btns) {
        if(obtn != btn) {
            obtn->setChecked(false);
        }
    }

    QString name = btn->text();
    if(name == "注册")
        ui->bottom_stW->setCurrentIndex(0);
    else if(name == "心跳")
        ui->bottom_stW->setCurrentIndex(1);
    else if(name == "获取名单")
        ui->bottom_stW->setCurrentIndex(2);
    else if(name == "获取个人")
        ui->bottom_stW->setCurrentIndex(3);
    else if(name == "添加个人") {
        ui->bottom_stW->setCurrentIndex(4);
        PersonManage::instance()->setPersonManagerType(PersonManage::ADDPERSON);
        PersonManage::instance()->show();
        PersonManage::instance()->setGeometry(0,height()-420,600,380);
        MoveAni(PersonManage::instance(),500,PersonManageTemp);
        PersonManageTemp = !PersonManageTemp;
    }
    else if(name == "修改个人") {
        ui->bottom_stW->setCurrentIndex(5);
        PersonManage::instance()->setPersonManagerType(PersonManage::EDITPERSON);
        PersonManage::instance()->show();
        PersonManage::instance()->setGeometry(0,height()-420,600,380);
        MoveAni(PersonManage::instance(),500,PersonManageTemp);
        PersonManageTemp = !PersonManageTemp;
    }
    else if(name == "删除名单")
        ui->bottom_stW->setCurrentIndex(6);
    else if(name == "删除个人")
        ui->bottom_stW->setCurrentIndex(7);
    else if(name == "获取设备")
        ui->bottom_stW->setCurrentIndex(8);
    else if(name == "IO控制")
        ui->bottom_stW->setCurrentIndex(9);
    else if(name == "参数获取")
        ui->bottom_stW->setCurrentIndex(10);
    else if(name == "参数设置")
        ui->bottom_stW->setCurrentIndex(11);
    else if(name == "时间同步")
        ui->bottom_stW->setCurrentIndex(12);
}

void Widget::Slot_TBtnGrounpClicked()
{
    QToolButton *tbtn = (QToolButton *)sender();

    QString name = tbtn->text();
    if(name == "设备信息")
        ui->left_stW->setCurrentIndex(0);
    else if(name == "人员信息")
        ui->left_stW->setCurrentIndex(1);
}

void Widget::MoveAni(QWidget *tarWgt,int during, bool dir)
{
    if(tarWgt== nullptr) return;
    QPropertyAnimation * pMoveAni = new QPropertyAnimation(tarWgt,"geometry");
    pMoveAni->setDuration(during);
    pMoveAni->setEasingCurve(QEasingCurve::Linear);

    if(dir)
    {
        pMoveAni->setStartValue(QRect((width()-tarWgt->width())/2, (height()-tarWgt->height())/2-90,0, 0));
        pMoveAni->setEndValue(QRect((width()-tarWgt->width())/2, (height()-tarWgt->height())/2-90, tarWgt->width(), tarWgt->height()));
        pMoveAni->start();
    }
    else
    {
        pMoveAni->setStartValue(QRect((width()-tarWgt->width())/2, (height()-tarWgt->height())/2-90, tarWgt->width(), tarWgt->height()));
        pMoveAni->setEndValue(QRect((width()-tarWgt->width())/2, (height()-tarWgt->height())/2-90,0, 0));
        pMoveAni->start();
    }

    QObject::connect(pMoveAni,&QPropertyAnimation::finished,pMoveAni,&QPropertyAnimation::deleteLater);
}

void Widget::Slot_RecvJsonData(QJsonObject *data)
{
    QJsonDocument JSonDoc;
    JSonDoc.setObject(*data);

    QNetworkReply *reply = g_httpmanage->postinfo(JSonDoc.toJson());

    if(reply->error() != QNetworkReply::NoError)
    {
        showErr(ERR_COLOR, reply->errorString());
    }
    else
    {
        QJsonObject Data = data->value("Data").toObject();
        QJsonObject PersonInfo = Data.value("PersonInfo").toObject();
        ui->errortextbrowser->append(
                    "{\n"
                    "   \"Name\" : \"personListRequest\"\n"
                    "   \"UUID\" : \"" + data->value("UUID").toString() + "\"\n"
                    "   \"ChannelNo\" : " + QString::number(data->value("ChannelNo").toInt()) + "\n"
                    "   \"Session\" : \"" + data->value("Session").toString() + "\"\n"
                    "   \"TimeStamp\" : " + QString::number(data->value("TimeStamp").toInt()) + "\n"
                    "   \"Sign\" : \"" + data->value("Sign").toString() + "\"\n"
                    "   \"Data\" :\n"
                    "   {\n"
                    "       \"Action\" : \"" + Data.value("Action").toString() + "\"\n"
                    "       \"PersonType\" : " + QString::number(Data.value("PersonType").toInt()) + "\n"
                    "       \"PersonInfo\" :\n"
                    "       {\n"
                    "          \"PersonId\" : \"" + PersonInfo.value("PersonId").toString() + "\"\n"
                    "          \"PersonName\" : \"" + PersonInfo.value("PersonName").toString() + "\"\n"
                    "          \"Sex\" : " + QString::number(PersonInfo.value("Sex").toInt()) + "\n"
                    "          \"IDCard\" : \"" + PersonInfo.value("IDCard").toString() + "\"\n"
                    "          \"Nation\" : \"" + PersonInfo.value("Nation").toString() + "\"\n"
                    "          \"Birthday\" : \"" + PersonInfo.value("Birthday").toString() + "\"\n"
                    "          \"Phone\" : \"" + PersonInfo.value("Phone").toString() + "\"\n"
                    "          \"Address\" : \"" + PersonInfo.value("Address").toString() + "\"\n"
                    "          \"LimitTime\" : " + QString::number(PersonInfo.value("LimitTime").toInt()) + "\n");
        if(PersonInfo   .value("LimitTime").toInt() == 1) {
            ui->errortextbrowser->append(
                    "          \"StartTime\" : \"" + PersonInfo.value("StartTime").toString() + "\"\n"
                    "          \"EndTime\" : \"" + PersonInfo.value("EndTime").toString() + "\"\n");
        }
        QJsonObject PersonExtension = PersonInfo.value("PersonExtension").toObject();
        ui->errortextbrowser->append(
                    "          \"Label\" : \"" + PersonInfo.value("Label").toString() + "\"\n"
                    "          \"PersonExtension\" :\n"
                    "          {\n"
                    "              \"PersonCode1\" : \"" + PersonExtension.value("PersonCode1").toString() + "\"\n"
                    "              \"PersonCode2\" : \"" + PersonExtension.value("PersonCode2").toString() + "\"\n"
                    "              \"PersonCode3\" : \"" + PersonExtension.value("PersonCode3").toString() + "\"\n"
                    "              \"PersonReserveName\" : \"" + PersonExtension.value("PersonReserveName").toString() + "\"\n"
                    "              \"PersonParam1\" : " + QString::number(PersonExtension.value("PersonParam1").toInt()) + "\n"
                    "              \"PersonParam2\" : " + QString::number(PersonExtension.value("PersonParam2").toInt()) + "\n"
                    "              \"PersonParam3\" : " + QString::number(PersonExtension.value("PersonParam3").toInt()) + "\n"
                    "              \"PersonParam4\" : " + QString::number(PersonExtension.value("PersonParam4").toInt()) + "\n"
                    "              \"PersonParam5\" : " + QString::number(PersonExtension.value("PersonParam5").toInt()) + "\n"
                    "              \"PersonData1\" : \"" + PersonExtension.value("PersonData1").toString() + "\"\n"
                    "              \"PersonData2\" : \"" + PersonExtension.value("PersonData2").toString() + "\"\n"
                    "              \"PersonData3\" : \"" + PersonExtension.value("PersonData3").toString() + "\"\n"
                    "              \"PersonData4\" : \"" + PersonExtension.value("PersonData4").toString() + "\"\n"
                    "              \"PersonData5\" : \"" + PersonExtension.value("PersonData5").toString() + "\"\n"
                    "          }\n"
                    "          \"PersonPhoto\" : \"\"\n"
                    "      }\n"
                    "}\n");
        showErr(NO_COLOR," :send success.");
    }
    delete data;
}

void Widget::Slot_PersonMangeHide(QWidget *widget,int time,bool tmp)
{
    PersonManageTemp = true;
    MoveAni(widget,time,tmp);
}
