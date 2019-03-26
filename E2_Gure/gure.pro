QT       += core gui widgets

TARGET = template
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/mainwidget.cpp \
    src/geometryengine.cpp

HEADERS += \
    src/mainwidget.h \
    src/geometryengine.h

RESOURCES += \
    ressources/shaders.qrc \

DISTFILES += \
    src/rectangle
