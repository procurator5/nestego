#-------------------------------------------------
#
# Project created by QtCreator 2015-07-04T11:52:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nestego
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vneuralanalyse.cpp \
    vgraphicsproxywidget.cpp \
    vlineeditdelegate.cpp \
    vchoosedirectory.cpp \
    vanalysebuffer.cpp \
    edge.cpp \
    node.cpp \
    vabufferform.cpp \
    buffer.cpp \
    vibuffer.cpp

HEADERS  += mainwindow.h \
    vneuralanalyse.h \
    vgraphicsproxywidget.h \
    vlineeditdelegate.h \
    vchoosedirectory.h \
    vanalysebuffer.h \
    edge.h \
    node.h \
    vabufferform.h \
    buffer.h \
    vibuffer.h

FORMS    += mainwindow.ui \
    vneuralanalyse.ui \
    vchoosedirectory.ui \
    vanalysebuffer.ui \
    vabufferform.ui

RESOURCES += \
    icons.qrc
