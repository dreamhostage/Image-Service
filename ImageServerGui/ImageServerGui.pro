#-------------------------------------------------
#
# Project created by QtCreator 2019-09-07T14:23:56
#
#-------------------------------------------------

QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageServerGui
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        settings.cpp \
        settingsdescription.cpp \
        tviewer.cpp

HEADERS += \
        mainwindow.h \
        settings.h \
        settingsdescription.h \
        tviewer.h

FORMS += \
        mainwindow.ui \
        settings.ui \
        settingsdescription.ui \
        tviewer.ui

RC_FILE = resources.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/../ImageService/x64/Release/ -lImageServerLib

INCLUDEPATH += $$PWD/../ImageService/ImageServerLib
DEPENDPATH += $$PWD/../ImageService/ImageServerLib

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../ImageService/x64/Release/ImageServerLib.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../ImageService/x64/Release/libImageServerLib.a

RESOURCES += \
    res.qrc
