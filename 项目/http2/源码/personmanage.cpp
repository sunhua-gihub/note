#include "personmanage.h"
#include "ui_personmanage.h"

#include <QPropertyAnimation>
#include <time.h>
#include "filechoose.h"
#include <QBuffer>

PersonManage* PersonManage::m_PersonManage = NULL;
PersonManage::PersonManage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonManage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
//    setStyleSheet("background-color: rgb(255, 255, 255);");

    ui->sexbox->setProperty("bcolor","black");
    ui->typebox->setProperty("bcolor","black");
    ui->limitTimebox->setProperty("bcolor","black");

    sexbox.addButton(ui->manrbtn,1);
    sexbox.addButton(ui->womanrbtn,2);
    ui->manrbtn->setChecked(true);

    typebox.addButton(ui->whiterbtn,2);
    typebox.addButton(ui->blackrbtn,1);
    typebox.addButton(ui->viprbtn,3);
    ui->whiterbtn->setChecked(true);

    timebox.addButton(ui->foreverrbtn,0);
    timebox.addButton(ui->deadlinerbtn,1);
    ui->foreverrbtn->setChecked(true);

    mtype = PERSONMANAGETYPE::ADDPERSON;
}

PersonManage::~PersonManage()
{
    delete ui;
}

PersonManage* PersonManage::instance()
{
    if(m_PersonManage == NULL) {
        m_PersonManage = new PersonManage();
    }

    return m_PersonManage;
}

void PersonManage::on_confirmbtn_clicked()
{
    time_t t;
    t = time(NULL);
    int TimeStamp = time(&t);


    QString imginfo = getBase64(ui->PersonManage_picEdit->text());
    if(mtype == PERSONMANAGETYPE::ADDPERSON){
        if(imginfo.isEmpty())
        {
            emit sendErr(ERR_COLOR,"[error]: get pic error.");
            return ;
        }
    }
    QJsonObject *data = new QJsonObject;
    QJsonObject obj1,obj2,obj3;

    QString Session = ui->PersonManage_sessionEdit->text();
    QStringList Sessions = Session.split("_");
    QString UUID = nullptr;
    if(Sessions.size() != 0) {
        UUID = Sessions.at(0);
    }

    data->insert("Name","personListRequest");
    data->insert("UUID",UUID);
    data->insert("ChannelNo",ui->PersonManage_channelEdit->text().toInt());
    data->insert("Session",ui->PersonManage_sessionEdit->text());
    data->insert("TimeStamp",TimeStamp);
    data->insert("Sign","");
    if(mtype == PERSONMANAGETYPE::ADDPERSON) {
        obj1.insert("Action","addPerson");
    }else if(mtype == PERSONMANAGETYPE::EDITPERSON) {
        obj1.insert("Action","editPerson");
    }
    obj1.insert("PersonType",typebox.checkedId());
    obj2.insert("PersonId",ui->PersonManage_idEdit->text());
    obj2.insert("PersonName",ui->PersonManage_nameEdit->text());
    obj2.insert("Sex",sexbox.checkedId());
    obj2.insert("IDCard",ui->PersonManage_idcardEdit->text());
    obj2.insert("Nation",ui->PersonManage_nationEdit->text());
    obj2.insert("Birthday",ui->PersonManage_birthdayEdit->text());
    obj2.insert("Phone",ui->PersonManage_phoneEdit->text());
    obj2.insert("Address",ui->PersonManage_addressEdit->text());
    obj2.insert("LimitTime",timebox.checkedId());
    if(timebox.checkedId() == 1) {
        obj2.insert("StartTime",ui->PersonManage_startTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss"));
        obj2.insert("EndTime",ui->PersonManage_endTimeEdit->dateTime().toString("yyyy-MM-dd HH:mm:ss"));
    }
    obj2.insert("Label",ui->PersonManage_labelEdit->text());
    obj3.insert("PersonCode1",ui->PersonManage_Code1Edit->text());
    obj3.insert("PersonCode2",ui->PersonManage_Code2Edit->text());
    obj3.insert("PersonCode3",ui->PersonManage_Code3Edit->text());
    obj3.insert("PersonReserveName",ui->PersonManage_formernameEdit->text());
    obj3.insert("PersonParam1",ui->PersonManage_Param1Edit->text().toInt());
    obj3.insert("PersonParam2",ui->PersonManage_Param2Edit->text().toInt());
    obj3.insert("PersonParam3",ui->PersonManage_Param3Edit->text().toInt());
    obj3.insert("PersonParam4",ui->PersonManage_Param4Edit->text().toInt());
    obj3.insert("PersonParam5",ui->PersonManage_Param5Edit->text().toInt());
    obj3.insert("PersonData1",ui->PersonManage_data1Edit->text());
    obj3.insert("PersonData2",ui->PersonManage_data2Edit->text());
    obj3.insert("PersonData3",ui->PersonManage_data3Edit->text());
    obj3.insert("PersonData4",ui->PersonManage_data4Edit->text());
    obj3.insert("PersonData5",ui->PersonManage_data5Edit->text());
    obj2.insert("PersonPhoto",imginfo);
    obj2.insert("PersonExtension",obj3);
    obj1.insert("PersonInfo",obj2);
    data->insert("Data",obj1);

    emit sendJsonData(data);
}

void PersonManage::MoveAni()
{
    QPropertyAnimation * pMoveAni = new QPropertyAnimation(this,"geometry");
    pMoveAni->setDuration(2000);
    pMoveAni->setEasingCurve(QEasingCurve::Linear);

    pMoveAni->setStartValue(QRect(geometry().x(), geometry().y(), 0, height()));
    pMoveAni->setEndValue(QRect(geometry().x(), geometry().y(),width(), height()));
    pMoveAni->start();

    QObject::connect(pMoveAni,&QPropertyAnimation::finished,pMoveAni,&QPropertyAnimation::deleteLater);
}

void PersonManage::on_cannelbtn_clicked()
{
    emit timetohide(this,500,false);
}

void PersonManage::on_getpicbtn_clicked()
{
    connect(filechoose::instance(),&filechoose::Signal_Path,this,[ = ](QString path){
        ui->PersonManage_picEdit->setText(path);
        filechoose *send = (filechoose *)sender();
        if(send)
            send->hide();
    });
    filechoose::instance()->exec();
}

QString PersonManage::getBase64(QString path)
{
    if(path.isEmpty()) {
        emit sendErr(WARN_COLOR,"[warning]: path is null.");
        return NULL;
    }
    QImage image(path);
    if(image.isNull()) {
        emit sendErr(ERR_COLOR,"[error]: pic is null.");
        return NULL;
    }
    QByteArray ba;
    QBuffer buf(&ba);

    image.save(&buf,"JPG");
    buf.close();
    return ba.toBase64();
}
