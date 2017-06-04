win32-g++: {
    INCLUDEPATH += C:/Qt/zlib-1.2.11
    INCLUDEPATH += C:/Qt/quazip-0.7.3
    LIBS += -LC:/Qt/zlib-1.2.11 -lz
    LIBS += -LC:/Qt/quazip-0.7.3/quazip/release -lquazip
}

win32-msvc: {
    LIBS += C:/Qt/zlib-1.2.8/zlib.lib
    LIBS += C:/Qt/quazip-0.7.2/quazip/release/quazip.lib
}

unix: {
    CONFIG += link_pkgconfig
    PKGCONFIG += zlib

    LIBS += -L/usr/lib -lquazip5
    INCLUDEPATH += /usr/include/quazip5
}
