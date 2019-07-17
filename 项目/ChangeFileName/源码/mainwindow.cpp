#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
//#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QToolButton>
#include <QToolBar>
#include <QSpinBox>

#include "QVariantListModel.h"
#include "judgepic.h"
#include "judgename.h"
#include "helpdlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_xls(NULL)
{
    ui->setupUi(this);
//主界面
    this->setWindowTitle("批量修改JPEG文件名");

    //工具栏
    QToolButton *toolBtn = new QToolButton(this);
    toolBtn->setText(tr("帮助"));
    ui->toolBar->addWidget(toolBtn);

    connect(toolBtn,SIGNAL(clicked(bool)),this,SLOT(Show_Help(bool)));

    m_model = new QStandardItemModel(this);
    ui->showinfo_tableView->setModel(m_model);

    QStringList header;

    header<<"文件名"<<"is jpeg?"<<"替换名字"<<"";
    m_model->setHorizontalHeaderLabels(header);

    QDirModel *model = new QDirModel();
    m_allpath = new PathSortFilter();
    m_allpath->setSourceModel(model);
    QTreeView *view = ui->path_treeView;
    view->setModel(m_allpath);

    view->setRootIndex(m_allpath->mapFromSource(model->index("/")));
    view->setColumnHidden(1,true);
    view->setColumnHidden(2,true);
    view->setColumnHidden(3,true);
    view->header()->hide();
    view->setRootIsDecorated(false);

    //ui->showinfo_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->showinfo_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //execl界面


//自定义界面
    QStringList items;

    items<<"固定文字"<<"自增量";
    ui->item_cBox->addItems(items);
    ui->item_cBox->setCurrentIndex(-1);
    ui->input_label->hide();
    ui->input_lineEdit->hide();
    ui->len_cBox->hide();
    ui->len_label->hide();

    m_smodel = new QStandardItemModel(this);
    ui->showitem_tableView->setModel(m_smodel);

    ui->showitem_tableView->horizontalHeader()->hide();
    //ui->showitem_tableView->verticalHeader()->hide();
    ui->showitem_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->showitem_tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->showitem_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->exportTwo_btn,SIGNAL(clicked(bool)),this,SLOT(on_export_btn_clicked()));
}

MainWindow::~MainWindow()
{
    delete m_model;
    delete m_smodel;
    delete ui;
}

void MainWindow::Show_Help(bool)
{
    helpdlg help(this);
    help.setWindowModality(Qt::WindowModal);
    help.exec();
}

void MainWindow::RecvPNGResult()
{
    int i=0;
    foreach (QString isPNG, m_isPNG) {
        m_model->setItem(i,1,new QStandardItem(isPNG));
        if("No" == isPNG)
            m_model->item(i,1)->setForeground(QBrush(QColor(255,0,0)));
        i++;
    }
}

void MainWindow::RecvRNAMEResult(QHash<int, QList<QString> > newfilename_list)
{
    QList<QString> list;
    QList<QString> temp;
    for(int i=0;i<m_model->rowCount();++i)
    {
        list = newfilename_list.value(i,temp);
        if(list != temp)
        {
            if(!list.at(0).isEmpty())
            {
                m_model->setItem(i,2,new QStandardItem(list.at(0)));
            }
            if(list.at(1) != "Ok")
            {
                m_model->setItem(i,3,new QStandardItem("修改失败"));
                m_model->item(i,3)->setForeground(QBrush(QColor(255,0,0)));
            }
            else
            {
                m_model->setItem(i,3,new QStandardItem("修改成功"));
            }
        }
    }

    m_filename.clear();
    m_directory.clear();
    m_isPNG.clear();
}

void MainWindow::on_xlsxpath_btn_clicked()
{
    QString xlsFile = QFileDialog::getOpenFileName(this,QString(),QString(),"excel(*.xls *.xlsx)");
    if(xlsFile.isEmpty())
       return;
    ui->xlsxpath_lineEdit->setText(xlsFile);

    if(m_xls.isNull())
       m_xls.reset(new ExcelBase);
    m_xls->open(xlsFile);
    m_xls->setCurrentSheet(1);
    m_xls->readAll(m_datas);
}

void MainWindow::on_start_btn_clicked()
{
    if(m_datas.isEmpty() || m_filename.isEmpty()
             || m_directory.isEmpty())
    {
        QMessageBox::warning(this,"警告!","未选定目标或已经完成！",QMessageBox::Cancel);
        return;
    }

    QString seg=ui->seg_lineEdit_2->text();
    int mark=ui->mark_cBox_2->currentIndex();
    int size= m_datas.at(0).size();

    if(mark-1 > size)
    {
        QMessageBox::warning(this,"警告!","标识项太大了！",QMessageBox::Cancel);
        return;
    }

    m_isPNG.clear();
    Judgepic *jud =new Judgepic(&m_filename,&m_isPNG,m_directory,&m_datas,seg,mark-1);
    connect(jud,SIGNAL(sendPNGResult()),this,SLOT(RecvPNGResult()));
    connect(jud,SIGNAL(sendRNAMEResult(QHash<int,QList<QString> >)),
            this,SLOT(RecvRNAMEResult(QHash<int,QList<QString> >)));
    jud->start();
}

void MainWindow::on_export_btn_clicked()
{
    QString xlsFile = QFileDialog::getExistingDirectory(this);
    if(xlsFile.isEmpty())
       return;

    QTime time =QTime::currentTime();
    QString name = time.toString("hhmmss");

    xlsFile += "/";
    xlsFile += name.append(".txt");

    QFile file(xlsFile);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream out(&file);
        QStandardItem *item;
        int row =m_model->rowCount();
        int column = m_model->columnCount();

        for(int i=0;i<row;i++)
        {
            if(item = m_model->item(i,3))
            {
                if(item->text() == "修改失败")
                {
                    out<<i<<". ";
                    for(int j=0;j<column;j++)
                    {
                        if(item = m_model->item(i,j))
                        {
                            out<<m_model->item(i,j)->text()<<"             ";
                        }
                    }
                    out<<endl;
                }
            }
            else
            {
                out<<i<<". ";
                for(int j=0;j<column;j++)
                {
                    if(item = m_model->item(i,j))
                    {
                        out<<m_model->item(i,j)->text()<<"             ";
                    }
                }
                out<<endl;
            }
        }
        file.close();

        QMessageBox::warning(this,"提示!","导出完成",QMessageBox::Cancel);
    }
}

void MainWindow::on_item_cBox_currentIndexChanged(int index)
{
    ui->input_label->show();
    ui->input_lineEdit->show();
    ui->input_lineEdit->clear();
    if(0 == index)
    {
        ui->input_label->setText("输入：");
        ui->len_label->hide();
        ui->len_cBox->hide();
    }
    else if(1 == index)
    {
        ui->input_label->setText("前缀：");
        ui->len_label->show();
        ui->len_cBox->show();
    }
}

void MainWindow::on_path_treeView_clicked(const QModelIndex &index)
{
    QList<QString> pathList;

    QModelIndex ParentIndex = index.parent();
    QModelIndex CurrentIndex = index;

    while(CurrentIndex != ParentIndex)
    {
        pathList.push_front(CurrentIndex.data().toString());
        CurrentIndex = ParentIndex;
        ParentIndex = CurrentIndex.parent();
    }

    QString path;
    QList<QString>::const_iterator iter;

    path.append(*pathList.constBegin());
    for(iter = pathList.constBegin()+1;iter != pathList.constEnd();++iter) {
        path.append("/").append(*iter);
    }

    ui->workpath_lineEdit->setText(path);
    m_directory = path;

    //遍历文件名
        QDir dir(m_directory);
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setSorting(QDir::Name);
    //清空原先数据
        int rows = m_model->rowCount();
        m_model->removeRows(0,rows);
        m_filename.clear();
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            QString filename;
            filename.append(fileInfo.fileName());
            m_filename.append(filename);
            m_model->setItem(i,0,new QStandardItem(filename));
        }
}

void MainWindow::on_add_btn_clicked()
{
    int itemIndex = ui->item_cBox->currentIndex();
    if(-1 == itemIndex)
        return;
#if 0
    if(ui->seg_lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"警告!","先填一下分割符嘛~",QMessageBox::Cancel);
        return;
    }
#endif
    QString str = ui->input_lineEdit->text();
    QList<QStandardItem *> items;
    if(0 == itemIndex)
    {
        if(!str.isEmpty())
        {
            QStandardItem *item1 = new QStandardItem(str);
            QStandardItem *item2 = new QStandardItem("0");
            items.append(item1);
            items.append(item2);

            m_smodel->appendRow(items);
        }
        else
            return;
    }
    else if(1 == itemIndex)
    {
        int len = ui->len_cBox->currentIndex();
        QString num;
        num = QString("%1").arg(1,len+3,10,QChar('0'));
        str.append(num);
        QStandardItem *item1 = new QStandardItem(str);
        QStandardItem *item2 = new QStandardItem(QString("%1").arg(len+3));
        items.append(item1);
        items.append(item2);

        m_smodel->appendRow(items);
    }
//显示名字
    QString namestr;
    int rows = m_smodel->rowCount();

    namestr.append(m_smodel->item(0)->text());
    for(int i=1;i<rows;i++)
    {
        namestr.append(ui->seg_lineEdit->text()).append(m_smodel->item(i)->text());
    }

    namestr.append(ui->extension_cBox->currentText());

    ui->showname_label->setText(namestr);
}

void MainWindow::on_startTwo_btn_clicked()
{
    if(m_filename.isEmpty())
    {
        QMessageBox::warning(this,"警告!","重新获取一下图片路径！",QMessageBox::Cancel);
        return;
    }

    int rows = m_smodel->rowCount();


    QList<QString> list;
    m_format.clear();

    for(int i=0;i<rows;i++)
    {
        list.append(m_smodel->item(i,0)->text());
        list.append(m_smodel->item(i,1)->text());
        m_format.append(list);
        list.clear();
    }

    list.append(ui->extension_cBox->currentText());
    list.append("0");
    m_format.append(list);
    list.clear();

    m_isPNG.clear();
    QString seg = ui->seg_lineEdit->text();
    int mark = ui->mark_cBox->currentIndex();

    judgename *jud =new judgename(&m_filename,&m_isPNG,&m_format,m_directory,seg,mark);
    connect(jud,SIGNAL(sendPNGResult()),this,SLOT(RecvPNGResult()));
    connect(jud,SIGNAL(sendRNAMEResult(QHash<int,QList<QString> >)),
            this,SLOT(RecvRNAMEResult(QHash<int,QList<QString> >)));
    jud->start();

    ui->mark_cBox->setCurrentIndex(0);
}


void MainWindow::on_showitem_tableView_pressed(const QModelIndex &index)
{
    if(qApp->mouseButtons() == Qt::RightButton)
    {
        int row = ui->showitem_tableView->currentIndex().row();

        m_smodel->removeRow(row);
        //显示名字
        QString namestr;
        int rows = m_smodel->rowCount();

        if(0 == rows)
        {
            ui->showname_label->clear();
            return;
        }
        namestr.append(m_smodel->item(0)->text());
        for(int i=1;i<rows;i++)
        {
            namestr.append(ui->seg_lineEdit->text()).append(m_smodel->item(i)->text());
        }

        namestr.append(ui->extension_cBox->currentText());
        ui->showname_label->setText(namestr);
    }
}

