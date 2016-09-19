#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T12:47:04
#
#-------------------------------------------------

QT       += core gui axcontainer sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = com_connection
TEMPLATE = app


CONFIG += console axcontainer c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    employee.cpp \
    comconnector.cpp \
    base_con.cpp \
    ccon_iiko.cpp \
    ccon_legos.cpp \
    dbconnector.cpp \
    person.cpp \
    ccon_anviz.cpp \
    logger.cpp \
    ccon_tc.cpp \
    ccon_file.cpp \
    dolgnost.cpp \
    profile.cpp \
    door.cpp \
    syncronizer.cpp \
    commonfunction.cpp \
    runparams.cpp

HEADERS  += mainwindow.h \
    employee.h \
    comconnector.h \
    base_con.h \
    ccon_iiko.h \
    ccon_legos.h \
    dbconnector.h \
    person.h \
    ccon_anviz.h \
    logger.h \
    constansts.h \
    ccon_tc.h \
    ccon_file.h \
    dolgnost.h \
    profile.h \
    door.h \
    syncronizer.h \
    commonfunction.h \
    runparams.h

FORMS    += mainwindow.ui

#LIBS += "C:\\Program Files (x86)\\Windows Kits\\8.1\\Lib\\winv6.3\\um\\x64\\" -lOle32
LIBS += -lOle32 -luser32 -lkernel32 -lstrmiids -luuid -loleaut32
#LIBS += "c:/Qt/Qt5.3.2/5.3/mingw482_32/plugins/sqldrivers/qsqlodbc.lib"

INCLUDEPATH += "c:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/atlmfc/include/"
#DEPENDPATH += "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64"

include(qtservice/qtservice.pri)
