#-------------------------------------------------
#
# Project created by QtCreator 2018-12-26T13:58:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = make_mask
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


SOURCES += \
        main.cpp \
    about_dialog.cpp \
    image_canvas.cpp \
    image_mask.cpp \
    label_widget.cpp \
    labels.cpp \
    main.cpp \
    main_window.cpp \
    utils.cpp

HEADERS += \
    about_dialog.h \
    image_canvas.h \
    image_mask.h \
    label_widget.h \
    labels.h \
    main_window.h \
    pixel_annotation_tool_version.h \
    utils.h
	
INCLUDEPATH += C:/opencv-3.4.1/xy_opencv/include

LIBS += -LC:/opencv-3.4.1/xy_opencv/x64_static/lib/ \
        -lopencv_world341 \
        -lopencv_ts341 \
        -lIlmImf \
        -lippicvmt \
        -lippiw \
        -littnotify \
        -llibjasper \
        -llibjpeg \
        -llibpng \
        -llibprotobuf \
        -llibtiff \
        -llibwebp \
        -lzlib


FORMS += \
    main_window.ui

SUBDIRS += \
    make_mask.pro
