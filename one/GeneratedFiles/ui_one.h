/********************************************************************************
** Form generated from reading UI file 'one.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONE_H
#define UI_ONE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_oneClass
{
public:
    QWidget *centralWidget;
    QPushButton *btn_inp;
    QLabel *lbl_deb_8;
    QLabel *lbl_deb_4;
    QPushButton *btn_cla_7;
    QPushButton *btn_cla_2;
    QPushButton *btn_cla_8;
    QLabel *lbl_deb_3;
    QLabel *lbl_deb_6;
    QPushButton *btn_cla_4;
    QPushButton *btn_cla_3;
    QLabel *lbl_deb_5;
    QPushButton *btn_cla_5;
    QPushButton *btn_cla_6;
    QLabel *lbl_deb_2;
    QLabel *lbl_deb_7;
    QPushButton *btn_cla;
    QLabel *lbl_deb;

    void setupUi(QMainWindow *oneClass)
    {
        if (oneClass->objectName().isEmpty())
            oneClass->setObjectName(QStringLiteral("oneClass"));
        oneClass->resize(500, 500);
        oneClass->setMinimumSize(QSize(500, 500));
        oneClass->setMaximumSize(QSize(500, 500));
        oneClass->setAutoFillBackground(false);
        oneClass->setStyleSheet(QLatin1String("border:2px groove gray;border-radius:10px;padding:2px 4px;\n"
"color: rgb(255, 255, 255);\n"
"background-image: url(:/new/picture/beauty2.jpg);\n"
""));
        centralWidget = new QWidget(oneClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btn_inp = new QPushButton(centralWidget);
        btn_inp->setObjectName(QStringLiteral("btn_inp"));
        btn_inp->setGeometry(QRect(40, 220, 81, 30));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_inp->sizePolicy().hasHeightForWidth());
        btn_inp->setSizePolicy(sizePolicy);
        btn_inp->setMinimumSize(QSize(0, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        btn_inp->setFont(font);
        btn_inp->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        lbl_deb_8 = new QLabel(centralWidget);
        lbl_deb_8->setObjectName(QStringLiteral("lbl_deb_8"));
        lbl_deb_8->setGeometry(QRect(319, 369, 101, 30));
        lbl_deb_8->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);\n"
"font: 9pt \"\351\273\221\344\275\223\";\n"
"border-color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        lbl_deb_8->setAlignment(Qt::AlignCenter);
        lbl_deb_4 = new QLabel(centralWidget);
        lbl_deb_4->setObjectName(QStringLiteral("lbl_deb_4"));
        lbl_deb_4->setGeometry(QRect(319, 209, 101, 30));
        lbl_deb_4->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);\n"
"font: 9pt \"\351\273\221\344\275\223\";\n"
"border-color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        lbl_deb_4->setAlignment(Qt::AlignCenter);
        btn_cla_7 = new QPushButton(centralWidget);
        btn_cla_7->setObjectName(QStringLiteral("btn_cla_7"));
        btn_cla_7->setGeometry(QRect(180, 330, 120, 30));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btn_cla_7->sizePolicy().hasHeightForWidth());
        btn_cla_7->setSizePolicy(sizePolicy1);
        btn_cla_7->setFont(font);
        btn_cla_7->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        btn_cla_2 = new QPushButton(centralWidget);
        btn_cla_2->setObjectName(QStringLiteral("btn_cla_2"));
        btn_cla_2->setGeometry(QRect(180, 130, 120, 30));
        sizePolicy1.setHeightForWidth(btn_cla_2->sizePolicy().hasHeightForWidth());
        btn_cla_2->setSizePolicy(sizePolicy1);
        btn_cla_2->setFont(font);
        btn_cla_2->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        btn_cla_8 = new QPushButton(centralWidget);
        btn_cla_8->setObjectName(QStringLiteral("btn_cla_8"));
        btn_cla_8->setGeometry(QRect(180, 370, 120, 30));
        sizePolicy1.setHeightForWidth(btn_cla_8->sizePolicy().hasHeightForWidth());
        btn_cla_8->setSizePolicy(sizePolicy1);
        btn_cla_8->setFont(font);
        btn_cla_8->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        lbl_deb_3 = new QLabel(centralWidget);
        lbl_deb_3->setObjectName(QStringLiteral("lbl_deb_3"));
        lbl_deb_3->setGeometry(QRect(319, 169, 101, 30));
        lbl_deb_3->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);\n"
"font: 9pt \"\351\273\221\344\275\223\";\n"
"background-color: rgb(0, 0, 0);\n"
"border-color: rgb(255, 255, 255);"));
        lbl_deb_3->setAlignment(Qt::AlignCenter);
        lbl_deb_6 = new QLabel(centralWidget);
        lbl_deb_6->setObjectName(QStringLiteral("lbl_deb_6"));
        lbl_deb_6->setGeometry(QRect(319, 289, 101, 30));
        lbl_deb_6->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);\n"
"font: 9pt \"\351\273\221\344\275\223\";\n"
"border-color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        lbl_deb_6->setAlignment(Qt::AlignCenter);
        btn_cla_4 = new QPushButton(centralWidget);
        btn_cla_4->setObjectName(QStringLiteral("btn_cla_4"));
        btn_cla_4->setGeometry(QRect(180, 210, 120, 30));
        sizePolicy1.setHeightForWidth(btn_cla_4->sizePolicy().hasHeightForWidth());
        btn_cla_4->setSizePolicy(sizePolicy1);
        btn_cla_4->setFont(font);
        btn_cla_4->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        btn_cla_3 = new QPushButton(centralWidget);
        btn_cla_3->setObjectName(QStringLiteral("btn_cla_3"));
        btn_cla_3->setGeometry(QRect(180, 170, 120, 30));
        sizePolicy1.setHeightForWidth(btn_cla_3->sizePolicy().hasHeightForWidth());
        btn_cla_3->setSizePolicy(sizePolicy1);
        btn_cla_3->setFont(font);
        btn_cla_3->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        lbl_deb_5 = new QLabel(centralWidget);
        lbl_deb_5->setObjectName(QStringLiteral("lbl_deb_5"));
        lbl_deb_5->setGeometry(QRect(319, 249, 101, 30));
        lbl_deb_5->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);\n"
"font: 9pt \"\351\273\221\344\275\223\";\n"
"background-color: rgb(0, 0, 0);\n"
"border-color: rgb(255, 255, 255);"));
        lbl_deb_5->setAlignment(Qt::AlignCenter);
        btn_cla_5 = new QPushButton(centralWidget);
        btn_cla_5->setObjectName(QStringLiteral("btn_cla_5"));
        btn_cla_5->setGeometry(QRect(180, 250, 120, 30));
        sizePolicy1.setHeightForWidth(btn_cla_5->sizePolicy().hasHeightForWidth());
        btn_cla_5->setSizePolicy(sizePolicy1);
        btn_cla_5->setFont(font);
        btn_cla_5->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        btn_cla_6 = new QPushButton(centralWidget);
        btn_cla_6->setObjectName(QStringLiteral("btn_cla_6"));
        btn_cla_6->setGeometry(QRect(180, 290, 120, 30));
        sizePolicy1.setHeightForWidth(btn_cla_6->sizePolicy().hasHeightForWidth());
        btn_cla_6->setSizePolicy(sizePolicy1);
        btn_cla_6->setFont(font);
        btn_cla_6->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        lbl_deb_2 = new QLabel(centralWidget);
        lbl_deb_2->setObjectName(QStringLiteral("lbl_deb_2"));
        lbl_deb_2->setGeometry(QRect(319, 129, 101, 30));
        lbl_deb_2->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);\n"
"background-color: rgb(0, 0, 0);\n"
"font: 9pt \"\351\273\221\344\275\223\";\n"
"border-color: rgb(255, 255, 255);"));
        lbl_deb_2->setAlignment(Qt::AlignCenter);
        lbl_deb_7 = new QLabel(centralWidget);
        lbl_deb_7->setObjectName(QStringLiteral("lbl_deb_7"));
        lbl_deb_7->setGeometry(QRect(319, 329, 101, 30));
        lbl_deb_7->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);\n"
"font: 9pt \"\351\273\221\344\275\223\";\n"
"border-color: rgb(255, 255, 255);\n"
"background-color: rgb(0, 0, 0);"));
        lbl_deb_7->setAlignment(Qt::AlignCenter);
        btn_cla = new QPushButton(centralWidget);
        btn_cla->setObjectName(QStringLiteral("btn_cla"));
        btn_cla->setGeometry(QRect(180, 90, 120, 30));
        sizePolicy1.setHeightForWidth(btn_cla->sizePolicy().hasHeightForWidth());
        btn_cla->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        btn_cla->setFont(font1);
        btn_cla->setStyleSheet(QStringLiteral("border-color: rgb(255, 255, 255);"));
        lbl_deb = new QLabel(centralWidget);
        lbl_deb->setObjectName(QStringLiteral("lbl_deb"));
        lbl_deb->setGeometry(QRect(319, 89, 101, 30));
        sizePolicy1.setHeightForWidth(lbl_deb->sizePolicy().hasHeightForWidth());
        lbl_deb->setSizePolicy(sizePolicy1);
        lbl_deb->setStyleSheet(QString::fromUtf8("background-color: rgb(181, 181, 181);\n"
"background-color: rgb(0, 0, 0);\n"
"font: 9pt \"\351\273\221\344\275\223\";\n"
"border-bottom-color: rgb(0, 0, 0);\n"
"border-color: rgb(255, 255, 255);"));
        lbl_deb->setAlignment(Qt::AlignCenter);
        oneClass->setCentralWidget(centralWidget);
        lbl_deb_8->raise();
        lbl_deb_4->raise();
        btn_cla_7->raise();
        btn_cla_2->raise();
        btn_cla_8->raise();
        lbl_deb_3->raise();
        lbl_deb_6->raise();
        btn_cla_4->raise();
        btn_cla_3->raise();
        lbl_deb_5->raise();
        btn_cla_5->raise();
        btn_cla_6->raise();
        lbl_deb_2->raise();
        lbl_deb_7->raise();
        lbl_deb->raise();
        btn_inp->raise();
        btn_cla->raise();

        retranslateUi(oneClass);

        QMetaObject::connectSlotsByName(oneClass);
    } // setupUi

    void retranslateUi(QMainWindow *oneClass)
    {
        oneClass->setWindowTitle(QApplication::translate("oneClass", "one", 0));
        btn_inp->setText(QApplication::translate("oneClass", "\345\257\274\345\205\245\345\233\276\347\211\207", 0));
        lbl_deb_8->setText(QString());
        lbl_deb_4->setText(QString());
        btn_cla_7->setText(QApplication::translate("oneClass", "\344\270\255\345\200\274\346\273\244\346\263\242CPU", 0));
        btn_cla_2->setText(QApplication::translate("oneClass", "\347\273\217\345\205\270\346\240\270\345\233\236\345\275\222GPU", 0));
        btn_cla_8->setText(QApplication::translate("oneClass", "\344\270\255\345\200\274\346\273\244\346\263\242GPU", 0));
        lbl_deb_3->setText(QString());
        lbl_deb_6->setText(QString());
        btn_cla_4->setText(QApplication::translate("oneClass", "\350\207\252\351\200\202\345\272\224\346\240\270\345\233\236\345\275\222GPU", 0));
        btn_cla_3->setText(QApplication::translate("oneClass", "\350\207\252\351\200\202\345\272\224\346\240\270\345\233\236\345\275\222CPU", 0));
        lbl_deb_5->setText(QString());
        btn_cla_5->setText(QApplication::translate("oneClass", "\345\235\207\345\200\274\346\273\244\346\263\242CPU", 0));
        btn_cla_6->setText(QApplication::translate("oneClass", "\345\235\207\345\200\274\346\273\244\346\263\242GPU", 0));
        lbl_deb_2->setText(QString());
        lbl_deb_7->setText(QString());
        btn_cla->setText(QApplication::translate("oneClass", "\347\273\217\345\205\270\346\240\270\345\233\236\345\275\222CPU", 0));
        lbl_deb->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class oneClass: public Ui_oneClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONE_H
