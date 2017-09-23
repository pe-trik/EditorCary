#-------------------------------------------------
#
# Project created by QtCreator 2017-09-02T11:23:12
#
#-------------------------------------------------

QT       += core gui svg xml

CONFIG += c++17

QMAKE_CFLAGS_RELEASE    = -O2
QMAKE_LFLAGS_RELEASE = /LTCG

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

TARGET = EditorCary
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        hlavneokno.cpp \
    hlavneokno.cpp \
    main.cpp \
    pracovnaplocha.cpp \
    Dokumenty/dokument.cpp \
    Dokumenty/vlastnost.cpp \
    vlastnostmanager.cpp \
    panelvlastnosti.cpp \
    Dokumenty/qrealvlastnost.cpp \
    qrealvlastnostmanager.cpp \
    Nastroje/nastroj.cpp \
    Nastroje/kurzor.cpp \
    Komponenty/komponent.cpp \
    Komponenty/manipulator.cpp \
    Komponenty/ciara.cpp \
    Komponenty/dvojbodovy.cpp \
    Nastroje/dvojbodovynastroj.cpp \
    Nastroje/ciaranastroj.cpp \
    Nastroje/ciaranastroj.cpp \
    Komponenty/spojenie.cpp \
    Komponenty/spojenieslot.cpp \
    Komponenty/spline.cpp \
    Nastroje/splinenastroj.cpp \
    Dokumenty/stringvlastnost.cpp \
    stringvlastnostmanager.cpp \
    Dokumenty/spojeniezoznamvlastnost.cpp \
    spojeniezoznamvlastnostmanager.cpp \
    Komponenty/splinegroup.cpp \
    Komponenty/tridiagonalnamatica.cpp \
    Komponenty/volnaciara.cpp \
    Nastroje/volnaciaranastroj.cpp \
    Dokumenty/boolvlastnost.cpp \
    boolvlastnostmanager.cpp \
    Komponenty/prerusenie.cpp \
    Nastroje/prerusenienastroj.cpp \
    Kontroly/kontrola.cpp \
    Kontroly/chyba.cpp \
    Kontroly/kontrolaokrajov.cpp \
    kontrolypanel.cpp \
    chybypanel.cpp \
    Kontroly/kontrolazakrivenia.cpp \
    Kontroly/kontroladlzky.cpp \
    Kontroly/kontrolaspojeni.cpp

HEADERS += \
        hlavneokno.h \
    hlavneokno.h \
    pracovnaplocha.h \
    Dokumenty/dokument.h \
    Dokumenty/vlastnost.h \
    vlastnostmanager.h \
    panelvlastnosti.h \
    Dokumenty/qrealvlastnost.h \
    qrealvlastnostmanager.h \
    Nastroje/nastroj.h \
    Nastroje/kurzor.h \
    Komponenty/komponent.h \
    Komponenty/manipulator.h \
    Komponenty/ciara.h \
    Komponenty/dvojbodovy.h \
    Nastroje/dvojbodovynastroj.h \
    Nastroje/ciaranastroj.h \
    Nastroje/ciaranastroj.h \
    Komponenty/spojenie.h \
    Komponenty/spojenieslot.h \
    Komponenty/spline.h \
    Nastroje/splinenastroj.h \
    Dokumenty/stringvlastnost.h \
    stringvlastnostmanager.h \
    Dokumenty/spojeniezoznamvlastnost.h \
    spojeniezoznamvlastnostmanager.h \
    Komponenty/splinegroup.h \
    Komponenty/tridiagonalnamatica.h \
    Komponenty/volnaciara.h \
    Nastroje/volnaciaranastroj.h \
    Dokumenty/boolvlastnost.h \
    boolvlastnostmanager.h \
    Komponenty/prerusenie.h \
    Nastroje/prerusenienastroj.h \
    Kontroly/kontrola.h \
    Kontroly/chyba.h \
    Kontroly/kontrolaokrajov.h \
    kontrolypanel.h \
    chybypanel.h \
    Kontroly/kontrolazakrivenia.h \
    Kontroly/kontroladlzky.h \
    Kontroly/kontrolaspojeni.h

FORMS += \
        hlavneokno.ui

RESOURCES += \
    ikonky.qrc

DISTFILES += \
    ikonky/ciara.svg \
    ikonky/KruhovyVysek.svg \
    ikonky/kursor.svg \
    ikonky/odbocka.svg \
    ikonky/spline.svg \
    ikonky/volnaciara.svg \
    ikonky/prerusenie.svg
