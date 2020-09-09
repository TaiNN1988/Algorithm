TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS +=\-lboost_system\

SOURCES += \
    Lesson1/MergeSort.cpp \
    Lesson1/MutipleInteger.cpp \
    Lesson2/InversionCount.cpp \
    Lesson2/MutipleMatrix.cpp \
    Lesson2/closerpair.cpp \
    Main.cpp \
    utilities.cpp

HEADERS += \
    Lesson1/MergeSort.h \
    Lesson1/MutipleInteger.h \
    Lesson2/InversionCount.h \ \
    Lesson2/MutipleMatrix.h \
    Lesson2/closerpair.h \
    utilities.h
