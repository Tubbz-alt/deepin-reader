QT += core gui svg sql printsupport
QT += dtkwidget
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = deepin_reader
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += x11 poppler-qt5 ddjvuapi
LIBS+=-luuid


include (frame/frame.pri)
include (utils/utils.pri)
include (controller/controller.pri)
include (pdfControl/pdfControl.pri)
include (docview/docview.pri)
include (subjectObserver/subjectObserver.pri)
include (mainShow/mainShow.pri)
include (translator/translator.pri)

SOURCES += \
        main.cpp \
    application.cpp\

RESOURCES +=         resources.qrc

HEADERS +=\
    application.h \
    translator/Frame.h \
    translator/Font.h \
    translator/Thumbnail.h \
    translator/BookMark.h \
    translator/Note.h \
    translator/Search.h


DISTFILES += \
    themes/solarized_light.theme \
    themes/atom_dark.theme \
    themes/deepin_dark.theme \
    themes/solarized_dark.theme \
    themes/deepin.theme

APPICONDIR = $$PREFIX/share/icons/deepin/apps/scalable

app_icon.path = $$APPICONDIR
app_icon.files = $$PWD/resources/image/logo/logo.svg
