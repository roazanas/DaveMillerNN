QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filepathvalidator.cpp \
    main.cpp \
    mainwindow.cpp \
    neuron.cpp \
    nnet.cpp \
    trainer.cpp

HEADERS += \
    filepathvalidator.h \
    mainwindow.h \
    neuron.h \
    nnet.h \
    trainer.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    translations/DaveMillerNN_ru.ts


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
