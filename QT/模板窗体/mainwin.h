#ifndef MAINWIN_H
#define MAINWIN_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class mainwin;
}

class mainwin : public QDialog
{
    Q_OBJECT
public:
    explicit mainwin(QWidget *parent = 0);
    ~mainwin();

    enum RESIZEDIRECTION {
        NOTHING = 0,
        TOP,
        TOPRIGHT,
        RIGHT,
        BOTTOMRIGHT,
        BOTTOM,
        BOTTOMLEFT,
        LEFT,
        TOPLEFT
    };
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void on_closebtn_clicked();

    void on_minbtn_clicked();

    void on_maxbtn_clicked();

private:
    Ui::mainwin *ui;
    bool max;
    bool mDragWindow;
    QRect location;
    QPoint mMousePoint;
    QPoint mResizePoint;
    RESIZEDIRECTION mResizeWindow;
};

#endif // MAINWIN_H
