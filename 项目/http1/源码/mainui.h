#ifndef MAINUI_H
#define MAINUI_H

#include <QDialog>
#include <QtNetwork>
#include <QButtonGroup>
#include <QStandardItemModel>

#include "linkagewidget.h"

namespace Ui {
class mainui;
}

class mainui : public QDialog
{
    Q_OBJECT

public:
    explicit mainui(QWidget *parent = 0);
    void setinit();
    bool detection_data(QStringList);
    bool detection_time(QString);
    bool detection_week(QString);
    void assemble_json(QJsonObject *);
    ~mainui();

    enum FontColor {
        NO_COLOR = 0,
        ERR_COLOR = 1,
        WARN_COLOR =2,
    };
    enum DevModule {
        BROWSE_DEV = 0,
        ADD_DEV = 1,
        MOD_DEV = 2,
        DEL_DEV = 3,
    };
    enum ListModule {
        BROWSE_LIST = 0,
        ADD_LIST = 1,
        MODIFY_LIST = 2,
        DEL_LIST = 3,
        CLEAR_LIST = 4,
        FIND_LIST = 5,
    };
    enum SignModule {
        GET_SIGNLIST = 0,
        GET_NOSIGNLIST = 1,
    };
    enum PlanModule {
        GET_PLAN = 0,
        MOD_PLAN = 1,
    };
    enum StatisModule {
        GET_STATIS = 0,
    };
    enum SettingModule {
        SET_DATETIME = 0,
        SET_NETWORK = 1,
        SET_BUFFANDTHRESHOLD = 2,
    };
    enum OtherModule {
        PIC_SERACH = 0,
        PIC_COMPARE = 1,
        PIC_DETECTION = 2,
    };
    enum RecvModule {
        CLEAR_RECV = 0,
    };
    enum ManageModule {
        DEV_MODULE = 0,
        LIST_MODULE = 1,
        PLAN_MODULE = 2,
        SIGN_MODULE = 3,
        STATIS_MODULE = 4,
        SETTING_MODULE = 5,
        OTHER_MODULE = 6,
        RECV_MODULE = 7,
    };
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void closeEvent(QCloseEvent *);
//    void focusInEvent(QFocusEvent *event);
//    void focusOutEvent(QFocusEvent *event);
    void changeEvent(QEvent *);
private slots:
    void replyFinished(QNetworkReply *);
    void on_btnMenu_Close_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Min_clicked();
    void buttonClick();
    void btnClick();
    void on_connect_clicked();
    void send_clicked();
    void filebtn_clicked();
    void on_filebtn7_clicked();
    void on_foldbtn7_clicked();
    void clear();
    void ChildWidgethide();
    void on_getfile3_clicked();

private:
    QString getBase64(QString);
    void showErr(int, QString);
    bool isJPG(QByteArray);
    void setModelDate(int, QString);

    void hidesomething();
    Ui::mainui *ui;
    bool max;
    bool mDragWindow;
    QRect location;
    QPoint mMousePoint;

    QStringList DevList;
    QStringList ListList;
    QStringList PlanList;
    QStringList SignList;
    QStringList StatisList;
    QStringList SettingList;
    QStringList OtherList;
    QStringList RecvList;

    QNetworkAccessManager *manager;
    QNetworkRequest m_request;

    QString m_url;
    QString m_filename;
    QString m_filenameT;
    QString m_formatfilename;
    int mleftbtn;
    int mtopbtn;

    QStandardItemModel *m_model;

    QButtonGroup *m_btngroup1;
    QButtonGroup *m_btngroup2;
    QButtonGroup *m_btngroup4;
    QButtonGroup *m_btngroup5;
    QButtonGroup *m_pbtngroup4;

    LinkageWidget *ChildWidget;
    bool m_ChildWidget_isShow;

    QList<QStringList> PlanDataList;
    QStringList PlanNumberList;
};

#endif // MAINUI_H




