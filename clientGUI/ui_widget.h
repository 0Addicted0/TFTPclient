/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QHBoxLayout *horizontalLayout_4;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QLabel *serverIPLabel;
    QLineEdit *IPLine;
    QSpacerItem *horizontalSpacer_6;
    QLabel *portLabel;
    QLineEdit *portLine;
    QHBoxLayout *horizontalLayout_7;
    QLabel *tryCNTLabel;
    QLineEdit *tryCNTLine;
    QSpacerItem *horizontalSpacer_7;
    QLabel *timeoutLabel;
    QLineEdit *timeoutLine;
    QHBoxLayout *horizontalLayout;
    QPushButton *folderButton;
    QSpacerItem *horizontalSpacer_8;
    QLineEdit *folderLine;
    QLabel *speedLabel;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_9;
    QLabel *modelLabel;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *remoteLabel;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_5;
    QLineEdit *remoteFileLine;
    QLabel *lossRateLabel;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *commitButton;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QTreeWidget *messageTreeWidget;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(1000, 407);
        Widget->setMinimumSize(QSize(1000, 407));
        Widget->setMaximumSize(QSize(16777215, 407));
        horizontalLayout_4 = new QHBoxLayout(Widget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        splitter = new QSplitter(Widget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setMinimumSize(QSize(900, 0));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setSpacing(15);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(30);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(10, 10, 10, 10);
        serverIPLabel = new QLabel(layoutWidget);
        serverIPLabel->setObjectName(QString::fromUtf8("serverIPLabel"));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(12);
        serverIPLabel->setFont(font);

        horizontalLayout_6->addWidget(serverIPLabel);

        IPLine = new QLineEdit(layoutWidget);
        IPLine->setObjectName(QString::fromUtf8("IPLine"));
        IPLine->setMinimumSize(QSize(0, 36));
        IPLine->setMaximumSize(QSize(1120, 36));
        IPLine->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        horizontalLayout_6->addWidget(IPLine);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);

        portLabel = new QLabel(layoutWidget);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        portLabel->setFont(font);

        horizontalLayout_6->addWidget(portLabel);

        portLine = new QLineEdit(layoutWidget);
        portLine->setObjectName(QString::fromUtf8("portLine"));
        portLine->setMinimumSize(QSize(0, 36));
        portLine->setMaximumSize(QSize(1120, 36));
        portLine->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        horizontalLayout_6->addWidget(portLine);

        horizontalLayout_6->setStretch(0, 1);
        horizontalLayout_6->setStretch(1, 4);
        horizontalLayout_6->setStretch(2, 1);
        horizontalLayout_6->setStretch(3, 1);
        horizontalLayout_6->setStretch(4, 4);

        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(10, 10, 10, 10);
        tryCNTLabel = new QLabel(layoutWidget);
        tryCNTLabel->setObjectName(QString::fromUtf8("tryCNTLabel"));
        tryCNTLabel->setFont(font);

        horizontalLayout_7->addWidget(tryCNTLabel);

        tryCNTLine = new QLineEdit(layoutWidget);
        tryCNTLine->setObjectName(QString::fromUtf8("tryCNTLine"));
        tryCNTLine->setMinimumSize(QSize(0, 36));
        tryCNTLine->setMaximumSize(QSize(1120, 36));
        tryCNTLine->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        horizontalLayout_7->addWidget(tryCNTLine);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);

        timeoutLabel = new QLabel(layoutWidget);
        timeoutLabel->setObjectName(QString::fromUtf8("timeoutLabel"));
        timeoutLabel->setFont(font);

        horizontalLayout_7->addWidget(timeoutLabel);

        timeoutLine = new QLineEdit(layoutWidget);
        timeoutLine->setObjectName(QString::fromUtf8("timeoutLine"));
        timeoutLine->setMinimumSize(QSize(0, 36));
        timeoutLine->setMaximumSize(QSize(1120, 36));
        timeoutLine->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        horizontalLayout_7->addWidget(timeoutLine);

        horizontalLayout_7->setStretch(0, 1);
        horizontalLayout_7->setStretch(1, 4);
        horizontalLayout_7->setStretch(2, 1);
        horizontalLayout_7->setStretch(4, 4);

        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, 10, -1);
        folderButton = new QPushButton(layoutWidget);
        folderButton->setObjectName(QString::fromUtf8("folderButton"));
        folderButton->setMinimumSize(QSize(60, 60));
        folderButton->setMaximumSize(QSize(60, 60));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        folderButton->setFont(font1);
        folderButton->setFlat(true);

        horizontalLayout->addWidget(folderButton);

        horizontalSpacer_8 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);

        folderLine = new QLineEdit(layoutWidget);
        folderLine->setObjectName(QString::fromUtf8("folderLine"));
        folderLine->setMinimumSize(QSize(0, 36));
        folderLine->setMaximumSize(QSize(1120, 36));
        folderLine->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));

        horizontalLayout->addWidget(folderLine);

        speedLabel = new QLabel(layoutWidget);
        speedLabel->setObjectName(QString::fromUtf8("speedLabel"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font2.setPointSize(10);
        speedLabel->setFont(font2);

        horizontalLayout->addWidget(speedLabel);

        progressBar = new QProgressBar(layoutWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setMinimumSize(QSize(0, 30));
        progressBar->setMaximumSize(QSize(520, 30));
        progressBar->setStyleSheet(QString::fromUtf8("font: 10pt \"\351\273\221\344\275\223\";"));
        progressBar->setValue(24);

        horizontalLayout->addWidget(progressBar);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(2, 6);
        horizontalLayout->setStretch(4, 3);

        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(1);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(5, -1, -1, -1);
        horizontalSpacer_9 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_9);

        modelLabel = new QLabel(layoutWidget);
        modelLabel->setObjectName(QString::fromUtf8("modelLabel"));
        modelLabel->setMaximumSize(QSize(60, 60));
        modelLabel->setFont(font);
        modelLabel->setLayoutDirection(Qt::LeftToRight);
        modelLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(modelLabel);

        horizontalSpacer = new QSpacerItem(40, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, -1, 10, -1);
        remoteLabel = new QLabel(layoutWidget);
        remoteLabel->setObjectName(QString::fromUtf8("remoteLabel"));
        remoteLabel->setFont(font);

        horizontalLayout_3->addWidget(remoteLabel);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        remoteFileLine = new QLineEdit(layoutWidget);
        remoteFileLine->setObjectName(QString::fromUtf8("remoteFileLine"));
        remoteFileLine->setMinimumSize(QSize(0, 36));
        remoteFileLine->setFont(font);

        verticalLayout_5->addWidget(remoteFileLine);

        lossRateLabel = new QLabel(layoutWidget);
        lossRateLabel->setObjectName(QString::fromUtf8("lossRateLabel"));
        lossRateLabel->setFont(font2);

        verticalLayout_5->addWidget(lossRateLabel);


        horizontalLayout_3->addLayout(verticalLayout_5);

        horizontalSpacer_2 = new QSpacerItem(50, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        commitButton = new QPushButton(layoutWidget);
        commitButton->setObjectName(QString::fromUtf8("commitButton"));
        commitButton->setMinimumSize(QSize(60, 60));
        commitButton->setMaximumSize(QSize(60, 60));
        commitButton->setFont(font1);

        horizontalLayout_3->addWidget(commitButton);

        horizontalLayout_3->setStretch(1, 1);
        horizontalLayout_3->setStretch(2, 8);
        horizontalLayout_3->setStretch(3, 1);

        horizontalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(2, 1);
        horizontalLayout_2->setStretch(3, 3);

        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalLayout_3->setStretch(0, 3);
        verticalLayout_3->setStretch(1, 1);
        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        messageTreeWidget = new QTreeWidget(layoutWidget1);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(2, font2);
        __qtreewidgetitem->setFont(1, font2);
        __qtreewidgetitem->setText(0, QString::fromUtf8("\344\274\240\350\276\223\347\261\273\345\236\213"));
        __qtreewidgetitem->setFont(0, font2);
        messageTreeWidget->setHeaderItem(__qtreewidgetitem);
        messageTreeWidget->setObjectName(QString::fromUtf8("messageTreeWidget"));
        messageTreeWidget->setMinimumSize(QSize(400, 0));
        messageTreeWidget->setStyleSheet(QString::fromUtf8("font: 10pt \"\351\273\221\344\275\223\";"));

        verticalLayout->addWidget(messageTreeWidget);

        splitter->addWidget(layoutWidget1);

        horizontalLayout_4->addWidget(splitter);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "clientGUI", nullptr));
        serverIPLabel->setText(QCoreApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250ip", nullptr));
        IPLine->setText(QCoreApplication::translate("Widget", "127.0.0.1", nullptr));
        portLabel->setText(QCoreApplication::translate("Widget", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243 ", nullptr));
        portLine->setText(QCoreApplication::translate("Widget", "69", nullptr));
        tryCNTLabel->setText(QCoreApplication::translate("Widget", "\346\234\200\345\244\247\351\207\215\350\257\225", nullptr));
        tryCNTLine->setText(QCoreApplication::translate("Widget", "1", nullptr));
        timeoutLabel->setText(QCoreApplication::translate("Widget", "\346\234\200\345\244\247\350\266\205\346\227\266/ms", nullptr));
        timeoutLine->setText(QCoreApplication::translate("Widget", "500", nullptr));
        folderButton->setText(QString());
        folderLine->setText(QCoreApplication::translate("Widget", "C:/Users/29429/Desktop/", nullptr));
        speedLabel->setText(QCoreApplication::translate("Widget", "0kb/s", nullptr));
        modelLabel->setText(QCoreApplication::translate("Widget", "\344\270\212\344\274\240", nullptr));
        remoteLabel->setText(QCoreApplication::translate("Widget", "\350\277\234\347\253\257\346\226\207\344\273\266", nullptr));
        remoteFileLine->setText(QString());
        lossRateLabel->setText(QCoreApplication::translate("Widget", "<html><head/><body><p align=\"center\">\344\270\242\345\214\205\347\216\207:0.00%</p></body></html>", nullptr));
        commitButton->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = messageTreeWidget->headerItem();
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("Widget", "\347\212\266\346\200\201", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("Widget", "\346\226\207\344\273\266\345\220\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
