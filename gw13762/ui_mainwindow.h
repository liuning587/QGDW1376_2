/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Feb 24 08:29:53 2014
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_F1;
    QAction *action_F2;
    QAction *action_C;
    QAction *action_Q;
    QAction *action_2013;
    QAction *action_S;
    QAction *action;
    QAction *action_H;
    QAction *action_D;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTextEdit *textEdit;
    QTextBrowser *textBrowser;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(700, 500);
        action_F1 = new QAction(MainWindow);
        action_F1->setObjectName(QString::fromUtf8("action_F1"));
        action_F2 = new QAction(MainWindow);
        action_F2->setObjectName(QString::fromUtf8("action_F2"));
        action_C = new QAction(MainWindow);
        action_C->setObjectName(QString::fromUtf8("action_C"));
        action_Q = new QAction(MainWindow);
        action_Q->setObjectName(QString::fromUtf8("action_Q"));
        action_2013 = new QAction(MainWindow);
        action_2013->setObjectName(QString::fromUtf8("action_2013"));
        action_S = new QAction(MainWindow);
        action_S->setObjectName(QString::fromUtf8("action_S"));
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        action_H = new QAction(MainWindow);
        action_H->setObjectName(QString::fromUtf8("action_H"));
        action_D = new QAction(MainWindow);
        action_D->setObjectName(QString::fromUtf8("action_D"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setMinimumSize(QSize(100, 0));
        textEdit->setMaximumSize(QSize(300, 16777215));

        horizontalLayout->addWidget(textEdit);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        textBrowser->setFont(font);

        horizontalLayout->addWidget(textBrowser);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 700, 19));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(action_F1);
        menu->addAction(action_F2);
        menu->addAction(action_C);
        menu->addAction(action_S);
        menu->addAction(action_Q);
        menu_2->addAction(action_2013);
        menu_3->addAction(action);
        menu_3->addAction(action_H);
        menu_3->addAction(action_D);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\345\233\275\347\275\2211376.2\350\247\204\347\272\246\346\212\245\346\226\207\350\247\243\346\236\220", 0, QApplication::UnicodeUTF8));
        action_F1->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\212\245\346\226\207(&O) F1", 0, QApplication::UnicodeUTF8));
        action_F2->setText(QApplication::translate("MainWindow", "\350\247\243\346\236\220\346\212\245\346\226\207(&X) F2", 0, QApplication::UnicodeUTF8));
        action_C->setText(QApplication::translate("MainWindow", "\346\270\205\345\261\217(&C)       F3", 0, QApplication::UnicodeUTF8));
        action_Q->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272(&Q)", 0, QApplication::UnicodeUTF8));
        action_2013->setText(QApplication::translate("MainWindow", "1376.2(&N)", 0, QApplication::UnicodeUTF8));
        action_S->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\347\273\223\346\236\234(&S) F4", 0, QApplication::UnicodeUTF8));
        action->setText(QApplication::translate("MainWindow", "\347\211\210\346\234\254\350\257\264\346\230\216(&V)", 0, QApplication::UnicodeUTF8));
        action_H->setText(QApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", 0, QApplication::UnicodeUTF8));
        action_D->setText(QApplication::translate("MainWindow", "\344\270\213\350\275\275\346\234\200\346\226\260\347\211\210\346\234\254(&D)", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("MainWindow", "\350\217\234\345\215\225(&M)", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("MainWindow", "\350\247\243\346\236\220\350\256\276\347\275\256(&W)", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251\344\270\216\346\233\264\346\226\260(&H)", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
