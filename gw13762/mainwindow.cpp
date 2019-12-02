#include <stdlib.h>
#include <string.h>
#include <QtDebug>
#include <qmessagebox.h>
#include <QKeyEvent>
#include <QTextCodec>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GDW1376_2.h"

QTextBrowser *ptextBrowser = NULL;
static char the_text[1024*512];    //512k
static int the_pos = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("国网1376.2规约报文解析(%1)").arg(QString::fromLocal8Bit(GDW1376_2_ver_str())));
    ui->textBrowser->clear();
    ui->textBrowser->setText(QString::fromLocal8Bit(GDW1376_2_changelog()));
    ptextBrowser = ui->textBrowser;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_F1:
        on_action_F1_triggered();
        break;
    case Qt::Key_F2:
        on_action_F2_triggered();
        break;
    case Qt::Key_F3:
        on_action_C_triggered();
        break;
    case Qt::Key_F4:
        on_action_S_triggered();
        break;
    default:
        break;
    }
}


void printCallBack(const char *pstr)
{
    int len;

    if (ptextBrowser)
    {
        len = strlen(pstr);
        if ((unsigned)(the_pos + len) < sizeof(the_text))
        {
            memcpy(the_text + the_pos, pstr, len);
            the_pos += len;
        }
    }
}

void MainWindow::deBlank(QString &strs)
{
    int len = strs.length();
    int newlen = 0;

    for (int i = 0; i < len; i++)
    {
        if (!strs.at(i).isSpace())
        {
            strs[newlen++] = strs[i];
        }
    }
    strs.truncate(newlen);
}

void MainWindow::on_action_Q_triggered()
{
    this->close();
}

void MainWindow::on_action_F1_triggered()
{
    qDebug("打开报文");
}

void MainWindow::on_action_F2_triggered()
{
    //解析报文
    bool ok;
    int inLen;
    QString inData = ui->textEdit->toPlainText();

    deBlank(inData);    //去除空格
    inLen = inData.length();

    if (inLen & 0x01)
    {
        QMessageBox::warning(this, "提示", tr("输入数据非偶数个!"));
        return;
    }

    inLen /= 2;
    if (inLen <= 0)
    {
        return;
    }

    if (inLen > MAX_FRAME_LEN)
    {
        QMessageBox::warning(this, "提示", tr("输入数据不能大于%1字节!").arg(MAX_FRAME_LEN));
        return;
    }

    ok = true;
    for (int i = 0; i < inLen; i++)
    {
        inpbuf[i] = inData.left(2).toUInt(&ok, 16);
        if (ok != true)
        {
            break;
        }
        inData.remove(0, 2);
    }
    if (ok != true)
    {
        QMessageBox::warning(this, "提示", "输入数据含有非十六进制，请仔细检查!");
        return;
    }

    ui->textBrowser->clear();

    the_pos = 0;
    memset(the_text, 0x00, sizeof(the_text));

    QString my3762;
    for (int i = 0; i < inLen; i++)
    {
        if (inpbuf[i] < 0x10)
            my3762 += QString(tr("0"));
        my3762 += QString::number(inpbuf[i], 16).toUpper() + QString(tr(" "));
    }
    my3762.truncate(my3762.length() - 1);
    ui->textEdit->setText(my3762);

    int err = GDW1376_2_parse((const unsigned char *)inpbuf, inLen,
                              printCallBack, "", "\n");

    ptextBrowser->append(QString::fromLocal8Bit(the_text));
    if (err)
    {
        QMessageBox::warning(this, "提示", QString::fromLocal8Bit(GDW1376_2_error(err)));
    }
}

void MainWindow::on_action_C_triggered()
{
    //清屏
    if (ui->textBrowser->toPlainText().size() > 0)
    {
        ui->textBrowser->clear();
    }
    else
    {
        ui->textEdit->clear();
    }
}

void MainWindow::on_action_2013_triggered()
{
    //选择规约
}

void MainWindow::on_action_S_triggered()
{
    //保存结果
}

void MainWindow::on_action_triggered()
{
    //版本说明
    ui->textBrowser->clear();
    ui->textBrowser->setText(QString::fromLocal8Bit(GDW1376_2_changelog()));
}

void MainWindow::on_action_H_triggered()
{
    //帮助
    ui->textBrowser->clear();
    ui->textBrowser->setText(tr("F2:解析\nF3:清除"));
}

void MainWindow::on_action_D_triggered()
{
    //升级
}
