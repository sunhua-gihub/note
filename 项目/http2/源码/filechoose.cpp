#include "filechoose.h"
#include "ui_filechoose.h"

#include <QDesktopWidget>
#include <QDebug>

filechoose* filechoose::m_filechoose = NULL;
filechoose::filechoose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::filechoose)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    QSize size = qApp->desktop()->availableGeometry().size();
    setGeometry((size.width()-400)/2,(size.height()-300)/2,400,300);

    connect(ui->closebtn, &QPushButton::clicked, this, &filechoose::close);
    connect(ui->treeView, &QTreeView::doubleClicked, this, &filechoose::Slot_TreeViewDoubleClicked);
    model = new QFileSystemModel(this);
    model->setRootPath("");

    QStringList filter;
    filter<<"*.jpg";
    model->setNameFilters(filter);          //设置过滤标记
    model->setNameFilterDisables(false);    //设置过滤不可见

    ui->treeView->setModel(model);
    ui->treeView->setRootIndex(model->index(""));
    ui->treeView->setColumnHidden(1,true);
    ui->treeView->setColumnHidden(2,true);
    ui->treeView->setColumnHidden(3,true);
    ui->treeView->setHeaderHidden(true);

    if(m_index.column() != -1 && m_index.row() != -1) {
        qDebug() << m_index;
        ui->treeView->expand(m_index);
    }
}

filechoose::~filechoose()
{
    delete ui;
}

filechoose* filechoose::instance()
{
    if(m_filechoose == NULL) {
        m_filechoose = new filechoose();
    }

    return m_filechoose;
}

void filechoose::Slot_TreeViewDoubleClicked(const QModelIndex &index)
{
    m_index = index;
    QModelIndex child = index;
    QModelIndex parent = index.parent();

    QStringList filepath;
    while(child != parent) {
        filepath<<child.data().toString();
        child = parent;
        parent = parent.parent();
    }

    QString filename;
    int length = filepath.length();
    for(int i=length-1;i > 0;i--) {
        filename.append(filepath.at(i)).append("/");
    }
    filename.append(filepath.at(0));
    emit Signal_Path(filename);
}
