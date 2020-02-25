QT += core network sql

TARGET = LYXF-Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    startserver.cpp \
    backprosess.cpp

HEADERS += \
    startserver.h \
    backprosess.h

CONFIG += C++11
