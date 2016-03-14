TEMPLATE = app
win32:TARGET=ImageGlue
SOURCES = main.cpp mainwindow.cpp imageview.cpp preview.cpp util.cpp
HEADERS = mainwindow.h imageview.h preview.h
TRANSLATIONS = imageglue_en.ts imageglue_fi.ts
QT += widgets
CONFIG += lrelease lupdate
RESOURCES = imageglue.qrc
win32 {
	LIBS += shlwapi.lib
	RC_FILE = imageglue.rc
}