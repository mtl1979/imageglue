TEMPLATE = app
SOURCES = main.cpp mainwindow.cpp imageview.cpp preview.cpp util.cpp
HEADERS = mainwindow.h imageview.h preview.h
TRANSLATIONS = imageglue_en.ts imageglue_fi.ts
QT += widgets
CONFIG += lrelease lupdate
win32: LIBS += shlwapi.lib