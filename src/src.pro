TEMPLATE = app
win32:TARGET=ImageGlue
GUID={DBFAD74C-D75C-3B14-919A-C77651A75A23}
SOURCES = main.cpp mainwindow.cpp imageview.cpp preview.cpp util.cpp
HEADERS = mainwindow.h imageview.h preview.h
TRANSLATIONS = imageglue_en.ts imageglue_fi.ts
QT += widgets svg
CONFIG += lrelease lupdate
RESOURCES = imageglue.qrc
win32 {
	LIBS += shlwapi.lib
	RC_FILE = imageglue.rc
	qtlibs.files = $$[QT_INSTALL_LIBS]\\Qt5Core.dll \
			       $$[QT_INSTALL_LIBS]\\Qt5Gui.dll \
			       $$[QT_INSTALL_LIBS]\\Qt5Svg.dll \
			       $$[QT_INSTALL_LIBS]\\Qt5Widgets.dll \
			       $$[QT_INSTALL_LIBS]\\libGLESv2.dll
	qtlibs.path = ../..
	qtlibs.CONFIG += recursive
	INSTALLS += qtlibs

	qtimageformats.path = ../../plugins/imageformats
	qtimageformats.files = $$[QT_INSTALL_PLUGINS]\\imageformats\\qsvg.dll \
			       $$[QT_INSTALL_PLUGINS]\\imageformats\\qgif.dll \
			       $$[QT_INSTALL_PLUGINS]\\imageformats\\qico.dll \
			       $$[QT_INSTALL_PLUGINS]\\imageformats\\qjpeg.dll \
			       $$[QT_INSTALL_PLUGINS]\\imageformats\\qmng.dll \
			       $$[QT_INSTALL_PLUGINS]\\imageformats\\qtga.dll \
			       $$[QT_INSTALL_PLUGINS]\\imageformats\\qtiff.dll

	qtimageformats.CONFIG += recursive
	INSTALLS += qtimageformats

	qtplatforms.path = ../../plugins/platforms
	qtplatforms.files = $$[QT_INSTALL_PLUGINS]\\platforms\\qwindows.dll
	qtplatforms.CONFIG += recursive
	INSTALLS += qtplatforms

	qtconf.path = ../..
	qtconf.files = ../qt.conf
	qtconf.CONFIG += recursive
	INSTALLS += qtconf
}

target.path = ../..
INSTALLS += target

!equals(TEMPLATE, vcapp) {
translations.files = $$TRANSLATIONS
translations.files ~= s/\\.ts/.qm/g
translations.path = ../../translations
translations.CONFIG += recursive
INSTALLS += translations
}