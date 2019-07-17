#ifndef PERSONMANAGE_H
#define PERSONMANAGE_H

#include <QDialog>
#include <QJsonObject>
#include <QButtonGroup>
namespace Ui {
class PersonManage;
}

class PersonManage : public QDialog
{
    Q_OBJECT

public:
    enum FontColor {
        NO_COLOR = 0,
        ERR_COLOR = 1,
        WARN_COLOR =2,
    };
    enum PERSONMANAGETYPE{
        ADDPERSON = 0,
        EDITPERSON = 1,
    };
    explicit PersonManage(QWidget *parent = 0);
    ~PersonManage();
    static PersonManage* instance();
    void MoveAni();
    void setPersonManagerType(PERSONMANAGETYPE type){mtype = type;}
    PERSONMANAGETYPE getPersonManagerType(){return mtype;}
signals:
    void timetohide(QWidget*,int,bool);
    void sendJsonData(QJsonObject *data);
    void sendErr(int,QString);
private slots:
    void on_confirmbtn_clicked();
    void on_cannelbtn_clicked();
    void on_getpicbtn_clicked();

private:
    QString getBase64(QString path);
    Ui::PersonManage *ui;
    static PersonManage *m_PersonManage;
    QButtonGroup sexbox;
    QButtonGroup typebox;
    QButtonGroup timebox;
    PERSONMANAGETYPE mtype;
};

#endif // PERSONMANAGE_H
