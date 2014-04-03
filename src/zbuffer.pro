#-------------------------------------------------
#
# Project created by QtCreator 2014-03-27T14:56:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zbuffer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    scene.cpp \
    zbuffer.cpp \
    zbufferGraphicsView.cpp

HEADERS  += mainwindow.h \
    camera.h \
    geometry.h \
    matrix4x4.h \
    scene.h \
    triangleMesh.h \
    zbuffer.h \
    zbufferGraphicsView.h

FORMS    += mainwindow.ui
