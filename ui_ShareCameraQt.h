/********************************************************************************
** Form generated from reading UI file 'ShareCameraQt.ui'
**
** Created: Sun Apr 21 16:14:35 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHARECAMERAQT_H
#define UI_SHARECAMERAQT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QScrollArea>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "ViewportWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ShareCameraQt
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionPrint;
    QAction *actionHelp;
    QAction *actionSave;
    QAction *actionFlip_Meshes;
    QAction *actionWrite_Back_Meshes;
    QAction *actionOpen_vtk_Files;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_6;
    QGroupBox *groupBox_4;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox;
    QGroupBox *groupBox_5;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBox_9;
    QHBoxLayout *horizontalLayout_7;
    QCheckBox *checkBox_10;
    QLineEdit *lineEdit;
    QLabel *label_5;
    QCheckBox *checkBox_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QWidget *widget;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_4;
    QToolButton *toolButton_3;
    QToolButton *toolButton_5;
    QToolButton *toolButton_6;
    ViewportWidget *dockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_5;
    QMenuBar *menuBar;
    QMenu *menuTools;
    QMenu *menuFile;

    void setupUi(QMainWindow *ShareCameraQt)
    {
        if (ShareCameraQt->objectName().isEmpty())
            ShareCameraQt->setObjectName(QString::fromUtf8("ShareCameraQt"));
        ShareCameraQt->resize(1118, 850);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ShareCameraQt->sizePolicy().hasHeightForWidth());
        ShareCameraQt->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Icons/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        ShareCameraQt->setWindowIcon(icon);
        ShareCameraQt->setIconSize(QSize(22, 22));
        actionOpenFile = new QAction(ShareCameraQt);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        actionOpenFile->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenFile->setIcon(icon1);
        actionExit = new QAction(ShareCameraQt);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        actionPrint = new QAction(ShareCameraQt);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icons/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon3);
        actionHelp = new QAction(ShareCameraQt);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionHelp->setIcon(icon);
        actionSave = new QAction(ShareCameraQt);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Icons/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon4);
        actionFlip_Meshes = new QAction(ShareCameraQt);
        actionFlip_Meshes->setObjectName(QString::fromUtf8("actionFlip_Meshes"));
        actionWrite_Back_Meshes = new QAction(ShareCameraQt);
        actionWrite_Back_Meshes->setObjectName(QString::fromUtf8("actionWrite_Back_Meshes"));
        actionOpen_vtk_Files = new QAction(ShareCameraQt);
        actionOpen_vtk_Files->setObjectName(QString::fromUtf8("actionOpen_vtk_Files"));
        centralwidget = new QWidget(ShareCameraQt);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_6 = new QGroupBox(centralwidget);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setMinimumSize(QSize(1091, 200));
        groupBox_6->setMaximumSize(QSize(1091, 300));
        groupBox_4 = new QGroupBox(groupBox_6);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(540, 50, 461, 131));
        verticalLayoutWidget = new QWidget(groupBox_4);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 20, 451, 101));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        comboBox = new QComboBox(verticalLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout->addWidget(comboBox);

        groupBox_5 = new QGroupBox(groupBox_6);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(50, 40, 351, 161));
        verticalLayoutWidget_3 = new QWidget(groupBox_5);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(0, 30, 331, 111));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        checkBox_9 = new QCheckBox(verticalLayoutWidget_3);
        checkBox_9->setObjectName(QString::fromUtf8("checkBox_9"));

        verticalLayout_2->addWidget(checkBox_9);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        checkBox_10 = new QCheckBox(verticalLayoutWidget_3);
        checkBox_10->setObjectName(QString::fromUtf8("checkBox_10"));

        horizontalLayout_7->addWidget(checkBox_10);

        lineEdit = new QLineEdit(verticalLayoutWidget_3);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout_7->addWidget(lineEdit);

        label_5 = new QLabel(verticalLayoutWidget_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_7->addWidget(label_5);


        verticalLayout_2->addLayout(horizontalLayout_7);

        checkBox_3 = new QCheckBox(verticalLayoutWidget_3);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        verticalLayout_2->addWidget(checkBox_3);

        groupBox = new QGroupBox(groupBox_6);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(410, 50, 121, 121));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        widget = new QWidget(groupBox);
        widget->setObjectName(QString::fromUtf8("widget"));
        toolButton = new QToolButton(widget);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(60, 0, 31, 31));
        toolButton_2 = new QToolButton(widget);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        toolButton_2->setGeometry(QRect(60, 30, 31, 31));
        toolButton_4 = new QToolButton(widget);
        toolButton_4->setObjectName(QString::fromUtf8("toolButton_4"));
        toolButton_4->setGeometry(QRect(0, 30, 31, 31));
        toolButton_3 = new QToolButton(widget);
        toolButton_3->setObjectName(QString::fromUtf8("toolButton_3"));
        toolButton_3->setGeometry(QRect(0, 0, 31, 31));
        toolButton_5 = new QToolButton(widget);
        toolButton_5->setObjectName(QString::fromUtf8("toolButton_5"));
        toolButton_5->setGeometry(QRect(30, 0, 31, 31));
        toolButton_6 = new QToolButton(widget);
        toolButton_6->setObjectName(QString::fromUtf8("toolButton_6"));
        toolButton_6->setGeometry(QRect(30, 30, 31, 31));

        gridLayout_3->addWidget(widget, 0, 0, 1, 1);

        groupBox_5->raise();
        groupBox_4->raise();
        groupBox->raise();

        gridLayout->addWidget(groupBox_6, 0, 1, 1, 1);

        ShareCameraQt->setCentralWidget(centralwidget);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(dockWidgetContents->sizePolicy().hasHeightForWidth());
        dockWidgetContents->setSizePolicy(sizePolicy1);
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        scrollArea = new QScrollArea(dockWidgetContents);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        sizePolicy1.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy1);
        scrollArea->setMinimumSize(QSize(1041, 384));
        scrollArea->setMaximumSize(QSize(524287, 524287));
        scrollArea->setMouseTracking(true);
        scrollArea->setLayoutDirection(Qt::LeftToRight);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setWidgetResizable(false);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1094, 475));
        sizePolicy1.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy1);
        scrollAreaWidgetContents->setMouseTracking(true);
        scrollAreaWidgetContents->setAcceptDrops(true);
        gridLayout_5 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_2->addWidget(scrollArea, 0, 0, 1, 1);

        dockWidget = new ViewportWidget(ShareCameraQt,scrollAreaWidgetContents);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        sizePolicy.setHeightForWidth(dockWidget->sizePolicy().hasHeightForWidth());
        dockWidget->setSizePolicy(sizePolicy);
        dockWidget->setMinimumSize(QSize(1073, 500));
        dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidget->setWidget(dockWidgetContents);
        ShareCameraQt->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidget);
        menuBar = new QMenuBar(ShareCameraQt);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1118, 22));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        ShareCameraQt->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuTools->addAction(actionFlip_Meshes);
        menuTools->addAction(actionWrite_Back_Meshes);
        menuFile->addAction(actionOpen_vtk_Files);

        retranslateUi(ShareCameraQt);

        QMetaObject::connectSlotsByName(ShareCameraQt);
    } // setupUi

    void retranslateUi(QMainWindow *ShareCameraQt)
    {
        ShareCameraQt->setWindowTitle(QApplication::translate("ShareCameraQt", "ShareCameraQt", 0, QApplication::UnicodeUTF8));
        actionOpenFile->setText(QApplication::translate("ShareCameraQt", "Open File...", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("ShareCameraQt", "Exit", 0, QApplication::UnicodeUTF8));
        actionPrint->setText(QApplication::translate("ShareCameraQt", "Print", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("ShareCameraQt", "Help", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("ShareCameraQt", "Save", 0, QApplication::UnicodeUTF8));
        actionFlip_Meshes->setText(QApplication::translate("ShareCameraQt", "Flip Meshes", 0, QApplication::UnicodeUTF8));
        actionWrite_Back_Meshes->setText(QApplication::translate("ShareCameraQt", "Write Back Meshes", 0, QApplication::UnicodeUTF8));
        actionOpen_vtk_Files->setText(QApplication::translate("ShareCameraQt", "Open .vtk Files", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("ShareCameraQt", "Control Panel", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("ShareCameraQt", "Visualization Options", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("ShareCameraQt", "Select Color Map", 0, QApplication::UnicodeUTF8)
        );
        groupBox_5->setTitle(QApplication::translate("ShareCameraQt", "View Options", 0, QApplication::UnicodeUTF8));
        checkBox_9->setText(QApplication::translate("ShareCameraQt", "View All Meshes", 0, QApplication::UnicodeUTF8));
        checkBox_10->setText(QApplication::translate("ShareCameraQt", "View Meshes in", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("ShareCameraQt", "columns", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("ShareCameraQt", "Desynchronize Meshes", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ShareCameraQt", "View Axes", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("ShareCameraQt", "+Z", 0, QApplication::UnicodeUTF8));
        toolButton_2->setText(QApplication::translate("ShareCameraQt", "-Z", 0, QApplication::UnicodeUTF8));
        toolButton_4->setText(QApplication::translate("ShareCameraQt", "-X", 0, QApplication::UnicodeUTF8));
        toolButton_3->setText(QApplication::translate("ShareCameraQt", "+X", 0, QApplication::UnicodeUTF8));
        toolButton_5->setText(QApplication::translate("ShareCameraQt", "+Y", 0, QApplication::UnicodeUTF8));
        toolButton_6->setText(QApplication::translate("ShareCameraQt", "-Y", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("ShareCameraQt", "Tools", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("ShareCameraQt", "File", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ShareCameraQt: public Ui_ShareCameraQt {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHARECAMERAQT_H
