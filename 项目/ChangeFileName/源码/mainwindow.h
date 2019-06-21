#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QScopedPointer>
#include <QList>
#include <QHash>
#include <QVariant>
#include <QStandardItemModel>

#include "ExcelBase.h"
#include "pathsortfilter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<QString> m_filename;
    QList<QString> m_isPNG;
    QList<QList<QVariant> > m_datas;
    QList<QList<QString> > m_format;

private slots:
    void RecvPNGResult();
    void RecvRNAMEResult(QHash<int,QList<QString> >);
    void Show_Help(bool);
    void on_xlsxpath_btn_clicked();
    void on_start_btn_clicked();
    void on_export_btn_clicked();

    void on_item_cBox_currentIndexChanged(int index);
    void on_path_treeView_clicked(const QModelIndex &index);
    void on_add_btn_clicked();
    void on_startTwo_btn_clicked();
    void on_showitem_tableView_pressed(const QModelIndex &index);
private:
    Ui::MainWindow *ui;

    QString m_directory;

    QScopedPointer<ExcelBase> m_xls;
    QStandardItemModel *m_model;
    QStandardItemModel *m_smodel;

    PathSortFilter *m_allpath;

    QList<QString> m_namestr;
};

#endif // MAINWINDOW_H
