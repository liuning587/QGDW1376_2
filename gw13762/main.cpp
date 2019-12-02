#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 设置编码
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    MainWindow w;


    //加载中文字符
    QTranslator translator;
    translator.load(":/qt_zh_CN.qm");
    a.installTranslator(&translator);

    w.show();
    return a.exec();
}
