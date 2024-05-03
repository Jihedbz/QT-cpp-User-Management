QT       += core gui
QT += axcontainer
QT+= sql charts
QT+= printsupport
QT+= charts serialport
QT+= location qml quickwidgets
QT+= core network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console
QMAKE_CXXFLAGS += -std=gnu++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aides.cpp \
    arduino.cpp \
    benif.cpp \
    calculator.cpp \
    connection.cpp \
    employee.cpp \
    event.cpp \
    main.cpp \
    mainwindow.cpp \
    messengerconnectiondialog.cpp \
    messengerlanclient.cpp \
    messengerserver.cpp \
    messengersocket.cpp \
    qrcodegen.cpp \
    restaurant.cpp \
    user.cpp

HEADERS += \
    aides.h \
    arduino.h \
    benif.h \
    calculator.h \
    connection.h \
    employee.h \
    event.h \
    mainwindow.h \
    messengerconnectiondialog.h \
    messengerlanclient.h \
    messengerserver.h \
    messengersocket.h \
    qrcodegen.hpp \
    restaurant.h \
    user.h

FORMS += \
    calculator.ui \
    mainwindow.ui \
    messengerconnectiondialog.ui \
    messengerlanclient.ui

#LIBS += -L"C:\Program Files\OpenSSL-Win64\lib" -lssl -lcrypto
#INCLUDEPATH += "C:\Program Files\OpenSSL-Win64\include"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Ressources.qrc

TARGET = your_app
TEMPLATE = app
QT += widgets
QT += sql
QT += charts

QT += printsupport
QT += core gui network
QT += core gui sql axcontainer printsupport


