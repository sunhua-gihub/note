#include "mainwin.h"
#include "ui_mainwin.h"

#include <QDesktopWidget>
#include <QDebug>
mainwin::mainwin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainwin)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    max = false;
    mDragWindow = false;
    mResizeWindow = NOTHING;
    location = geometry();
    qDebug() << pos();
}

mainwin::~mainwin()
{
    delete ui;
}

void mainwin::on_closebtn_clicked()
{
    close();
}

void mainwin::on_minbtn_clicked()
{
    showMinimized();
}

void mainwin::on_maxbtn_clicked()
{
    if (max) {
        setGeometry(location);
        mDragWindow = true;
    } else {
        location = geometry();
        setGeometry(qApp->desktop()->availableGeometry());
        mDragWindow = false;
    }

    max = !max;
}

void mainwin::mousePressEvent(QMouseEvent *e)
{
    if (!max && e->button() == Qt::LeftButton) {
        if(e->pos().x() > width()-5) {
            if(e->pos().y() > height()-5) {
                mResizeWindow = BOTTOMRIGHT;
            }else if(e->pos().y() < 5) {
                mResizeWindow = TOPRIGHT;
            }else {
                mResizeWindow = RIGHT;
            }
        }else if(e->pos().x() < 5) {
            if(e->pos().y() > height()-5) {
                mResizeWindow = BOTTOMLEFT;
            }else if(e->pos().y() < 5) {
                mResizeWindow = TOPLEFT;
            }else {
                mResizeWindow = LEFT;
            }
        }else if(e->pos().y() > height()-5) {
             mResizeWindow = BOTTOM;
        }
        else if(e->pos().y() < 5) {
             mResizeWindow = TOP;
        }
        else{
            mMousePoint = e->globalPos() - this->pos();
            e->accept();
            mDragWindow = true;
        }
        mResizePoint = e->globalPos();
    }
}

void mainwin::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    mDragWindow = false;
    mResizeWindow = NOTHING;
    location = geometry();

}

void mainwin::mouseMoveEvent(QMouseEvent *e)
{
    if (mDragWindow && (e->buttons() && Qt::LeftButton)) {
        this->move(e->globalPos() - mMousePoint);
        e->accept();
    }else if(mResizeWindow != NOTHING) {
        switch(mResizeWindow) {
            case TOP:
                this->setGeometry(pos().x(),e->globalPos().y(),location.width(),mResizePoint.y()+location.height()-e->globalPos().y());
            break;
            case TOPRIGHT:
            break;
            case RIGHT:
                this->setGeometry(pos().x(),pos().y(),e->globalPos().x()-mResizePoint.x()+location.width(),location.height());
            break;
            case BOTTOMRIGHT:
                this->setGeometry(pos().x(),pos().y(),e->globalPos().x()-mResizePoint.x()+location.width(),e->globalPos().y()-mResizePoint.y()+location.height());
            break;
            case BOTTOM:
                this->setGeometry(pos().x(),pos().y(),location.width(),location.height()+ e->globalPos().y()-mResizePoint.y());
            break;
            case BOTTOMLEFT:
                this->setGeometry(e->globalPos().x(),pos().y(),mResizePoint.x()-e->globalPos().x()+location.width(),e->globalPos().y()-mResizePoint.y()+location.height());
            break;
            case LEFT:
                this->setGeometry(e->globalPos().x(),pos().y(),location.width()+mResizePoint.x()-e->globalPos().x(),location.height());
            break;
            case TOPLEFT:
                this->setGeometry(e->globalPos().x(),e->globalPos().y(),location.width()+mResizePoint.x()-e->globalPos().x(),location.height()+mResizePoint.y()-e->globalPos().y());
            break;
        }
    }
}


