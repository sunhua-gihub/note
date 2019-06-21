#ifndef HELPDLG_H
#define HELPDLG_H

#include <QDialog>

#include <QSyntaxHighlighter>

class mysyntaxhighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit mysyntaxhighlighter(QTextDocument *parent = 0);
private:
    void highlightBlock(const QString &text);
};

namespace Ui {
class helpdlg;
}

class helpdlg : public QDialog
{
    Q_OBJECT

public:
    explicit helpdlg(QWidget *parent = 0);
    ~helpdlg();

private:
    Ui::helpdlg *ui;
    mysyntaxhighlighter *highlighter;
};

#endif // HELPDLG_H
