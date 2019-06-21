#ifndef FILECHOOSE_H
#define FILECHOOSE_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class filechoose;
}

class filechoose : public QDialog
{
    Q_OBJECT

public:
    explicit filechoose(QWidget *parent = 0);
    ~filechoose();
    static filechoose* instance();
signals:
    void Signal_Path(QString);
private slots:
    void Slot_TreeViewDoubleClicked(const QModelIndex &);
private:
    Ui::filechoose *ui;
    QFileSystemModel *model;
    static filechoose *m_filechoose;
    QModelIndex m_index;
};

#endif // UPGRADECHOOSE_H
