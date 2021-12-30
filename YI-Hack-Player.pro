QT       += core gui av avwidgets sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 avwidgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/camera.cpp \
    src/cameradao.cpp \
    src/camerainfowidget.cpp \
    src/database.cpp \
    src/ftpdirswidget.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/qt5ftp/qftp/qftp.cpp \
    src/qt5ftp/qftp/qurlinfo.cpp \
    src/ftpclient-cpp/FTP/FTPClient.cpp \
    src/ftpclient-cpp/FTP/CurlHandle.cpp

HEADERS += \
    src/camera.h \
    src/cameradao.h \
    src/camerainfowidget.h \
    src/database.h \
    src/ftpdirswidget.h \
    src/mainwindow.h \
    src/noteditabledelegate.h \
    src/qt5ftp/qftp/qftp.h \
    src/qt5ftp/qftp/qurlinfo.h \
    src/ftpclient-cpp/FTP/FTPClient.h \
    src/ftpclient-cpp/FTP/CurlHandle.h


FORMS += \
    src/camerainfowidget.ui \
    src/ftpdirswidget.ui \
    src/mainwindow.ui

TRANSLATIONS += \
    YI-Hack-Player_pt_BR.ts
CONFIG += lrelease
CONFIG += embed_translations

unix:!mac {
LIBS += -L$$QT.core.libs -lQtAV
}
unix:mac {
INCLUDEPATH += $$QT.core.libs/QtAV.framework/Versions/1/Headers
QMAKE_LFLAGS += -F$$QT.core.libs
LIBS += -framework QtAV
}
win32{
LIBS += -L$$QT.core.libs -lQtAV1 -llibQtAVWidgets1
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data/resources.qrc


unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libcurl

DISTFILES += \
    CMakeLists.txt
