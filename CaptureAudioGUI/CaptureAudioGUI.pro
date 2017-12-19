#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T22:48:11
#
#-------------------------------------------------

QT       += core gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CaptureAudioGUI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        maindialog.cpp \
    titlebuttons.cpp \
    objectminiwidget.cpp \
    objectdialog.cpp \
    objectinfowidget.cpp \
    transformwidget.cpp \
    timewidget.cpp \
    formatwidget.cpp \
    newdialog.cpp \
    capturedialog.cpp \
    ncframelesshelper.cpp \
    settingdialog.cpp \
    singleapplication.cpp

HEADERS += \
        maindialog.h \
    titlebuttons.h \
    objectminiwidget.h \
    objectdialog.h \
    objectinfowidget.h \
    transformwidget.h \
    timewidget.h \
    formatwidget.h \
    newdialog.h \
    capturedialog.h \
    ncframelesshelper.h \
    settingdialog.h \
    singleapplication.h

FORMS += \
        maindialog.ui \
    objectdialog.ui \
    newdialog.ui \
    capturedialog.ui \
    settingdialog.ui

RESOURCES += \
    resource.qrc

TRANSLATIONS += AudioCapturer_zh_CN.ts \
                AudioCapturer_en_US.ts

RC_ICONS = MainIcon.ico
