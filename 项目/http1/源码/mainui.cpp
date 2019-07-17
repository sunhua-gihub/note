#include "mainui.h"
#include "ui_mainui.h"

#include <QDesktopWidget>
#include <QMouseEvent>
#include <QDateTime>
#include <QFileDialog>
#include <QSettings>

#define NO_DEV
mainui::mainui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainui)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    setinit();
    hidesomething();
}

mainui::~mainui()
{
    delete ui;
}

void mainui::mousePressEvent(QMouseEvent *e)
{
    if (!max && e->button() == Qt::LeftButton) {
        mMousePoint = e->globalPos() - this->pos();
        e->accept();
        mDragWindow = true;
    }
}

void mainui::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    mDragWindow = false;
}

void mainui::mouseMoveEvent(QMouseEvent *e)
{
    if (mDragWindow && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mMousePoint);
        ChildWidget->getParentGeometry(this,LinkageWidget::Right);
        e->accept();
    }
}

void mainui::closeEvent(QCloseEvent *)
{
    if(ChildWidget != NULL) {
        ChildWidget->close();
    }
}

//void mainui::focusInEvent(QFocusEvent *event)
//{
//    Q_UNUSED(event);
//    qDebug()<<"focusIn";
//    if(!ChildWidget->isHidden()){
//        ChildWidget->setWindowFlags(Qt::WindowStaysOnTopHint |Qt::FramelessWindowHint|Qt::Tool);
//        ChildWidget->getParentGeometry(this);
//        ChildWidget->show();
//    }
//}

//void mainui::focusOutEvent(QFocusEvent *event)
//{
//    Q_UNUSED(event);
//    qDebug()<<"focusOut";
//    if(!ChildWidget->isHidden()) {
//        ChildWidget->setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint|Qt::Tool);
//        ChildWidget->show();
//    }
//}

void mainui::changeEvent(QEvent *e)
{
    switch(e->type()) {
        case QEvent::WindowStateChange:
            if(m_ChildWidget_isShow) {
                if(windowState() == Qt::WindowMinimized) {
                    ChildWidget->hide();
                }
                if(windowState() == Qt::WindowNoState)
                {
                    ChildWidget->getParentGeometry(this);
                    ChildWidget->showNormal();
                }
            }
//            qDebug()<<windowState()<<ChildWidget->windowState();
        break;
    default:
        break;
    }
}

void mainui::hidesomething()
{
    ui->uuidEdit2->hide();
    ui->uuidEdit3->hide();
    ui->uuidEdit4->hide();
    ui->uuidlab2->hide();
    ui->uuidlab3->hide();
    ui->uuidlab4->hide();

    ui->usedlab2->hide();
    ui->usedlab3->hide();
    ui->usedlab4->hide();
    ui->usedEdit2->hide();
    ui->usedEdit3->hide();
    ui->usedEdit4->hide();

    ui->passwordEdit2->hide();
    ui->passwordEdit3->hide();
    ui->passwordEdit4->hide();
    ui->passwordlab2->hide();
    ui->passwordlab3->hide();
    ui->passwordlab4->hide();

    ui->nameEdit4->hide();
    ui->namelab4->hide();
    ui->idEdit4->hide();
    ui->idlab4->hide();
}

void mainui::setinit()
{
    max = false;
    mDragWindow = false;
    location = geometry();

    //左侧导航信息
    DevList<<"查看设备"<<"添加设备"<<"修改设备"<<"删除设备";
    ListList<<"查看名单"<<"添加名单"<<"修改名单"<<"删除名单"<<"清空名单"<<"查找名单";
    PlanList<<"查看预案"<<"修改预案";
    SignList<<"查看未签到"<<"查看已签到";
    StatisList<<"查看统计";
    SettingList<<"设置时间"<<"设置网络"<<"设置阈值和缓冲";
    OtherList<<"图片检索"<<"图片比对"<<"图片检测";
    RecvList<<"清空";

    ui->urlEdit->setText("http://192.168.1.41:9999");

    ui->dateEdit4->setDate(QDate::currentDate());
    ui->dateEdit5->setDate(QDate::currentDate());
    ui->dateTimeEdit6->setDateTime(QDateTime::currentDateTime());

    setProperty("form",true);

    ui->widgetTitle->setProperty("form", "title");
    ui->widgetLeft->setProperty("nav", "left");
    ui->widgetTop->setProperty("nav", "top");
    ui->widget->setProperty("nav","no");

    ui->text->setText("宿舍管理平台");
    ui->text->setFont(QFont("Microsoft Yahei", 20));
    setWindowTitle(ui->text->text());

     PlanNumberList << "P1"<< "P2"<< "P3"<< "P4"<< "P5"<< "P6"<< "P7"<< "P8";
//    m_textBrowser = new QTextBrowser(ui->widget);
//    m_textBrowser->setGeometry(10,10,680,500);

    ChildWidget = LinkageWidget::instance();
    ChildWidget->setTitleText("接收");
    ChildWidget->setFixedWidth(500);
    m_ChildWidget_isShow = false;
    connect(ChildWidget,SIGNAL(usedhide()),this,SLOT(ChildWidgethide()));
    //设置顶部导航按钮
    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns) {
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    }
    ui->DevManage->click();

    //设置左侧导航按钮
    QList<QPushButton *> lbtns = ui->widgetLeft->findChildren<QPushButton *>();
    foreach (QPushButton *btn, lbtns) {
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(btnClick()));
    }
    ui->P1->click();

    m_btngroup1 = new QButtonGroup(this);
    m_btngroup1->addButton(ui->indoorrbtn1,0);
    m_btngroup1->addButton(ui->outdoorrbtn1,1);

    ui->indoorrbtn1->setChecked(true);

    //名单库选项
    m_btngroup2 = new QButtonGroup(this);
    m_btngroup2->addButton(ui->whiterbtn2,0);
    m_btngroup2->addButton(ui->blackrbtn2,1);
    m_btngroup2->addButton(ui->viprbtn2,2);

    ui->whiterbtn2->setChecked(true);

    m_btngroup4 = new QButtonGroup(this);
    m_btngroup4->addButton(ui->whiterbtn4,0);
    m_btngroup4->addButton(ui->blackrbtn4,1);
    m_btngroup4->addButton(ui->viprbtn4,2);

    ui->whiterbtn4->setChecked(true);

    m_btngroup5 = new QButtonGroup(this);
    m_btngroup5->addButton(ui->whiterbtn5,0);
    m_btngroup5->addButton(ui->blackrbtn5,1);
    m_btngroup5->addButton(ui->viprbtn5,2);

    ui->whiterbtn5->setChecked(true);

    m_pbtngroup4 = new QButtonGroup(this);
    m_pbtngroup4->addButton(ui->prbtn0,0);
    m_pbtngroup4->addButton(ui->prbtn1,1);
    m_pbtngroup4->addButton(ui->prbtn2,2);
    m_pbtngroup4->addButton(ui->prbtn3,3);
    m_pbtngroup4->addButton(ui->prbtn4,4);
    m_pbtngroup4->addButton(ui->prbtn5,5);
    m_pbtngroup4->addButton(ui->prbtn6,6);
    m_pbtngroup4->addButton(ui->prbtn7,7);

    ui->prbtn0->setChecked(true);

    m_model = new QStandardItemModel(this);

    QStringList labels;
    labels<<"文件名"<<"大小(KB)"<<"分辨率"<<"格式";
    m_model->setHorizontalHeaderLabels(labels);

    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setDefaultSectionSize(20);

    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));

    connect(ui->send1,SIGNAL(clicked()),this,SLOT(send_clicked()));
    connect(ui->send2,SIGNAL(clicked()),this,SLOT(send_clicked()));
    connect(ui->send3,SIGNAL(clicked()),this,SLOT(send_clicked()));
    connect(ui->send4,SIGNAL(clicked()),this,SLOT(send_clicked()));
    connect(ui->send5,SIGNAL(clicked()),this,SLOT(send_clicked()));
    connect(ui->send6,SIGNAL(clicked()),this,SLOT(send_clicked()));

    connect(ui->P1,SIGNAL(clicked()),this,SLOT(clear()));

    connect(ui->filebtn2,SIGNAL(clicked()),this,SLOT(filebtn_clicked()));
}

void mainui::on_btnMenu_Close_clicked()
{
    close();
}

void mainui::clear()
{
    if(ui->P1->text() == "清空") {
        ui->errtextBrowser->clear();
    }
}

void mainui::ChildWidgethide()
{
    m_ChildWidget_isShow = false;
}
void mainui::on_btnMenu_Max_clicked()
{
#if 0
    if (max) {
        setGeometry(location);
        mDragWindow = true;
    } else {
        location = geometry();
        setGeometry(qApp->desktop()->availableGeometry());
        mDragWindow = false;
    }

    max = !max;
#endif
}

void mainui::on_btnMenu_Min_clicked()
{
    showMinimized();
    ChildWidget->hide();
}

void mainui::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    if(name == "接收") {
        ChildWidget->getParentGeometry(this,LinkageWidget::Right);
        ChildWidget->show();
        m_ChildWidget_isShow = true;
        b->setChecked(false);
        return;
    }
    int curpage = 0;
    QList<QToolButton *> tbtns = ui->widgetTop->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns) {
        if (btn == b) {
            mtopbtn = curpage;
            ui->stackedWidget->setCurrentIndex(curpage);
            btn->setChecked(true);
        } else {
            btn->setChecked(false);
        }
        curpage++;
    }

    int i=0;
    QList<QPushButton *> lbtns = ui->widgetLeft->findChildren<QPushButton *>();
    if(name == "设备")
    {
        foreach (QPushButton *btn, lbtns) {
            if(i < DevList.length())
            {
                btn->setText(DevList.at(i));
                i++;
            }
            else
                btn->setText("");
        }
    }
    else if(name == "名单")
    {
        foreach (QPushButton *btn, lbtns) {
            if(i < ListList.length())
            {
                btn->setText(ListList.at(i));
                i++;
            }
            else
                btn->setText("");
        }
    }
    else if(name == "预案")
    {
        foreach (QPushButton *btn, lbtns) {
            if(i < PlanList.length())
            {
                btn->setText(PlanList.at(i));
                i++;
            }
            else
                btn->setText("");
        }
    }
    else if(name == "签到")
    {
        foreach (QPushButton *btn, lbtns) {
            if(i < SignList.length())
            {
                btn->setText(SignList.at(i));
                i++;
            }
            else
                btn->setText("");
        }
    }
    else if(name == "统计")
    {
        foreach (QPushButton *btn, lbtns) {
            if(i < StatisList.length())
            {
                btn->setText(StatisList.at(i));
                i++;
            }
            else
                btn->setText("");
        }
    }
    else if(name == "设置")
    {
        foreach (QPushButton *btn, lbtns) {
            if(i < SettingList.length())
            {
                btn->setText(SettingList.at(i));
                i++;
            }
            else
                btn->setText("");
        }
    }
    else if(name == "其他")
    {
        foreach (QPushButton *btn, lbtns) {
            if(i < OtherList.length())
            {
                btn->setText(OtherList.at(i));
                i++;
            }
            else
                btn->setText("");
        }
    }

    if(ui->P1->text() != "清空") {
        ui->P1->click();
    }
}

void mainui::btnClick()
{
    QPushButton *b = (QPushButton *)sender();

    if(b->text() == "清空") {
        return ;
    }
    int i = 0;
    QList<QPushButton *> lbtns = ui->widgetLeft->findChildren<QPushButton *>();
    foreach (QPushButton *btn, lbtns) {
        if (btn == b) {
            btn->setChecked(true);
            mleftbtn = i;
        } else {
            btn->setChecked(false);
        }
        i++;
    }

}

void mainui::on_connect_clicked()
{
    m_url = ui->urlEdit->text();
    if(m_url.isEmpty())
    {
        showErr(ERR_COLOR,"[error]: url is null.");
        return;
    }
    m_request.setUrl(m_url);
    m_request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    m_request.setRawHeader("Connection","Keep-Alive");

    showErr(NO_COLOR,"[info]: url set success.");
}

QString mainui::getBase64(QString path)
{
    showErr(NO_COLOR,path);
    if(path.isEmpty())
    {
        showErr(WARN_COLOR,"[warning]: pic is null,no picture data to send.");
        return NULL;
    }
    QImage image(path);
    QByteArray ba;
    QBuffer buf(&ba);

    image.save(&buf,"JPG");
    buf.close();
    return ba.toBase64();
}

void mainui::showErr(int type,QString err)
{
    switch(type)
    {
        case ERR_COLOR:
            ui->errtextBrowser->setTextColor(QColor(255,0,0));
        break;
        case WARN_COLOR:
            ui->errtextBrowser->setTextColor(QColor(255,255,0));
        break;
        default:
            ui->errtextBrowser->setTextColor(QColor(255,255,255));
        break;
    }
    ui->errtextBrowser->append(err);
}

bool mainui::isJPG(QByteArray a)
{
    QString b = a.toHex();
    if(b == "ffd8ffe000104a4649460001")
        return true;

    return false;
}

void mainui::setModelDate(int row, QString filename)
{
    bool temp = false;

    QFile file(filename);
    file.open(QFile::ReadOnly);
    QStringList list = filename.split("/");
    QString name = list.takeLast();
    //文件名格式检测
    QStringList namelist = name.split("_");
    if(namelist.length() < 2)
    {
        temp = true;
    }

    QImage image(filename);
    QString resolution = QString::number(image.width())+"*"+QString::number(image.height());
    //文件格式检测
    QString format;
    if(isJPG(file.read(12)))
    {
        format.append(".jpg");
    }
    else
    {
        format.append("未知");
        temp = true;
    }
    //分辨率 和 图片宽高检测
    if(image.width() < 100 || image.width() >640 || image.height() < 100  || image.height() > 640
           || file.size() < 5 * 1024 || file.size() > 200 * 1024)
    {
        temp = true;
    }

    QStringList info;
    info<<name<<QString::number(file.size()/1024.0)<<resolution<<format;

    for(int i=0;i<4;i++)
    {
        QStandardItem *item = new QStandardItem();
        item->setText(info.at(i));
        item->setTextAlignment(Qt::AlignCenter);
        if(temp)
        {
            item->setForeground(QBrush(QColor(255,0,0)));
        }
        else
        {
            item->setForeground(QBrush(QColor(255,255,255)));
        }
        m_model->setItem(row,i,item);
    }
}

void mainui::replyFinished(QNetworkReply *reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        ChildWidget->appendTextBrowserInfo(reply->errorString());
        //m_textBrowser->append(reply->errorString());
    }
    else
    {
        QString all = reply->readAll();
        ChildWidget->appendTextBrowserInfo(all);
        //m_textBrowser->append(all);
        reply->deleteLater();
    }
}

void mainui::send_clicked()
{
    if(m_url.isEmpty())
    {
        showErr(ERR_COLOR,"[error]: url is null.");
        return;
    }

    QJsonObject Jdata;
    QJsonObject obj1;
    bool temp = true;

    switch(mtopbtn)
    {
        case LIST_MODULE:
        {
/******************************模块1-start**************************************/
            QString uuid = ui->uuidEdit2->text();
            QString used = ui->usedEdit2->text();
            QString password = ui->passwordEdit2->text();

            if(uuid.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: uuid is null.");
            }
            if(used.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: usename is null.");
            }
            if(password.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: password is null.");
            }

/******************************模块1-end**************************************/
/******************************加密-start**************************************/
            QDateTime time = QDateTime::currentDateTime();
            QString curtime = time.toString("yyyy-MM-dd hh:mm:ss");
            QString Curtime = time.toString("yyyyMMddhhmmss");

            QString sign;
            sign.append(uuid).append(":").append(used).append(":").append(password).append(":").append(Curtime);

            QCryptographicHash md(QCryptographicHash::Md5);
            QByteArray a;
            md.addData(a.append(sign));
            sign = md.result().toHex();

/******************************加密-end**************************************/
            int mode = m_btngroup2->checkedId();

            QString PersonName = ui->nameEdit2->text();
            QString PersonID = ui->idEdit2->text();
            QString Pic = ui->filenameEdit2->text();

/******************************Json组装-start********************************/
            QString img;

            Jdata.insert("Name","WBListInfoREQ");
            Jdata.insert("TimeStamp",curtime);
            Jdata.insert("Sign",sign);
            Jdata.insert("Mode",mode);

            if(mleftbtn == BROWSE_LIST){

            }
            else{
                obj1.insert("PersonName",PersonName);
                obj1.insert("PersonID",PersonID);
                QStringList list = Pic.split("/");
                QString Picname = list.takeLast();
                obj1.insert("Pic",Picname);
            }

            switch(mleftbtn){
                case BROWSE_LIST:
                    Jdata.insert("Action","GetList");
                    break;
                case ADD_LIST:
                    if(PersonName.isEmpty()){
                        showErr(ERR_COLOR,"name is null.");
                        return;
                    }
                    if(PersonID.isEmpty()){
                        showErr(ERR_COLOR,"id is null.");
                        return;
                    }
                    Jdata.insert("Action","AddOnePic");
                    img = getBase64(m_filename);
                    if(img.isEmpty()){
                        showErr(ERR_COLOR,"pic data is null.");
                        return;
                    }
                    obj1.insert("Img",img);
                    Jdata.insert("AddOnePic",obj1);
                    break;
                case MODIFY_LIST:
                    if(PersonName.isEmpty()){
                        showErr(ERR_COLOR,"name is null.");
                        return;
                    }
                    if(PersonID.isEmpty()){
                        showErr(ERR_COLOR,"id is null.");
                        return;
                    }
                    Jdata.insert("Action","ModOnePic");
                    img = getBase64(m_filename);
                    if(img.isEmpty()){
                        showErr(NO_COLOR,"no picture data,but sent.");
                    }
                    obj1.insert("Img",img);
                    Jdata.insert("ModOnePic",obj1);
                    break;
                case DEL_LIST:
                    if(PersonName.isEmpty() && PersonID.isEmpty()){
                        showErr(ERR_COLOR,"choose one to wirte,name or id.");
                        return;
                    }
                    Jdata.insert("Action","DeleteOnePic");
                    Jdata.insert("DeleteOnePic",obj1);
                    break;
                case CLEAR_LIST:
                    Jdata.insert("Action","ClearList");
                    break;
                case FIND_LIST:
                    if(PersonID.isEmpty()){
                        showErr(ERR_COLOR,"id is null.");
                        return;
                    }
                    Jdata.insert("Action","GetOnePic");
                    Jdata.insert("GetOnePic",obj1);
                    break;
                default:
                    temp = false;
                    break;
            }
/******************************Json组装-end********************************/
        }
            break;
        case SIGN_MODULE:
        {
/******************************模块1-start**************************************/

            QString uuid = ui->uuidEdit4->text();
            QString used = ui->usedEdit4->text();
            QString password = ui->passwordEdit4->text();
#ifndef NO_DEV
            if(uuid.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: uuid is null.");
            }
            if(used.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: usename is null.");
            }
            if(password.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: password is null.");
            }
#endif
/******************************模块1-end**************************************/
/******************************加密-start**************************************/
            QDateTime time = QDateTime::currentDateTime();
            QString curtime = time.toString("yyyy-MM-dd hh:mm:ss");
            QString Curtime = time.toString("yyyyMMddhhmmss");

            QString sign;
            sign.append(uuid).append(":").append(used).append(":").append(password).append(":").append(Curtime);

            QCryptographicHash md(QCryptographicHash::Md5);
            QByteArray a;
            md.addData(a.append(sign));
            sign = md.result().toHex();

/******************************加密-end**************************************/
            QString Data = ui->dateEdit4->text();
            int plan = m_pbtngroup4->checkedId();
            int mode = m_btngroup4->checkedId();

/******************************Json组装-start********************************/
            Jdata.insert("Name","WBListInfoREQ");
            Jdata.insert("TimeStamp",curtime);
            Jdata.insert("Sign",sign);
            Jdata.insert("Mode",mode);

            obj1.insert("Date",Data);
            obj1.insert("Plan",plan);
            switch(mleftbtn){
                case GET_SIGNLIST:
                    Jdata.insert("Action","GetSignList");
                    Jdata.insert("GetSignList",obj1);
                    break;
                case GET_NOSIGNLIST:
                    Jdata.insert("Action","GetNoSignList");
                    Jdata.insert("GetNoSignList",obj1);
                    break;
                default:
                    temp = false;
                    break;
            }

/******************************Json组装-end********************************/
        }
            break;
        case PLAN_MODULE:
        {
/******************************模块1-start**************************************/
            QString uuid = ui->uuidEdit3->text();
            QString used = ui->usedEdit3->text();
            QString password = ui->passwordEdit3->text();
#ifndef NO_DEV
            if(uuid.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: uuid is null.");
            }
            if(used.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: usename is null.");
            }
            if(password.isEmpty())
            {
                showErr(WARN_COLOR,"[warning]: password is null.");
            }
#endif
/******************************模块1-end**************************************/
/******************************加密-start**************************************/
            QDateTime time = QDateTime::currentDateTime();
            QString curtime = time.toString("yyyy-MM-dd hh:mm:ss");
            QString Curtime = time.toString("yyyyMMddhhmmss");

            QString sign;
            sign.append(uuid).append(":").append(used).append(":").append(password).append(":").append(Curtime);

            QCryptographicHash md(QCryptographicHash::Md5);
            QByteArray a;
            md.addData(a.append(sign));
            sign = md.result().toHex();

/******************************加密-end**************************************/
/******************************Json组装-start********************************/
            Jdata.insert("Name","WBListInfoREQ");
            Jdata.insert("TimeStamp",curtime);
            Jdata.insert("Sign",sign);

            switch(mleftbtn){
                case GET_PLAN:
                    Jdata.insert("Action","GetPlan");
                    break;
                case MOD_PLAN:
                {
//                    QJsonObject plan;
//                    QJsonArray array;
//                    Jdata.insert("Action","ModPlan");
//                    Jdata.insert("plan",plan);
                    assemble_json(&Jdata);
                }
                    break;
                default:
                    temp = false;
                    break;
            }

/******************************Json组装-end********************************/
        }
            break;
        case DEV_MODULE:
        {
            QDateTime time = QDateTime::currentDateTime();
            QString curtime = time.toString("yyyy-MM-dd hh:mm:ss");
            QString uuid = ui->uuidEdit1->text();
            QString name = ui->devEdit1->text();
            QString usedname = ui->usedEdit1->text();
            QString password = ui->passwordEdit1->text();
            QString ip = ui->ipEdit1->text();
            int post = ui->postEdit1->text().toInt();
            int type = m_btngroup1->checkedId();
/******************************Json组装-start********************************/
            Jdata.insert("Name","WBListInfoREQ");
            Jdata.insert("TimeStamp",curtime);

            switch(mleftbtn) {
                case BROWSE_DEV:
                    Jdata.insert("Action","GetDev");
                    break;
                case ADD_DEV:
                    Jdata.insert("Action","AddDev");
                    obj1.insert("DevName",name);
                    obj1.insert("UsedName",usedname);
                    obj1.insert("Password",password);
                    obj1.insert("IP",ip);
                    obj1.insert("Post",post);
                    obj1.insert("Type",type);
                    Jdata.insert("AddDev",obj1);
                    break;
                case MOD_DEV:
                    if(uuid.isEmpty()) {
                        showErr(ERR_COLOR,"uuid is null.");
                        return;
                    }
                    Jdata.insert("Action","ModDev");
                    obj1.insert("UUID",uuid);
                    obj1.insert("DevName",name);
                    obj1.insert("UsedName",usedname);
                    obj1.insert("Password",password);
                    obj1.insert("IP",ip);
                    obj1.insert("Post",post);
                    obj1.insert("Type",type);
                    Jdata.insert("ModDev",obj1);
                    break;
                case DEL_DEV:
                    if(uuid.isEmpty()) {
                        showErr(ERR_COLOR,"uuid is null.");
                        return;
                    }
                    Jdata.insert("Action","DelDev");
                    Jdata.insert("UUID",uuid);
                    break;
                default:
                    temp = false;
                    break;
            }
/******************************Json组装-end********************************/
        }
            break;
        case STATIS_MODULE:
        {
            QDateTime time = QDateTime::currentDateTime();
            QString curtime = time.toString("yyyy-MM-dd hh:mm:ss");

            QString date = ui->dateEdit5->text();
            date.remove("-");

            int mode = m_btngroup5->checkedId();
/******************************Json组装-start********************************/
            Jdata.insert("Name","WBListInfoREQ");
            Jdata.insert("TimeStamp",curtime);

            switch(mleftbtn) {
                case GET_STATIS:
                    Jdata.insert("Action","GetStatis");
                    Jdata.insert("Mode",mode);
                    obj1.insert("Date",date);
                    Jdata.insert("GetStatis",obj1);
                    break;
                default:
                    temp = false;
                    break;
            }
/******************************Json组装-end********************************/
        }
            break;
        case SETTING_MODULE:
        {
            QDateTime time = QDateTime::currentDateTime();
            QString curtime = time.toString("yyyy-MM-dd hh:mm:ss");

            QString date = ui->dateTimeEdit6->text();
            QString ip = ui->ipEdit6->text();
            QString gateway = ui->gatewayEdit6->text();
            QString mask = ui->maskEdit6->text();


            int Threshold = ui->thresholdEdit6->text().toInt();
            int Buff = ui->buffEdit6->text().toInt();
/******************************Json组装-start********************************/
            Jdata.insert("Name","WBListInfoREQ");
            Jdata.insert("TimeStamp",curtime);

            switch(mleftbtn) {
                case SET_DATETIME:
                    Jdata.insert("Action","SetTime");
                    Jdata.insert("Date",date);
                    break;
                case SET_NETWORK:
                    if(ip.isEmpty() || gateway.isEmpty() || mask.isEmpty() ){
                        showErr(ERR_COLOR,"ip,gateway and mask must be write.");
                        return;
                    }
                    Jdata.insert("Action","SetNetwork");
                    obj1.insert("IP",ip);
                    obj1.insert("Gateway",gateway);
                    obj1.insert("Mask",mask);
                    Jdata.insert("SetNetwork",obj1);
                    break;
                case SET_BUFFANDTHRESHOLD:
                    if(Threshold < 1 || Buff < 1){
                        showErr(ERR_COLOR,"threshold or buff is error,and they all write.");
                        return;
                    }
                    if(Threshold > 100){
                        showErr(ERR_COLOR,"threshold range is 0~100.");
                        return;
                    }
                    if(Buff > 5000 || (Buff > 0 && Buff < 500)) {
                        showErr(ERR_COLOR,"Buff range is 500~5000.");
                        return;
                    }
                    Jdata.insert("Action","SetBuffandThreshold");
                    obj1.insert("Threshold",Threshold);
                    obj1.insert("Buff",Buff);
                    Jdata.insert("SetBuffandThreshold",obj1);
                    break;
                default:
                    temp = false;
                    break;
            }
/******************************Json组装-end********************************/
        }
            break;
        case OTHER_MODULE:
        {
            QString img;
            QStringList list;
            QDateTime time = QDateTime::currentDateTime();
            QString curtime = time.toString("yyyy-MM-dd hh:mm:ss");

            Jdata.insert("Name","WBListInfoREQ");
            Jdata.insert("TimeStamp",curtime);
            Jdata.insert("Mode",0);
            switch (mleftbtn) {
                case PIC_SERACH:
                    Jdata.insert("Action","DetectOnePic");
                    list = m_formatfilename.split("/");
                    obj1.insert("Pic",list.takeLast());
                    img = getBase64(m_formatfilename);
                    obj1.insert("Img",img);
                    Jdata.insert("DetectOnePic",obj1);
                    break;
                case PIC_COMPARE:
                    Jdata.insert("Action","DetectTwoPic");
                    list = m_filename.split("/");
                    obj1.insert("Pic1",list.takeLast());
                    list.clear();
                    list = m_filenameT.split("/");
                    obj1.insert("Pic2",list.takeLast());
                    img = getBase64(m_filename);
                    obj1.insert("Img1",img);
                    img = getBase64(m_filenameT);
                    obj1.insert("Img2",img);
                    Jdata.insert("DetectTwoPic",obj1);
                    break;
                default:
                    break;
            }
            break;
        }

        default:
            temp = false;
            break;
    }

    if(temp) {
        QJsonDocument JSonDoc;
        JSonDoc.setObject(Jdata);

        manager->post(m_request,JSonDoc.toJson());

        showErr(NO_COLOR,"[info]: send success.");
    }else{
        showErr(WARN_COLOR,"[warning]: nothing to send.");
    }
}

void mainui::filebtn_clicked()
{
    QPushButton *btn = (QPushButton *)sender();

//    QSettings settings(QCoreApplication::applicationDirPath()+"/programmer.ini",QSettings::IniFormat);
//    QString path = settings.value("filepath").toString();
//    if(path.isEmpty()) {
//        path = ".";
//    }
//    showErr(NO_COLOR,QCoreApplication::applicationDirPath());
    if(btn->objectName() == "filebtn2")
    {
        m_filename = QFileDialog::getOpenFileName(this,QString(),QString(),"图片(*.jpg *.jpeg)");
        if(m_filename.isEmpty())
        {
            showErr(WARN_COLOR,"[warning]：pictures is null.");
            return;
        }

//        QStringList list = m_filename.split("/");
//        QString filepath = m_filename.mid(0,m_filename.length()-list.last().length());

//        settings.setValue("filepath",filepath);
        ui->filenameEdit2->setText(m_filename);
    }
}

void mainui::on_filebtn7_clicked()
{
    if(m_url.isNull()) {
        showErr(ERR_COLOR,"url is null.");
        return ;
    }

//    QSettings settings(QCoreApplication::applicationDirPath()+"/programmer.ini",QSettings::IniFormat);
//    QString path = settings.value("filepath").toString();
//    if(path.isEmpty()) {
//        path = ".";
//    }

//    QString filepath;

    if(mleftbtn == PIC_DETECTION) {
        m_formatfilename.clear();
        m_formatfilename = QFileDialog::getOpenFileName(this,QString(),QString(),"图片(*.jpg *.jpeg)");
        if(m_formatfilename.isEmpty())
            return;

//        QStringList list = m_formatfilename.split("/");
//        filepath = m_formatfilename.mid(0,m_formatfilename.length()-list.takeLast().length());

        m_model->removeRows(0,m_model->rowCount());
        setModelDate(0,m_formatfilename);
    }else if (mleftbtn == PIC_SERACH) {
        m_formatfilename.clear();
        m_formatfilename = QFileDialog::getOpenFileName(this,QString(),QString(),"图片(*.jpg *.jpeg)");
        if(m_formatfilename.isEmpty())
            return;

//        QStringList list = m_formatfilename.split("/");
//        filepath = m_formatfilename.mid(0,m_formatfilename.length()-list.takeLast().length());

        send_clicked();
    }else if (mleftbtn == PIC_COMPARE) {
        QStringList list= QFileDialog::getOpenFileNames(this,QString(),QString(),"图片(*.jpg *.jpeg)");
        if(list.size() != 2)
        {
            showErr(WARN_COLOR,"[warning]： the number of pictures is not two.");
            return;
        }
        m_filename = list.at(0);
        m_filenameT = list.at(1);

//        list.clear();
//        list = m_filenameT.split("/");
//        filepath = m_filenameT.mid(0,m_filenameT.length()-list.takeLast().length());

        send_clicked();
    }else {
        showErr(WARN_COLOR,"nothing to do.");
    }
//    if(!filepath.isEmpty())
//        settings.setValue("filepath",filepath);
}

void mainui::on_foldbtn7_clicked()
{
    if(m_url.isEmpty()) {
        showErr(ERR_COLOR,"url is null.");
        return;
    }
//    QSettings settings(QCoreApplication::applicationDirPath()+"/programmer.ini",QSettings::IniFormat);
//    QString path = settings.value("filepath").toString();
//    if(path.isEmpty()) {
//        path = ".";
//    }

    if(mleftbtn == PIC_DETECTION) {
        QString folder = QFileDialog::getExistingDirectory(this,QString(),QString(),QFileDialog::ShowDirsOnly);

//        settings.setValue("filepath",folder);

        QDir *dir =  new QDir(folder);
        dir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir->setSorting(QDir::Size | QDir::Reversed);
        QFileInfoList list = dir->entryInfoList();

        if(list.isEmpty())
        {
            showErr(ERR_COLOR,"[error]: dir is null.");
        }
        m_model->removeRows(0,m_model->rowCount());

        for(int i=0;i<list.length();i++) {
            setModelDate(i,list.at(i).filePath());
        }
    }

}

void mainui::on_getfile3_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,QString(),QString(),"文本文件(*.txt)");
    if(filename.isEmpty())
    {
        showErr(WARN_COLOR,"[warning]：filename is null.");
        return;
    }

    QFile file(filename);
    file.open(QFile::ReadOnly);
    QTextStream in(&file);
    QStringList lines;
    QStringList tempList;
    QString oneline;
    while(!in.atEnd()) {
        lines.append(in.readLine());
    }
    file.close();
    if(lines.length() != 16) {
        showErr(ERR_COLOR,"[error]: data line error!");
        return ;
    }

    PlanDataList.clear();
    for(int i=0;i<16;i+=2) {
        if(lines.at(i) != PlanNumberList.at(i/2)) {
            showErr(ERR_COLOR,"[error]: data format error!");
            return ;
        }
        oneline = lines.at(i+1);
        tempList = oneline.split("\t",QString::SkipEmptyParts);
        if(detection_data(tempList)) {
            PlanDataList.append(tempList);
        }else {
            PlanDataList.clear();
            return;
        }
    }
    showErr(NO_COLOR,"[info]: data detection success!");
}

bool mainui::detection_data(QStringList list)
{
    if(list.length() != 8) {
        showErr(ERR_COLOR,"[error]: data format error!");
        return false;
    }

    if(list.at(0) != "start_time") {
        showErr(ERR_COLOR,"[error]: data start_time error!");
        return false;
    }

    if(!detection_time(list.at(1))) {
        showErr(ERR_COLOR,"[error]: data start_time format error!");
        return false;
    }

    if(list.at(2) != "end_time") {
        showErr(ERR_COLOR,"[error]: data end_time error!");
        return false;
    }

    if(!detection_time(list.at(3))) {
        showErr(ERR_COLOR,"[error]: data end_time format error!");
        return false;
    }

    if(list.at(4) != "isactive") {
        showErr(ERR_COLOR,"[error]: data isactive error!");
        return false;
    }
    QString isactive = list.at(5);
    if(!(isactive.toInt() == 0 || isactive.toInt() == 1)) {
        showErr(ERR_COLOR,"[error]: data isactive format error!");
        return false;
    }

    if(list.at(6) != "week") {
        showErr(ERR_COLOR,"[error]: data week error!");
        return false;
    }

    if(!detection_week(list.at(7))) {
        showErr(ERR_COLOR,"[error]: data week format error!");
        return false;
    }

    return true;
}

bool mainui::detection_time(QString time)
{
    QTime t = QTime::fromString(time,"hh:mm");
    if(!t.isValid()) {
        return false;
    }
    return true;
}

bool mainui::detection_week(QString week)
{
    if(week.length() != 15) {
        return false;
    }
    QStringList list;
    list = week.split(",");
    if(list.at(0) != "[1" && list.at(0) != "[0" && list.last() != "1]" && list.last() != "0]") {
        return false;
    }

    QString index;
    for(int i=1;i<list.length()-1;i++) {
        index = list.at(i);
        if(index.toInt() != 0 && index.toInt() != 1) {
            return false;
        }
    }
    return true;
}

void mainui::assemble_json(QJsonObject *data)
{
    int weekindex;
    QString str;
    QStringList list;
    QJsonObject plan;

    data->insert("Action","ModPlan");
    for(int i=0;i<8;i++) {
        list.clear();
        QJsonArray array;
        QJsonObject planindex;
        planindex.insert(PlanDataList.at(i).at(0),PlanDataList.at(i).at(1));
        planindex.insert(PlanDataList.at(i).at(2),PlanDataList.at(i).at(3));
        str = PlanDataList.at(i).at(5);
        planindex.insert(PlanDataList.at(i).at(4),str.toInt());
        str = PlanDataList.at(i).at(7);
        list = str.split(",");

        weekindex = ((QString)list.at(0)).at(1).digitValue();
        array.insert(0,weekindex);
        for(int j=1;j<6;j++) {
            weekindex = ((QString)list.at(j)).toInt();
            array.insert(j,weekindex);
        }
        weekindex = ((QString)list.at(6)).at(0).digitValue();
        array.insert(6,weekindex);
        planindex.insert(PlanDataList.at(i).at(6),array);
        plan.insert(PlanNumberList.at(i),planindex);
    }
    data->insert("plan",plan);
}
