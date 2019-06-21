#include "helpdlg.h"
#include "ui_helpdlg.h"

#include <QFile>
#include <QTextStream>


mysyntaxhighlighter::mysyntaxhighlighter(QTextDocument *parent):
    QSyntaxHighlighter(parent)
{

}

void mysyntaxhighlighter::highlightBlock(const QString &text)
{
    QTextCharFormat myFormat;
    myFormat.setFontWeight(QFont::Bold);
    myFormat.setForeground(Qt::green);
    QString pattern = "[(].*[)]";                         //要匹配的字符串
    QRegExp expression(pattern);
    int index = text.indexOf(expression);                   //匹配字符串
    while(index>=0)
    {
        int length = expression.matchedLength();            //匹配长度
        setFormat(index,length,myFormat);                   //设置格式
        index = text.indexOf(expression,index +length);    //继续匹配
    }
}

helpdlg::helpdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpdlg)
{
    ui->setupUi(this);
    QFile file(":/file/help.txt");
    file.open(QIODevice::ReadOnly |QIODevice::Truncate);
    QTextStream in(&file);
    QString str;
    while(!in.atEnd())
    {
        str.append(in.readLine());
        str.append("\n");
    }

    highlighter = new mysyntaxhighlighter(ui->textEdit->document());
    ui->textEdit->append(str);
}

helpdlg::~helpdlg()
{
    delete ui;
    delete highlighter;
}
