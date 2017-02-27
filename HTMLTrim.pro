#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T07:17:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HTMLTrim
TEMPLATE = app


SOURCES += main.cpp \
    __html_read_to.cpp \
    html_css.cpp \
    html_optimize.cpp \
    html_parser.cpp \
    html_preprocess.cpp \
    htmltrim.cpp \
    stdafx.cpp \
    dialogsettings.cpp

HEADERS  += \
    __html_read_to.h \
    html_element.h \
    html_css.h \
    html_optimize.h \
    html_parser.h \
    html_preprocess.h \
    htmltrim.h \
    stdafx.h \
    dialogsettings.h

FORMS    += \
    htmltrim.ui \
    dialogsettings.ui

CONFIG += c++11

CONFIG+=precompile_header
PRECOMPILED_HEADER = stdafx.h
