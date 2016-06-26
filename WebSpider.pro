#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T15:15:42
#
#-------------------------------------------------

QT       += core gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebSpider
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            node.cpp \
            model.cpp \
            threadparser.cpp \
            searchengine.cpp

HEADERS  += mainwindow.h \
            node.h \
            model.h \
            threadparser.h \
            searchengine.h

FORMS    += mainwindow.ui

