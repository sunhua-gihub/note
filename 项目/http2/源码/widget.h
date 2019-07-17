#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDateTime>
#include <QTime>
#include <QMouseEvent>
#include <QButtonGroup>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    ~Widget();
    enum FontColor {
        NO_COLOR = 0,
        ERR_COLOR = 1,
        WARN_COLOR =2,
    };
    void showErr(int, QString);
    QString getBase64(QString);
    void MoveAni(QWidget *tarWgt,int during, bool dir);
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
private slots:
    void on_closebtn_clicked();
    void on_connectbtn_clicked();
    void on_sendbtn_clicked();
    void on_clearbtn_clicked();
    void Slot_BtnGrounpClicked();
    void Slot_TBtnGrounpClicked();
    void Slot_RecvJsonData(QJsonObject *data);
    void Slot_PersonMangeHide(QWidget*,int,bool);
private:
    int http_Register(QJsonObject *data);
    int http_HeartBeat(QJsonObject *data);
    int http_GetPersonList(QJsonObject *data);
    int http_GetPersonInfo(QJsonObject *data);
    int http_AddPersonInfo(QJsonObject *data);
    int http_EditPersonInfo(QJsonObject *data);
    int http_DelPersonList(QJsonObject *data);
    int http_DelPersonInfo(QJsonObject *data);
    int http_GetDeviceInfo(QJsonObject *data);
    int http_IOControl(QJsonObject *data);
    int http_GetParam(QJsonObject *data);
    int http_SetParam(QJsonObject *data);
    int http_SynTime(QJsonObject *data);
    bool max;
    bool mDragWindow;
    QRect location;
    QPoint mMousePoint;

    Ui::Widget *ui;
    QTime m_time;
    QButtonGroup CaptureEnabledbox;
    QButtonGroup FaceInfobox;
    QButtonGroup CompareInfobox;
    QButtonGroup FacePicturebox;
    QButtonGroup BackgroundPicturebox;
    QButtonGroup PersonPhotobox;
    QButtonGroup RegisterEnabledbox;
    QButtonGroup HeartbeatEnabledbox;
    QButtonGroup SignCheckbox;
    QButtonGroup GetPhotobox;
};

#endif // WIDGET_H
