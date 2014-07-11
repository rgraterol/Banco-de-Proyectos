#-------------------------------------------------
#
# Project created by QtCreator 2014-05-29T14:46:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += sql widgets widgets

TARGET = Mproject
TEMPLATE = app
QT += printsupport

SOURCES += main.cpp\
        mainwindow.cpp \
    pdfprinter_db.cpp \
    ayuda.cpp \
    tools.cpp \
    csv_manejador_db.cpp \


HEADERS  += mainwindow.h \
    proyecto.h \
    createConnection.h \
    pdfprinter_db.h \
    ayuda.h \
    tools.h \
    csv_manejador_db.h \
    sqlmanager.h

FORMS    += mainwindow.ui \
    ayuda.ui \
    tools.ui

wince*: {
    CONFIG(debug, debug|release):sqlPlugins.files = $$QT_BUILD_TREE/plugins/sqldrivers/*d4.dll
    CONFIG(release, debug|release):sqlPlugins.files = $$QT_BUILD_TREE/plugins/sqldrivers/*[^d]4.dll
    sqlPlugins.path = sqldrivers
    DEPLOYMENT += sqlPlugins
}
