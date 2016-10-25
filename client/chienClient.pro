
TEMPLATE = app
CONFIG += release
QT += widgets network
TARGET = chienClient
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += FenClient.h perso.h
FORMS += FenClient.ui
SOURCES += FenClient.cpp main.cpp
