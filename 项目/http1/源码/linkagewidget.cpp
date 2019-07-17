#include "linkagewidget.h"
#include "ui_linkagewidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

LinkageWidget* LinkageWidget::m_linkageWidegt = NULL;
LinkageWidget::LinkageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinkageWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    m_linkageWidegt = NULL;
    m_frame = true;

    m_TitleHeight = style()->pixelMetric(QStyle::PM_TitleBarHeight);

    setProperty("form",true);
    ui->widgetTitle->setProperty("form", "title");
    ui->widget_2->setProperty("nav","no");
}

LinkageWidget::~LinkageWidget()
{
    delete ui;
}

void LinkageWidget::getParentGeometry(QWidget *parent, WidgetOrientation Orientation)
{
    if(parent != NULL) {
        m_rect = parent->geometry();
        if(Orientation == WidgetOrientation::Left){
            setGeometry(m_rect.x()-width(),m_rect.y(),width(),m_rect.height());
        }
        else if(Orientation == WidgetOrientation::Top){
            if(m_frame) {
                setGeometry(m_rect.x(),m_rect.y()-height()-m_TitleHeight,m_rect.width(),height());
            }
        }
        else if(Orientation == WidgetOrientation::Right){
            setGeometry(m_rect.x()+m_rect.width(),m_rect.y(),width(),m_rect.height());
        }
        else if(Orientation == WidgetOrientation::Bottom){
            setGeometry(m_rect.x(),m_rect.y()+m_rect.height(),m_rect.width(),height());
        }
    }
}

LinkageWidget* LinkageWidget::instance()
{
    if(m_linkageWidegt == NULL) {
        m_linkageWidegt = new LinkageWidget();
    }

    return m_linkageWidegt;
}

void LinkageWidget::on_btnMenu_Min_clicked()
{
    //showMinimized();
    hide();
    //setWindowState(Qt::WindowMinimized);
    emit usedhide();
}

void LinkageWidget::on_btnMenu_Close_clicked()
{
    hide();
    //setWindowState(Qt::WindowMinimized);
    emit usedhide();
}

void LinkageWidget::on_btnMenu_Max_clicked()
{
    //showMaximized();
}

void LinkageWidget::appendTextBrowserInfo(QString str)
{
    ui->textBrowser->append(str);
}

void LinkageWidget::setTitleText(QString str)
{
    ui->label->setText(str);
}

void LinkageWidget::on_clearBtn_clicked()
{
    ui->textBrowser->clear();
}
