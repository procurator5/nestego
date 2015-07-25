#-------------------------------------------------
#
# Project created by QtCreator 2015-07-04T11:52:04
#
#-------------------------------------------------

QT       += core gui
LIBS += -lfann

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nestego
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vneuralanalyse.cpp \
    vgraphicsproxywidget.cpp \
    vlineeditdelegate.cpp \
    vchoosedirectory.cpp \
    edge.cpp \
    node.cpp \
    vabufferform.cpp \
    buffer.cpp \
    vibuffer.cpp \
    vanalyserform.cpp \
    brain.cpp \
    analyse.cpp \
    vedgedialog.cpp

HEADERS  += mainwindow.h \
    vneuralanalyse.h \
    vgraphicsproxywidget.h \
    vlineeditdelegate.h \
    vchoosedirectory.h \
    edge.h \
    node.h \
    vabufferform.h \
    buffer.h \
    vibuffer.h \
    vanalyserform.h \
    brain.h \
    analyse.h \
    vedgedialog.h \
    fann.h \
    fann_activation.h \
    fann_cascade.h \
    fann_cpp.h \
    fann_data.h \
    fann_error.h \
    fann_internal.h \
    fann_io.h \
    fann_train.h \
    fixedfann.h

FORMS    += mainwindow.ui \
    vneuralanalyse.ui \
    vchoosedirectory.ui \
    vanalysebuffer.ui \
    vabufferform.ui \
    vanalyserform.ui \
    vedgedialog.ui

RESOURCES += \
    icons.qrc
