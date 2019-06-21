#ifndef LINKAGEWIDGET_H
#define LINKAGEWIDGET_H

#include <QWidget>

namespace Ui {
class LinkageWidget;
}

class LinkageWidget : public QWidget
{
    Q_OBJECT

public:
    enum WidgetOrientation {
        Left = 0,
        Top = 1,
        Right = 2,
        Bottom = 3,
    };
    explicit LinkageWidget(QWidget *parent = 0);
    ~LinkageWidget();
    static LinkageWidget* instance();
    void getParentGeometry(QWidget *parent,WidgetOrientation Orientation = WidgetOrientation::Right);
    void appendTextBrowserInfo(QString);
    void setTitleText(QString);
protected:
private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_clearBtn_clicked();
signals:
    void usedhide();
private:
    Ui::LinkageWidget *ui;

    static LinkageWidget *m_linkageWidegt;
    QRect m_rect;           //生成本对象的对象位置，可以理解为本对象父类位置，但本对象没有父类
    bool m_frame;           //有无边框标志
    int m_TitleHeight;      //标题栏高度
};

#endif // LINKAGEWIDGET_H
